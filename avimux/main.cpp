#include <gst/gst.h>

int
main (int argc, char *argv[])
{
    GstElement *pipeline, *source1,*sink;
    GstBus *bus;
    GstMessage *msg;

    /* Initialize GStreamer */
    gst_init (&argc, &argv);

    source1 = gst_element_factory_make("videotestsrc", "source1");
    g_object_set(source1, "pattern", 14, NULL);
    sink = gst_element_factory_make("filesink", "sink");




    /* Build the pipeline */

    /* Start playing */
    gst_element_set_state (pipeline, GST_STATE_PLAYING);

    /* Wait until error or EOS */
    bus = gst_element_get_bus (pipeline);
    msg =
        gst_bus_timed_pop_filtered (bus, GST_CLOCK_TIME_NONE,
        (GstMessageType) (GST_MESSAGE_ERROR | GST_MESSAGE_EOS) );
    
    /* See next tutorial for proper error message handling/parsing */
    if (GST_MESSAGE_TYPE (msg) == GST_MESSAGE_ERROR) 
    {
        g_error ("An error occurred! Re-run with the GST_DEBUG=*:WARN environment "
            "variable set for more details.");
    }

    /* Free resources */
    gst_message_unref (msg);
    gst_object_unref (bus);
    gst_element_set_state (pipeline, GST_STATE_NULL);
    gst_object_unref (pipeline);
    return 0;
}



