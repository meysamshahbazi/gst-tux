#include <gst/gst.h>
#include <iostream>

// file:/home/meysam/Desktop/car1_s.mp4"
int main (int argc, char *argv[])
{
    GstElement *src, *pipline;
    GstElement *sink, *dec;
    GstElementFactory *srcfactory;


    GstBus *bus;
    GstMessage *msg;
    GstStateChangeReturn ret;


    gst_init (&argc, &argv);

    // srcfactory = gst_element_factory_find ("filesrc");
    // if(srcfactory == NULL)
    // {
    //     std::cout<<"cout not find srcfactory\n";
    //     return -1;
    // }
    // src = gst_element_factory_create (srcfactory, "src");
    // if(src == NULL)
    // {
    //     std::cout<<"cout not find srcfactory\n";
    //     return -1;
    // }

    src = gst_element_factory_make("uridecodebin", "source");
    // g_return_if_fail (src != NULL);

    // dec = gst_element_factory_make("decodebin", "dec");
    // if(dec == NULL)
    // {
    //     std::cout<<"cout not make decodebin \n";
    //     return -1;
    // }


    sink = gst_element_factory_make("autovideosink", "sink");


    // create the new empty pipeline 
    pipline = gst_pipeline_new("test-pipline");

    // build the pipe 
    gst_bin_add_many(GST_BIN (pipline),src,sink,NULL);
    if (gst_element_link_many(src,sink,NULL) != TRUE)
    {
        g_printerr("Elements could not be linked.\n");
        gst_object_unref(pipline);
        return -1;
    }

    g_object_set(src, "location", "/home/meysam/Desktop/car1_s.mp4", NULL);

    ret = gst_element_set_state(pipline,GST_STATE_PLAYING);

    if(ret == GST_STATE_CHANGE_FAILURE)
    {
        g_printerr("Unable to set the pipeline to the playing state.\n");
        gst_object_unref(pipline);
        return -1;
    }

    // wait until error or EOS
    bus = gst_element_get_bus(pipline);
    msg = gst_bus_timed_pop_filtered(bus,GST_CLOCK_TIME_NONE,
                    (GstMessageType) (GST_MESSAGE_ERROR | GST_MESSAGE_EOS) );

    

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


    /* Free resources */
    gst_object_unref (bus);
    gst_element_set_state (pipline, GST_STATE_NULL);
    gst_object_unref (pipline);


    std::cout<<"im here\n";

    return 0;
}



