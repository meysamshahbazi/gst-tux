#include <gst/gst.h>
#include <iostream>
using namespace std;

int
main (int argc, char *argv[])
{
    GstElement *pipeline,*source1, *source2,*sink,*out_switch;;
    GstBus *bus;
    GstMessage *msg;
    GstStateChangeReturn ret;
    GstElementFactory * comp_factory;

    /* Initialize GStreamer */
    gst_init (&argc, &argv);

        // create elements 
    source1 = gst_element_factory_make("videotestsrc", "source1");
    source2 = gst_element_factory_make("videotestsrc", "source2");
    out_switch = gst_element_factory_make("input-selector", "out_switch");
    g_object_set(source1, "pattern", 14, NULL);
    g_object_set(source2, "pattern", 13, NULL);

   sink = gst_element_factory_make("autovideosink", "sink");

    GstPadTemplate *padtemplate;
    GstPad *pad1 = NULL;
    GstPad *pad2 = NULL;
    // GstCaps *caps = NULL;

    padtemplate = gst_element_get_pad_template(out_switch,"sink_%u");


    pad1 = gst_element_request_pad(out_switch, padtemplate, "sink_0", NULL);
    pad2 = gst_element_request_pad(out_switch, padtemplate, "sink_1", NULL);


    

    pipeline = gst_pipeline_new("test-pipline");
    // build the pipe 
    gst_bin_add_many(GST_BIN (pipeline),source1,source2,out_switch,sink,NULL);

    if (gst_element_link(out_switch,sink) != TRUE)
    {
        g_printerr("Elements could not be linked.\n");
        gst_object_unref(pipeline);
        return -1;
    }

    GstPad *src_pad1,*src_pad2;
    src_pad1 = gst_element_get_static_pad(source1, "src");
    src_pad2 = gst_element_get_static_pad(source2, "src");

    if( gst_pad_link(src_pad1, pad1) != GST_PAD_LINK_OK ||
        gst_pad_link(src_pad2, pad2) != GST_PAD_LINK_OK )
    {
        gst_printerr("Tee could not be linked");
        gst_object_unref(pipeline);
        return -1;
    }

    gst_object_unref(src_pad1);
    gst_object_unref(src_pad2);

    // start playing

    ret = gst_element_set_state(pipeline,GST_STATE_PLAYING);

    if(ret == GST_STATE_CHANGE_FAILURE)
    {
        g_printerr("Unable to set the pipeline to the playing state.\n");
        gst_object_unref(pipeline);
        return -1;
    }

    // wait until error or EOS
    bus = gst_element_get_bus(pipeline);
    cout<<"insert mode(a,b): ";
    char inp;

    while (true)
    {
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
        
        cin>>inp;
        if(inp =='a')
        {
            cout<<"A\n";
            g_object_set(out_switch,"active-pad",pad1,NULL);
            cout<<"insert mode(a,b): ";

        }
        if(inp=='b')
        {
            cout<<"B\n";
            g_object_set(out_switch,"active-pad",pad2,NULL);
            cout<<"insert mode(a,b): ";
        }



    }
    
   

    





    /* Free resources */
    gst_message_unref (msg);
    gst_object_unref (bus);
    gst_element_set_state (pipeline, GST_STATE_NULL);
    gst_object_unref (pipeline);
    return 0;
}



