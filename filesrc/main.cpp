#include <gst/gst.h>
#include <iostream>

typedef struct _CustomData
{
    GstElement *pipeline;
    GstElement *src;
    GstElement *dec;
    GstElement *sink;

} CustomData;



static void pad_added_handler(GstElement *src,GstPad *pad, CustomData *data);

// file:/home/meysam/Desktop/car1_s.mp4"
int main (int argc, char *argv[])
{
    CustomData data;
    GstBus *bus;
    GstMessage *msg;
    GstStateChangeReturn ret;

    // gst-launch-1.0 filesrc location=/home/meysam/Desktop/car1_s.mp4 ! decodebin ! autovideosink

    gst_init (&argc, &argv);
    
    data.src = gst_element_factory_make("filesrc", "source");
    data.dec = gst_element_factory_make("decodebin","dec");
    data.sink = gst_element_factory_make("autovideosink", "sink");

    // create the new empty pipeline 
    data.pipeline = gst_pipeline_new("test-pipline");

    // build the pipe 
    gst_bin_add_many(GST_BIN (data.pipeline),data.src,data.dec, data.sink,NULL);
    if (gst_element_link_many(data.src,data.dec,NULL) != TRUE)
    {
        g_printerr("Elements could not be linked.\n");
        gst_object_unref(data.pipeline);
        return -1;
    }

    g_object_set(data.src, "location", "/home/meysam/Desktop/car1_s.mp4", NULL);

    g_signal_connect(data.dec, "pad-added", G_CALLBACK(pad_added_handler),&data);

    ret = gst_element_set_state(data.pipeline,GST_STATE_PLAYING);

    if(ret == GST_STATE_CHANGE_FAILURE)
    {
        g_printerr("Unable to set the pipeline to the playing state.\n");
        gst_object_unref(data.pipeline);
        return -1;
    }

    // wait until error or EOS
    bus = gst_element_get_bus(data.pipeline);

    while (true)
    {
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

    }

    /* Free resources */
    gst_object_unref (bus);
    gst_element_set_state (data.pipeline, GST_STATE_NULL);
    gst_object_unref (data.pipeline);


    std::cout<<"im here\n";

    return 0;
}

static void pad_added_handler(GstElement *src, GstPad *new_pad, CustomData *data)
{
    GstPad *sink_pad = gst_element_get_static_pad(data->sink,"sink");
    GstPadLinkReturn ret;
    GstCaps *new_pad_caps = NULL;
    GstStructure *new_pad_struct = NULL;
    const gchar *new_pad_type = NULL;

    g_print("Recived new pad '%s' from '%s':\n",GST_PAD_NAME(new_pad),GST_ELEMENT_NAME(src));

    if(gst_pad_is_linked(sink_pad))
    {
        g_print ("It has type '%s' which is not raw audio. Ignoring.\n", new_pad_type);
        goto exit;
    }

    new_pad_caps = gst_pad_get_current_caps(new_pad);
    new_pad_struct = gst_caps_get_structure(new_pad_caps,0);
    new_pad_type = gst_structure_get_name(new_pad_struct);

    if( !g_str_has_prefix(new_pad_type, "video/x-raw"))
    {
        g_print ("It has type '%s' which is not raw audio. Ignoring.\n", new_pad_type);
        goto exit;
    }

    ret = gst_pad_link(new_pad,sink_pad);

    if(GST_PAD_LINK_FAILED(ret))
        g_print ("Type is '%s' but link failed.\n", new_pad_type);
    else
        g_print ("Link succeeded (type '%s').\n", new_pad_type);

exit:
    if(new_pad_caps != NULL)
        gst_caps_unref(new_pad_caps);

    gst_object_unref(sink_pad);    
}

