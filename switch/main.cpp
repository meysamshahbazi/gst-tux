#include <gst/gst.h>

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

    
    /* Free resources */
    gst_message_unref (msg);
    gst_object_unref (bus);
    gst_element_set_state (pipeline, GST_STATE_NULL);
    gst_object_unref (pipeline);
    return 0;
}



