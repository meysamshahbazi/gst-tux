#include <gst/gst.h>
#include <iostream>
#include <stdio.h>
using namespace std;


int main(int argc, char** argv)
{
    GstElement *pipline; 
    GstElement *source0, *vid_conv0, *queue0;
    GstElement *source1, *vid_conv1, *queue1;

    GstElement *compositor,*sink_conv,*queues,*sink;
    GstBus *bus;
    GstMessage *msg;
    GstStateChangeReturn ret;
    GstElementFactory * comp_factory;
    // intilialize Gst 
    gst_init(&argc, &argv);

    // create elements 
    source0 = gst_element_factory_make("nvv4l2camerasrc", "source0");
    g_object_set(source0, "device", "/dev/video0", NULL);
    // g_object_set(source0, "num-buffers", 200, NULL);
    // g_object_set(source0,"cap-buffers",16,NULL);

    source1 = gst_element_factory_make("nvv4l2camerasrc", "source1");
    g_object_set(source1, "device", "/dev/video1", NULL);
    // g_object_set(source1, "num-buffers", 200, NULL);
    // g_object_set(source1,"cap-buffers",16,NULL);

    vid_conv0 = gst_element_factory_make("nvvidconv", "vid_conv0");
    vid_conv1 = gst_element_factory_make("nvvidconv", "vid_conv1");
 
    // g_object_set(vid_conv0,"output-buffers",100,NULL);
    // g_object_set(vid_conv1,"output-buffers",100,NULL);
    sink_conv = gst_element_factory_make("nvvidconv", "sink_conv");

    // g_object_set(sink_conv,"output-buffers",100,NULL);


    sink = gst_element_factory_make("nvoverlaysink", "sink");
    g_object_set(sink, "sync", FALSE, NULL);
    // sync=true enable-last-sample=false

    queue0 = gst_element_factory_make("queue","queue0");
    queue1 = gst_element_factory_make("queue","queue1");
    queues = gst_element_factory_make("queue","queues");

    g_object_set(queue0, "max-size-time",0, NULL);
    g_object_set(queue1, "max-size-time",0, NULL);
    g_object_set(queues, "max-size-time",0, NULL);


    comp_factory = gst_element_factory_find("nvcompositor");
    compositor = gst_element_factory_create(comp_factory,"compositor");

  


    // const GList *pads;
    GstPadTemplate *padtemplate;
    GstPad *compositor_pad0 = NULL;
    GstPad *compositor_pad1 = NULL;
    GstCaps *caps = NULL;

    padtemplate = gst_element_get_pad_template(compositor,"sink_%u");
    compositor_pad0 = gst_element_request_pad ( compositor,padtemplate, "sink_0", NULL);
    compositor_pad1 = gst_element_request_pad ( compositor,padtemplate, "sink_1", NULL);


    // g_object_set(compositor_pad0,"alpha",1.0f,"xpos",0,"ypos",0,"width",960,"height",1080,NULL);
    // g_object_set(compositor_pad1,"alpha",1.0f,"xpos",960,"ypos",0,"width",960,"height",1080,NULL);

    g_object_set(compositor_pad0,"xpos",0,"ypos",0,"width",960,"height",1080,NULL);
    g_object_set(compositor_pad1,"xpos",960,"ypos",0,"width",960,"height",1080,NULL);

    // g_object_set(compositor_pad0, "interpolation-method", 5,NULL);
    // g_object_set(compositor_pad1, "interpolation-method", 5,NULL);

    // g_object_set(vid_conv0, "interpolation-method", 5,NULL);
    // g_object_set(vid_conv1, "interpolation-method", 5,NULL);
    // g_object_set(sink_conv, "interpolation-method", 5,NULL);

    // create the new empty pipeline 
    pipline = gst_pipeline_new("test-pipline");

    // build the pipe 
    gst_bin_add_many(GST_BIN (pipline),
                    source0,vid_conv0,queue0,
                    source1,vid_conv1,queue1,
                    compositor,sink_conv,sink,queues,NULL);


    GstCaps *caps_src0;// UYVY ,framerate=60/1 UYVY
    caps_src0 = gst_caps_from_string("video/x-raw(memory:NVMM),width=1920,height=1080, format=UYVY,framerate=30/1");
    if (!gst_element_link_filtered(source0, vid_conv0, caps_src0)) {
        g_printerr("Fail to gst_element_link_filtered videosrc -- vidconv\n");
        return -1;
    }
    gst_caps_unref(caps_src0);

    GstCaps *caps_src1;
    caps_src1 = gst_caps_from_string("video/x-raw(memory:NVMM),width=1920,height=1080, format=UYVY,framerate=30/1");
    if (!gst_element_link_filtered(source1, vid_conv1, caps_src1)) {
        g_printerr("Fail to gst_element_link_filtered videosrc -- vidconv\n");
        return -1;
    }
    gst_caps_unref(caps_src1);



    GstCaps *caps_conv0;
    caps_conv0 = gst_caps_from_string("video/x-raw(memory:NVMM), width=1920, height=1080, format=RGBA");

    if (!gst_element_link_filtered(vid_conv0, queue0, caps_conv0)) {
        g_printerr("Fail to gst_element_link_filtered vidconv -- videoenc\n");
        return -1;
    }
    gst_caps_unref(caps_conv0);

    GstCaps *caps_conv1;
    caps_conv1 = gst_caps_from_string("video/x-raw(memory:NVMM), width=1920, height=1080, format=RGBA");

    if (!gst_element_link_filtered(vid_conv1, queue1, caps_conv1)) {
        g_printerr("Fail to gst_element_link_filtered vidconv -- videoenc\n");
        return -1;
    }
    gst_caps_unref(caps_conv1);


    GstCaps *sink_conv_caps;
    sink_conv_caps = gst_caps_from_string("video/x-raw(memory:NVMM), width=1920, height=1080,format=NV12");
    if (!gst_element_link_filtered(sink_conv, sink, sink_conv_caps)) {
        g_printerr("Fail to gst_element_link_filtered vidconv -- videoenc\n");
        return -1;
    }
    gst_caps_unref(sink_conv_caps);

    if (gst_element_link_many(compositor,queues,sink_conv,NULL) != TRUE)
    {
        g_printerr("Elements could not be linked.\n");
        gst_object_unref(pipline);
        return -1;
    }


    GstPad *src_pad0,*src_pad1;
    src_pad0 = gst_element_get_static_pad(queue0, "src");// TODO: use vidconv
    src_pad1 = gst_element_get_static_pad(queue1, "src");

    if( gst_pad_link(src_pad0, compositor_pad0) != GST_PAD_LINK_OK ||
        gst_pad_link(src_pad1, compositor_pad1) != GST_PAD_LINK_OK )
    {
        gst_printerr("Tee could not be linked");
        gst_object_unref(pipline);
        return -1;
    }

    gst_object_unref(src_pad0);
    gst_object_unref(src_pad1);

    // start playing
    ret = gst_element_set_state(pipline,GST_STATE_PLAYING);

    if(ret == GST_STATE_CHANGE_FAILURE)
    {
        g_printerr("Unable to set the pipeline to the playing state.\n");
        gst_object_unref(pipline);
        return -1;
    }


    cout<<"insert mode(a,b,c): ";
    char inp;


    bus = gst_element_get_bus(pipline);
    msg =
        gst_bus_timed_pop_filtered (bus, GST_CLOCK_TIME_NONE,
        (GstMessageType) (GST_MESSAGE_ERROR | GST_MESSAGE_EOS) );
    
    /* See next tutorial for proper error message handling/parsing */
    if (GST_MESSAGE_TYPE (msg) == GST_MESSAGE_ERROR) 
    {
        g_error ("An error occurred! Re-run with the GST_DEBUG=*:WARN environment "
            "variable set for more details.");
    }

    
    // while (true)
    // {
    //     // wait until error or EOS
    //     bus = gst_element_get_bus(pipline);
    //     msg = gst_bus_timed_pop_filtered(bus,10*GST_MSECOND,
    //                     (GstMessageType) (GST_MESSAGE_ERROR | GST_MESSAGE_EOS | GST_MESSAGE_DURATION) );

        
    //     /* Parse message */
    //     if (msg != NULL) 
    //     {
    //         GError *err;
    //         gchar *debug_info;

    //         switch (GST_MESSAGE_TYPE (msg)) 
    //         {
    //             case GST_MESSAGE_ERROR:
    //             gst_message_parse_error (msg, &err, &debug_info);
    //             g_printerr ("Error received from element %s: %s\n",
    //                 GST_OBJECT_NAME (msg->src), err->message);
    //             g_printerr ("Debugging information: %s\n",
    //                 debug_info ? debug_info : "none");
    //             g_clear_error (&err);
    //             g_free (debug_info);
    //             break;
    //             case GST_MESSAGE_EOS:
    //             g_print ("End-Of-Stream reached.\n");
    //             break;
    //             default:
    //             /* We should not reach here because we only asked for ERRORs and EOS */
    //             g_printerr ("Unexpected message received.\n");
    //             break;
    //         }
    //         gst_message_unref (msg);
    //     }
    //     cin>>inp;
    //     if(inp =='a')
    //     {
    //         g_object_set(compositor_pad0,"alpha",1.0f,"xpos",0,"ypos",0,"width",1920,"height",1080,NULL);
    //         g_object_set(compositor_pad1,"alpha",0.0f,"xpos",0,"ypos",0,"width",1920,"height",1080,NULL);
    //         cout<<"insert mode(a,b): ";
    //     }
    //     if(inp=='b')
    //     {
    //         g_object_set(compositor_pad0,"alpha",0.0f,"xpos",0,"ypos",0,"width",1920,"height",1080,NULL);
    //         g_object_set(compositor_pad1,"alpha",1.0f,"xpos",0,"ypos",0,"width",1920,"height",1080,NULL);
    //         cout<<"insert mode(a,b): ";
    //     }
    //     if(inp == 'c')
    //     {
    //         g_object_set(compositor_pad0,"alpha",1.0f,"xpos",0,"ypos",0,"width",960,"height",1080,NULL);
    //         g_object_set(compositor_pad1,"alpha",1.0f,"xpos",960,"ypos",0,"width",960,"height",1080,NULL);
    //         cout<<"insert mode(a,b): ";
    //     }
    // }

    /* Free resources */
    gst_object_unref (bus);
    gst_element_set_state (pipline, GST_STATE_NULL);
    gst_object_unref (pipline);
    return 0;
}

