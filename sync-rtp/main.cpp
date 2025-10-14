#include <gst/gst.h>
#include <iostream>
// #include "gst/video/gstvideometa.h"
// #include "my_frame_id_meta.h" 

// #include "gstmyfilter.h"

#define CUSTOM_UUID_LEN 16

// static GstPadProbeReturn cb_have_data(GstPad *pad, GstPadProbeInfo *info, gpointer user_data) {
//     gint x, y;
//     GstMapInfo map;
//     GstMapInfo map2;
//     guint8 *ptr;
//     guint16 *ptr2;
//     GstBuffer *buffer;
//     GstBuffer *buffer2;

//     GstElement *rtph264depay = (GstElement *)user_data;


//     buffer = GST_PAD_PROBE_INFO_BUFFER (info);
//     buffer = gst_buffer_make_writable (buffer);

//     /* Making a buffer writable can fail (for example if it
//     * cannot be copied and is used more than once)
//     */
//      if (buffer == NULL)
//         return GST_PAD_PROBE_OK;

//     /* Mapping a buffer can fail (non-writable) */
//     if (gst_buffer_map (buffer, &map, GST_MAP_WRITE)) {
        
//         ptr = (guint8 *) map.data;
//         for (int i =0; i < 20; i++)
//             std::cout << int(ptr[i]) << ",";
        
//         std::cout<< std::endl;

//         // for (int i = 5; i < map.size-1; i++) {
//         //     ptr[i] = ptr[i+1];
//         // }

//         for (int i =0; i < 20; i++)
//             std::cout << int(ptr[i]) << ",";

//         std::cout << "\ts: " << map.size << std::endl;
//         // for (int i = 0; i < map.size - 1; i++)
//         //     ptr[i] = ptr[i];

//         // map.size = map.size - 1;
//         gst_buffer_unmap (buffer, &map);
//     }
//     // std::cout << "im heree \n";

//     GST_PAD_PROBE_INFO_DATA(info) = buffer;


//     return GST_PAD_PROBE_OK;
// }

struct CustomData {
    guint8* ptr;
    int size;

    GstElement *appsink, *appsrc;
    guint sourceid;

};


static gboolean push_data(CustomData *data) {
    GstBuffer *buffer;
    GstFlowReturn ret;
    int i;
    GstMapInfo map;
    //   gint num_samples = CHUNK_SIZE / 2; /* Because each sample is 16 bits */

    /* Create a new empty buffer */
    buffer = gst_buffer_new_and_alloc (data->size);
    // buffer = gst_buffer_make_writable (buffer);
    /* Set its timestamp and duration */
    //   GST_BUFFER_TIMESTAMP (buffer) = gst_util_uint64_scale (data->num_samples, GST_SECOND, SAMPLE_RATE);
    //   GST_BUFFER_DURATION (buffer) = gst_util_uint64_scale (num_samples, GST_SECOND, SAMPLE_RATE);

    /* Generate some psychodelic waveforms */
    gst_buffer_map (buffer, &map, GST_MAP_WRITE);
    guint8 *raw = (guint8 *)map.data;
    for (int i =0; i <data->size; i++) {
        raw[i] = data->ptr[i];
    }
    gst_buffer_unmap (buffer, &map);
    /* Push the buffer into the appsrc */
    g_signal_emit_by_name (data->appsrc, "push-buffer", buffer, &ret);

    /* Free the buffer now that we are done with it */
    gst_buffer_unref (buffer);

    if (ret != GST_FLOW_OK) {
        /* We got some error, stop sending data */
        return FALSE;
    }

    return TRUE;
}


/* The appsink has received a buffer */
static GstFlowReturn new_sample (GstElement *sink, CustomData *data) {
    GstSample *sample;
    GstFlowReturn ret;
    GstMapInfo map;
    guint8 *ptr;
    /* Retrieve the buffer */
    // std::cout << "im here in new sample\n";
    g_signal_emit_by_name (sink, "pull-sample", &sample);
    if (sample) {
        GstBuffer* rec_buff = gst_sample_get_buffer(sample);
        gst_buffer_map (rec_buff, &map, GST_MAP_READ);
        data->size = map.size;
        guint8 *raw = (guint8 *)map.data;
        for (int i =0; i <data->size; i++) {
            data->ptr[i] = raw[i];
        }
        // // std::cout << "im hree\n";
        gst_buffer_unmap (rec_buff, &map);
        
        push_data(data);
        
        gst_sample_unref (sample);
        return GST_FLOW_OK;
    }

  return GST_FLOW_ERROR;
}


/* This signal callback triggers when appsrc needs data. Here, we add an idle handler
 * to the mainloop to start pushing data into the appsrc */
static void start_feed (GstElement *source, guint size, CustomData *data) {
    if (data->sourceid == 0) {
        g_print ("Start feeding\n");
        data->sourceid = g_idle_add ((GSourceFunc) push_data, data);
    }
}

/* This callback triggers when appsrc has enough data and we can stop sending.
 * We remove the idle handler from the mainloop */
static void stop_feed (GstElement *source, CustomData *data) {
  if (data->sourceid != 0) {
    g_print ("Stop feeding\n");
    g_source_remove (data->sourceid);
    data->sourceid = 0;
  }
}



gint main (gint   argc, gchar *argv[]) {
    GMainLoop *loop;
    GstElement*udpsrc, *capsfilter, *queue, 
    // *appsink, *appsrc,
    *rtph264depay,*h264parse,
    *nvh264dec,*videoconvert,*autovideosink;

    GstElement *pipeline1;
    GstElement *pipeline2;
    

    CustomData data;
    GstCaps *filtercaps, *caps;
    GstPad *pad;

    /* init GStreamer */
    gst_init (&argc, &argv);
    loop = g_main_loop_new (NULL, FALSE);

    data.ptr = new guint8[1920*1080*4];

    /* build */
    pipeline1 = gst_pipeline_new ("my-pipeline1");
    pipeline2 = gst_pipeline_new ("my-pipeline2");

    udpsrc = gst_element_factory_make ("udpsrc", "udp_src");
    if (udpsrc == NULL)
        g_error ("Could not create 'udpsrc' element");

    data.appsrc = gst_element_factory_make ("appsrc", "app_src");
    if (data.appsrc == NULL)
        g_error ("Could not create 'data.appsrc' element");

    data.appsink = gst_element_factory_make ("appsink", "app_sink");
    if (data.appsink == NULL)
        g_error ("Could not create 'data.appsink' element");

    capsfilter = gst_element_factory_make ("capsfilter", "caps_filter");
    g_assert (capsfilter != NULL); /* should always exist */

    queue = gst_element_factory_make ("queue", "queue0");
    if (queue == NULL)
        g_error ("Could not create 'queue' element");
    rtph264depay = gst_element_factory_make ("rtph264depay", "rtph264_depay");
    if (udpsrc == NULL)
        g_error ("Could not create 'udpsrc' element");
    h264parse = gst_element_factory_make ("h264parse", "h264_parse");
    if (udpsrc == NULL)
        g_error ("Could not create 'udpsrc' element");
    

    nvh264dec = gst_element_factory_make ("nvh264dec", "nvh264_dec");
    if (nvh264dec == NULL)
        g_error ("Could not create 'nvh264dec' element");

    videoconvert = gst_element_factory_make ("videoconvert", "video_convert");
    if (videoconvert == NULL)
        g_error ("Could not create 'videoconvert' element");

    autovideosink = gst_element_factory_make ("autovideosink", "sink");
    if (autovideosink == NULL) 
        g_error ("Could not create neither 'xvimagesink' nor 'ximagesink' element");


    gst_bin_add_many (GST_BIN (pipeline1), udpsrc, capsfilter, queue, 
        rtph264depay, data.appsink, NULL);

    gst_bin_add_many (GST_BIN (pipeline2),data.appsrc,h264parse,
        nvh264dec,videoconvert,autovideosink, NULL);

    gst_element_link_many(udpsrc, capsfilter, rtph264depay,data.appsink, NULL);

    gst_element_link_many(data.appsrc,h264parse, nvh264dec,videoconvert,autovideosink, NULL);



    filtercaps = gst_caps_new_simple ("application/x-rtp",
                "media", G_TYPE_STRING, "video",
                "clock-rate", G_TYPE_INT, 90000,
                "encoding-name", G_TYPE_STRING, "H264",
                "payload", G_TYPE_INT, 96,
                NULL);


                
    g_object_set (G_OBJECT (capsfilter), "caps", filtercaps, NULL);



    caps = gst_caps_new_simple("video/x-h264",
                    "stream-format", G_TYPE_STRING, "byte-stream",
                    "alignment", G_TYPE_STRING, "au",
                    NULL);
    
    
    gst_caps_unref(filtercaps);
    
    g_object_set(G_OBJECT (udpsrc), "uri", "udp://224.1.1.3:5000", NULL);
    g_object_set(G_OBJECT (queue), "max-size-buffers", 1, NULL);
    g_object_set(G_OBJECT (autovideosink), "sync", FALSE, NULL);


    // g_object_set (data.appsrc, "caps", caps,  "format", GST_FORMAT_TIME, NULL);
    g_signal_connect (data.appsrc, "need-data", G_CALLBACK (start_feed), &data);
    g_signal_connect (data.appsrc, "enough-data", G_CALLBACK (stop_feed), &data);

    
    /* Configure appsink */
    g_object_set (data.appsink, "emit-signals", TRUE, "caps", caps, NULL);
    g_signal_connect (data.appsink, "new-sample", G_CALLBACK (new_sample), &data);

    gst_caps_unref(caps);





    // pad = gst_element_get_static_pad (udpsrc, "src");
    // gst_pad_add_probe (pad, GST_PAD_PROBE_TYPE_BUFFER,
    //     (GstPadProbeCallback) cb_have_data, NULL, NULL);
    // gst_object_unref (pad);

    // pad = gst_element_get_static_pad (rtph264depay, "src");
    // gst_pad_add_probe (pad, GST_PAD_PROBE_TYPE_BUFFER,
    //     (GstPadProbeCallback) cb_have_data, (gpointer)rtph264depay, NULL);
    // gst_object_unref (pad);

    /* run */
    gst_element_set_state (pipeline1, GST_STATE_PLAYING);
    gst_element_set_state (pipeline2, GST_STATE_PLAYING);

    /* wait until it's up and running or failed */
    if (gst_element_get_state (pipeline1, NULL, NULL, -1) == GST_STATE_CHANGE_FAILURE) {
        g_error ("Failed to go into PLAYING state");
    }

    g_print ("Running ...\n");
    g_main_loop_run (loop);

    /* exit */
    gst_element_set_state (pipeline1, GST_STATE_NULL);
    gst_object_unref (pipeline1);

    gst_element_set_state (pipeline2, GST_STATE_NULL);
    gst_object_unref (pipeline2);

    delete data.ptr;

    return 0;
}


