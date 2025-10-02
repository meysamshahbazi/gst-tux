#include <gst/gst.h>
#include <iostream>
#include <stdio.h>


using namespace std;


// gst-launch-1.0 v4l2src device=/dev/video0 ! 'video/x-raw,width=1920,height=1080, format=UYVY,framerate=60/1'
//  ! nvvidconv ! 'video/x-raw(memory:NVMM), width=1920, height=1080, format=NV12' ! nvoverlaysink sync=false


int main(int argc, char** argv)
{
    GstElement *pipline, *source0, *sink0, *nvvidconv0,
    *source1, *sink1, *nvvidconv1;

    GstBus *bus;
    GstMessage *msg;
    GstStateChangeReturn ret;

    // intilialize Gst 
    gst_init(&argc, &argv);

    source0 = gst_element_factory_make("nvv4l2camerasrc", "source0");
    g_object_set(source0, "device", "/dev/video0","cap-buffers",3, NULL);


    nvvidconv0 = gst_element_factory_make("nvvidconv", "nvvidconv0");

    sink0 = gst_element_factory_make("nvoverlaysink", "sink0");
    g_object_set(sink0, "sync", FALSE, NULL);

    source1 = gst_element_factory_make("nvv4l2camerasrc", "source1");
    g_object_set(source1, "device", "/dev/video1","cap-buffers",3, NULL);


    nvvidconv1 = gst_element_factory_make("nvvidconv", "nvvidconv1");

    sink1 = gst_element_factory_make("nvvidconv", "sink1");
    g_object_set(sink1, "sync", FALSE, NULL);

    // g_object_set(sink0, "overlay",1,"overlay-x", 0, "overlay-y", 0, "overlay-w", 960, "overlay-h", 1080, NULL);
    
    pipline = gst_pipeline_new("test-pipline");

    // build the pipe 
    gst_bin_add_many(GST_BIN (pipline),source0,nvvidconv0,sink0,source1,nvvidconv1,sink1,NULL);

    GstCaps *caps_src0;
    caps_src0 = gst_caps_from_string("video/x-raw(memory:NVMM),width=1920,height=1080, format=UYVY,framerate=30/1");
    if (!gst_element_link_filtered(source0, nvvidconv0, caps_src0)) {
        g_printerr("Fail to gst_element_link_filtered videosrc -- vidconv\n");
        return -1;
    }
    gst_caps_unref(caps_src0);

    GstCaps *caps_src1;
    caps_src1 = gst_caps_from_string("video/x-raw(memory:NVMM),width=1920,height=1080, format=UYVY,framerate=30/1");
    if (!gst_element_link_filtered(source1, nvvidconv1, caps_src1)) {
        g_printerr("Fail to gst_element_link_filtered videosrc -- vidconv\n");
        return -1;
    }
    gst_caps_unref(caps_src1);



    GstCaps *caps_conv0;
    caps_conv0 = gst_caps_from_string("video/x-raw(memory:NVMM), width=960, height=1080, format=NV12");
    if (!gst_element_link_filtered(nvvidconv0, sink0, caps_conv0)) {
        g_printerr("Fail to gst_element_link_filtered vidconv -- videoenc\n");
        return -1;
    }
    gst_caps_unref(caps_conv0);

    g_object_set(sink0,"overlay",1, "overlay-x", 0, "overlay-y", 0, "overlay-w", 960, "overlay-h", 1080, NULL);

    GstCaps *caps_conv1;
    caps_conv1 = gst_caps_from_string("video/x-raw(memory:NVMM), width=960, height=1080, format=NV12");
    if (!gst_element_link_filtered(nvvidconv1, sink1, caps_conv1)) {
        g_printerr("Fail to gst_element_link_filtered vidconv -- videoenc\n");
        return -1;
    }
    gst_caps_unref(caps_conv1);

    g_object_set(sink1, "overlay",2,"overlay-x", 960, "overlay-y", 0, "overlay-w", 960, "overlay-h", 1080, NULL);


    ret = gst_element_set_state(pipline,GST_STATE_PLAYING);

    if(ret == GST_STATE_CHANGE_FAILURE)
    {
        g_printerr("Unable to set the pipeline to the playing state.\n");
        gst_object_unref(pipline);
        return -1;
    }

    // cout<<"insert mode(a,b,c): ";
    // char inp;
    while (true)
    {
        // wait until error or EOS
        bus = gst_element_get_bus(pipline);
        msg = gst_bus_timed_pop_filtered(bus,10*GST_MSECOND,
                        (GstMessageType) (GST_MESSAGE_ERROR | GST_MESSAGE_EOS | GST_MESSAGE_DURATION) );

        
        /* Parse message */
        if (msg != NULL) 
        {
            GError *err;
            gchar *debug_info;

            switch (GST_MESSAGE_TYPE (msg)) 
            {
                case GST_MESSAGE_ERROR:
                gst_message_parse_error (msg, &err, &debug_info);
                g_printerr ("Error received from element %s: %s\n",
                    GST_OBJECT_NAME (msg->src), err->message);
                g_printerr ("Debugging information: %s\n",
                    debug_info ? debug_info : "none");
                g_clear_error (&err);
                g_free (debug_info);
                break;
                case GST_MESSAGE_EOS:
                g_print ("End-Of-Stream reached.\n");
                break;
                default:
                /* We should not reach here because we only asked for ERRORs and EOS */
                g_printerr ("Unexpected message received.\n");
                break;
            }
            gst_message_unref (msg);
        }
    }


    /* Free resources */
    gst_object_unref (bus);
    gst_element_set_state (pipline, GST_STATE_NULL);
    gst_object_unref (pipline);
    return 0;

}