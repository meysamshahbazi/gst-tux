#include <gst/gst.h>
#include <iostream>
#include <vector>

#include <stdio.h>
#include <strings.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include<netinet/in.h>
#define PORT 5002
#define MAXLINE 1000


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

    /* Create a new empty buffer */
    buffer = gst_buffer_new_and_alloc (data->size);

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
    g_signal_emit_by_name (sink, "pull-sample", &sample);
    if (sample) {
        GstBuffer* rec_buff = gst_sample_get_buffer(sample);
        gst_buffer_map (rec_buff, &map, GST_MAP_READ);
        // GstClockTime pts = GST_BUFFER_PTS(rec_buff);

        data->size = map.size;
        guint8 *raw = (guint8 *)map.data;
       
        for (int i = 0; i < map.size ; i++) {
            data->ptr[i] = raw[i];
        }

        int header = 0;

        int nb_nal = 0;
        for (int i =0; i <data->size-3; i++) {   
            if (data->ptr[i]   == 0x00 &&
                data->ptr[i+1] == 0x00 &&
                data->ptr[i+2] == 0x00 &&
                data->ptr[i+3] == 0x01 )  {
                    nb_nal++; 
                    if (i+4 < data->size-3)
                        header = raw[i+4];
                }
        }

        // if (header == 6) {
        //     std::cout << data->size << "\t";
        //      for (int i =0; i <data->size; i++) {       
        //         std::cout << int(data->ptr[i]) << " ,";
        //     }
        //     std::cout << std::endl;
        // }

        gst_buffer_unmap (rec_buff, &map);
        if (header != 6)
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

    unsigned char buffer[100];
    int listenfd;
    socklen_t len;
    struct sockaddr_in servaddr, cliaddr;
    bzero(&servaddr, sizeof(servaddr));

    // Create a UDP Socket
    listenfd = socket(AF_INET, SOCK_DGRAM, 0);        
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);
    servaddr.sin_family = AF_INET; 
 
    // bind server address to socket descriptor
    bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
     
    


    GMainLoop *loop;
    GstElement*udpsrc, *capsfilter, *queue, 
    // *appsink, *appsrc,
    *rtph264depay,*h264parse,
    *nvh264dec,*videoconvert,*autovideosink;

    GstElement *pipeline1;
    GstElement *pipeline2;
    

    CustomData data;
    data.sourceid = -1;
    GstCaps *x_rtp_caps, *x_h264_caps;
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


    gst_bin_add_many (GST_BIN (pipeline1), udpsrc,
        capsfilter,
        rtph264depay,
        data.appsink, NULL);

    gst_element_link_many(udpsrc, 
        capsfilter,
        rtph264depay, 
        data.appsink, NULL);



    gst_bin_add_many (GST_BIN (pipeline2), data.appsrc, h264parse,
    nvh264dec,videoconvert,autovideosink, NULL);

    gst_element_link_many(data.appsrc,
        h264parse, 
        nvh264dec,videoconvert,autovideosink, NULL);



    x_rtp_caps = gst_caps_new_simple ("application/x-rtp",
                "media", G_TYPE_STRING, "video",
                "clock-rate", G_TYPE_INT, 90000,
                "encoding-name", G_TYPE_STRING, "H264",
                "payload", G_TYPE_INT, 96,
                NULL);
    
    g_object_set (G_OBJECT (capsfilter), "caps", x_rtp_caps, NULL);

    gst_caps_unref(x_rtp_caps);
    
    // g_object_set(G_OBJECT (udpsrc), "uri", "udp://224.1.1.3:5000", NULL);
    g_object_set(G_OBJECT (udpsrc), "port", 5000, NULL);
    // g_object_set(G_OBJECT (queue), "max-size-buffers", 1, NULL);
    g_object_set(G_OBJECT (autovideosink), "sync", FALSE, NULL);

    x_h264_caps = gst_caps_new_simple("video/x-h264",
                    "stream-format", G_TYPE_STRING, "byte-stream",
                    "alignment", G_TYPE_STRING, "nal",
                    NULL);

    g_object_set (data.appsrc,
         "caps", x_h264_caps, 
          "format", GST_FORMAT_TIME, NULL);
    g_signal_connect (data.appsrc, "need-data", G_CALLBACK (start_feed), &data);
    g_signal_connect (data.appsrc, "enough-data", G_CALLBACK (stop_feed), &data);

    
    /* Configure appsrc */
    g_object_set (data.appsink, "emit-signals", TRUE,
         "caps", x_h264_caps, 
         NULL);

    g_signal_connect (data.appsink, "new-sample", G_CALLBACK (new_sample), &data);

    gst_caps_unref(x_h264_caps);

    /* run */
    gst_element_set_state (pipeline2, GST_STATE_PLAYING);
    gst_element_set_state (pipeline1, GST_STATE_PLAYING);

    /* wait until it's up and running or failed */
    if (gst_element_get_state (pipeline1, NULL, NULL, -1) == GST_STATE_CHANGE_FAILURE) {
        g_error ("Failed to go into PLAYING state");
    }
    if (gst_element_get_state (pipeline2, NULL, NULL, -1) == GST_STATE_CHANGE_FAILURE) {
        g_error ("Failed to go into PLAYING state");
    }
    
    g_print ("Running ...\n");
    // g_main_loop_run (loop);

    uint32_t frame_cnt;

    uint16_t x_car, y_car, w_car, h_car;
    uint16_t x_plt, y_plt, w_plt, h_plt;
    std::vector<char> plate_digit;

    while (1) {
        plate_digit.clear();
        //receive the datagram
        len = sizeof(cliaddr);
        
        int n = recvfrom(listenfd, buffer, 28,
                0, (struct sockaddr*)&cliaddr,&len); //receive message from server

        frame_cnt = (buffer[0]<<24) + (buffer[1]<<16) + (buffer[2]<<8) + buffer[3];
        x_car = (buffer[4]<<8) + buffer[5];
        y_car = (buffer[6]<<8) + buffer[7];
        w_car = (buffer[8]<<8) + buffer[9];
        h_car = (buffer[10]<<8) + buffer[11];

        x_plt = (buffer[12]<<8) + buffer[13];
        y_plt = (buffer[14]<<8) + buffer[15];
        w_plt = (buffer[16]<<8) + buffer[17];
        h_plt = (buffer[18]<<8) + buffer[19];

        // buffer[n] = '\0';
        std::cout << "UDP: ";
        for (int i = 20; i < 28; i++){
            plate_digit.push_back(char(buffer[i]));
            std::cout << char(buffer[i]) << ", ";
        }
        std::cout << std::endl;
        
    }
    

    /* exit */
    gst_element_set_state (pipeline1, GST_STATE_NULL);
    gst_object_unref (pipeline1);

    gst_element_set_state (pipeline2, GST_STATE_NULL);
    gst_object_unref (pipeline2);

    delete data.ptr;

    return 0;
}


