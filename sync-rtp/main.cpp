#include <gst/gst.h>
#include <iostream>
// #include "my_frame_id_meta.h" 
// #include "gst/video/gstvideometa.h"

#include "gstmyfilter.h"

#define CUSTOM_UUID_LEN 16



static GstPadProbeReturn cb_have_data(GstPad *pad, GstPadProbeInfo *info, gpointer user_data) {
    gint x, y;
    GstMapInfo map;
    GstMapInfo map2;
    guint8 *ptr;
    guint16 *ptr2;
    GstBuffer *buffer;
    GstBuffer *buffer2;

    GstElement *rtph264depay = (GstElement *)user_data;


    buffer = GST_PAD_PROBE_INFO_BUFFER (info);
    buffer = gst_buffer_make_writable (buffer);

    

    /* Making a buffer writable can fail (for example if it
    * cannot be copied and is used more than once)
    */

     if (buffer == NULL)
        return GST_PAD_PROBE_OK;

    /* Mapping a buffer can fail (non-writable) */
    if (gst_buffer_map (buffer, &map, GST_MAP_WRITE)) {
        
        ptr = (guint8 *) map.data;
        for (int i =0; i < 20; i++)
            std::cout << int(ptr[i]) << ",";
        
        std::cout<< std::endl;

        for (int i = 5; i < map.size-1; i++) {
            ptr[i] = ptr[i+1];
        }

        for (int i =0; i < 20; i++)
            std::cout << int(ptr[i]) << ",";

        std::cout << "\ts: " << map.size << std::endl;
        // for (int i = 0; i < map.size - 1; i++)
        //     ptr[i] = ptr[i];

        // map.size = map.size - 1;
        gst_buffer_unmap (buffer, &map);
    }
    // std::cout << "im heree \n";

    GST_PAD_PROBE_INFO_DATA(info) = buffer;


    // if (buffer == NULL)
    //     return GST_PAD_PROBE_OK;

    // /* Mapping a buffer can fail (non-writable) */
    // if (gst_buffer_map (buffer, &map, GST_MAP_WRITE)) {
    //     buffer2 = gst_buffer_new_and_alloc (map.size - 1);
    //     buffer2 = gst_buffer_make_writable (buffer2);
    //     gst_buffer_map (buffer2, &map2, GST_MAP_WRITE);
    //     ptr = (guint16 *) map.data;
    //     ptr2 = (guint16 *) map2.data;
    //     std::cout << int(ptr[0]) << " s: " << map.size << " , "  << map2.size << std::endl;
    //     for (int i = 0; i < map.size - 1; i++)
    //         ptr2[i] = ptr[i + 1];


    //     gst_buffer_unmap (buffer2, &map2);
    //     gst_buffer_unmap (buffer, &map);
    // }
    // // std::cout << "im heree \n";

    // GST_PAD_PROBE_INFO_DATA(info) = buffer2;

    // gst_buffer_unref (buffer2);
    return GST_PAD_PROBE_OK;
}

// static GstPadProbeReturn cb_have_data(GstPad *pad, GstPadProbeInfo *info, gpointer user_data) {
//     GstElement *rtph264depay = (GstElement *)user_data;

//     GstStructure *stat;

//     gint val = 0;
    
//     // g_object_get(rtph264depay, "stats", &stat, NULL);
//     // gst_structure_get_int (stat,"timestamp", &val);
//     // std::cout << val << std::endl;

//     // gint x, y;
//     // GstMapInfo map;
//     // guint16 *ptr, t;
//     GstBuffer *buffer;

//     buffer = GST_PAD_PROBE_INFO_BUFFER (info);

//     std::cout << "im here\n";
//     GstVideoCropMeta *meta = 
//     (GstVideoCropMeta *) gst_buffer_get_meta (buffer, GST_VIDEO_CROP_META_API_TYPE);

//     if (meta) {
//         guint64 received_frame_id = meta->x;

//         std::cout <<"received_frame_id " << received_frame_id << std::endl;
//         // Use 'received_frame_id' as the key to look up the corresponding 
//         // (x, y, w, h) data you received over the separate UDP socket.
        
//         // Store this video frame in a map/queue indexed by 'received_frame_id'.
//     }


//     // auto meta = gst_buffer_get_custom_meta (buffer, "mymeta");
//     // auto metadata = gst_custom_meta_get_structure (meta);
//     // gint64 property;
//     // gst_structure_get_int64 (metadata, "property_name", &property);
//     // std::cout <<property << std::endl;

//     // buffer = gst_buffer_make_writable (buffer);

//     GST_PAD_PROBE_INFO_DATA(info) = buffer;
    
//     return GST_PAD_PROBE_OK;
// }

gint main (gint   argc, gchar *argv[]) {
    GMainLoop *loop;
    GstElement *pipeline, *udpsrc, *capsfilter, *queue,*rtph264depay ,*h264parse,*nvh264dec,*videoconvert,*autovideosink;
    GstElement* myfilter;
    GstCaps *filtercaps;
    GstPad *pad;

    /* init GStreamer */
    gst_init (&argc, &argv);
    loop = g_main_loop_new (NULL, FALSE);

    /* build */
    pipeline = gst_pipeline_new ("my-pipeline");

    udpsrc = gst_element_factory_make ("udpsrc", "udp_src");
    if (udpsrc == NULL)
        g_error ("Could not create 'videotestsrc' element");


    myfilter = gst_element_factory_make ("my_filter", "my_filter");
    if (myfilter == NULL)
        g_error ("Could not create 'myfilter' element"); 

    capsfilter = gst_element_factory_make ("capsfilter", "caps_filter");
    g_assert (capsfilter != NULL); /* should always exist */

    queue = gst_element_factory_make ("queue", "queue0");
    if (queue == NULL)
        g_error ("Could not create 'queue' element");

    rtph264depay = gst_element_factory_make ("rtph264depay", "rtph264_depay");
    if (rtph264depay == NULL)
        g_error ("Could not create 'rtph264depay' element");

    h264parse = gst_element_factory_make ("h264parse", "h264_parse");
    if (h264parse == NULL)
        g_error ("Could not create 'h264parse' element");

    nvh264dec = gst_element_factory_make ("nvh264dec", "nvh264_dec");
    if (nvh264dec == NULL)
        g_error ("Could not create 'nvh264dec' element");

    videoconvert = gst_element_factory_make ("videoconvert", "video_convert");
    if (videoconvert == NULL)
        g_error ("Could not create 'videoconvert' element");

    autovideosink = gst_element_factory_make ("autovideosink", "sink");
    if (autovideosink == NULL) 
        g_error ("Could not create neither 'xvimagesink' nor 'ximagesink' element");


    gst_bin_add_many (GST_BIN (pipeline), udpsrc, capsfilter, queue, rtph264depay,myfilter, h264parse,nvh264dec,videoconvert,autovideosink, NULL);

    gst_element_link_many (udpsrc, capsfilter, queue, rtph264depay, h264parse,myfilter, nvh264dec,videoconvert,autovideosink, NULL);
    filtercaps = gst_caps_new_simple ("application/x-rtp",
                "media", G_TYPE_STRING, "video",
                "clock-rate", G_TYPE_INT, 90000,
                "encoding-name", G_TYPE_STRING, "H264",
                "payload", G_TYPE_INT, 96,
                NULL);
    g_object_set (G_OBJECT (capsfilter), "caps", filtercaps, NULL);

    
    
    gst_caps_unref (filtercaps);
    
    g_object_set (G_OBJECT (udpsrc), "uri", "udp://224.1.1.3:5000", NULL);
    g_object_set (G_OBJECT (queue), "max-size-buffers", 1, NULL);
    g_object_set (G_OBJECT (autovideosink), "sync", FALSE, NULL);
    // pad = gst_element_get_static_pad (udpsrc, "src");
    // gst_pad_add_probe (pad, GST_PAD_PROBE_TYPE_BUFFER,
    //     (GstPadProbeCallback) cb_have_data, NULL, NULL);
    // gst_object_unref (pad);

    pad = gst_element_get_static_pad (rtph264depay, "src");
    gst_pad_add_probe (pad, GST_PAD_PROBE_TYPE_BUFFER,
        (GstPadProbeCallback) cb_have_data, (gpointer)rtph264depay, NULL);
    gst_object_unref (pad);

    /* run */
    gst_element_set_state (pipeline, GST_STATE_PLAYING);

    /* wait until it's up and running or failed */
    if (gst_element_get_state (pipeline, NULL, NULL, -1) == GST_STATE_CHANGE_FAILURE) {
        g_error ("Failed to go into PLAYING state");
    }

    g_print ("Running ...\n");
    g_main_loop_run (loop);

    /* exit */
    gst_element_set_state (pipeline, GST_STATE_NULL);
    gst_object_unref (pipeline);

    return 0;
}


