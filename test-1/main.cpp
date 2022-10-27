#include <gst/gst.h>
#include <iostream>
#include <stdio.h>
// file:/home/meysam/Desktop/lesson1.mp4



static gboolean print_field(GQuark field, const GValue *value, gpointer pfx)
{
    gchar *str = gst_value_serialize(value);
    g_print("%s  %15s: %s\n", (gchar *) pfx, g_quark_to_string (field), str);
    g_free (str);
    return TRUE;
}

static void print_caps (const GstCaps * caps, const gchar * pfx) 
{
    guint i;

    g_return_if_fail (caps != NULL);

    if (gst_caps_is_any (caps)) 
    {
        g_print ("%sANY\n", pfx);
        return;
    }
    if (gst_caps_is_empty (caps)) 
    {
        g_print ("%sEMPTY\n", pfx);
        return;
    }

    for (i = 0; i < gst_caps_get_size (caps); i++) 
    {
        GstStructure *structure = gst_caps_get_structure (caps, i);

        g_print ("%s%s\n", pfx, gst_structure_get_name (structure));
        gst_structure_foreach (structure, print_field, (gpointer) pfx);
    }
}



static void print_pad_templates_information (GstElementFactory * factory) 
{
    const GList *pads;
    GstStaticPadTemplate *padtemplate;

    g_print ("Pad Templates for %s:\n", gst_element_factory_get_longname (factory));
    if (!gst_element_factory_get_num_pad_templates (factory)) 
    {
        g_print ("  none\n");
        return;
    }

    pads = gst_element_factory_get_static_pad_templates (factory);
    while (pads) 
    {
        padtemplate = (GstStaticPadTemplate*) pads->data;
        pads = g_list_next (pads);

        if (padtemplate->direction == GST_PAD_SRC)
            g_print ("  SRC template: '%s'\n", padtemplate->name_template);
        else if (padtemplate->direction == GST_PAD_SINK)
            g_print ("  SINK template: '%s'\n", padtemplate->name_template);
        else
            g_print ("  UNKNOWN!!! template: '%s'\n", padtemplate->name_template);

        if (padtemplate->presence == GST_PAD_ALWAYS)
            g_print ("    Availability: Always\n");
        else if (padtemplate->presence == GST_PAD_SOMETIMES)
            g_print ("    Availability: Sometimes\n");
        else if (padtemplate->presence == GST_PAD_REQUEST)
            g_print ("    Availability: On request\n");
        else
            g_print ("    Availability: UNKNOWN!!!\n");

        if (padtemplate->static_caps.string) 
        {
            GstCaps *caps;
            g_print ("    Capabilities:\n");
            caps = gst_static_caps_get (&padtemplate->static_caps);
            print_caps (caps, "      ");
            gst_caps_unref (caps);

        }

        g_print ("\n");
    }
}






int main (int argc, char *argv[])
{
    GstElement *src, *sink,*convert,*pipeline;
    GstElementFactory *convert_factory;
    GstBus *bus;
    GstMessage *msg;
    GstStateChangeReturn ret;
    gboolean terminate = FALSE;

    gst_init(&argc, &argv);

    src = gst_element_factory_make("filesrc", "source");
    // convert = gst_element_factory_make("decodebin", "dec");
    convert_factory = gst_element_factory_find("decodebin");
    convert = gst_element_factory_create(convert_factory,"dec");

    sink = gst_element_factory_make("autovideosink", "sink");

    pipeline = gst_pipeline_new("test-pipeline");

    if (!pipeline || !src || !convert || !sink) 
    {
        g_printerr ("Not all elements could be created.\n");
        return -1;
    }

    GstPad *pad = NULL;
    GstCaps *caps = NULL;

    const GList * pads ;
    // print_pad_templates_information(convert_factory);

    pads = gst_element_factory_get_static_pad_templates (convert_factory);

    GstPadTemplate * padtemplate;
    pads = g_list_next (pads); // TODO: do it with safer way!

    padtemplate =(GstPadTemplate *) pads->data;

    
    pad = gst_element_request_pad ( convert,
                                    padtemplate,
                         NULL,
                         NULL);

    std::cout<< padtemplate<<std::endl;

    // Build the pipeline 
    gst_bin_add_many(GST_BIN(pipeline), src, convert ,sink, NULL);

    if(!gst_element_link_many(src,convert, sink,NULL))
    {
        g_printerr("Element could not be linked.\n");
        gst_object_unref(pipeline);
        return -1;
    }

    // Set the URI to play
    // g_object_set (data.source, "uri", "https://www.freedesktop.org/software/gstreamer-sdk/data/media/sintel_trailer-480p.webm", NULL);
    g_object_set (src, "location", "/home/meysam/Desktop/lesson1.mp4", NULL);


    ret = gst_element_set_state(pipeline, GST_STATE_PLAYING);

    if(ret == GST_STATE_CHANGE_FAILURE)
    {
        g_printerr("Unable to set the pipeline to the playing state.\n");
        gst_object_unref(pipeline);
        return -1;
    }

    // wait until error or EOS
    bus = gst_element_get_bus(pipeline);
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
    gst_element_set_state (pipeline, GST_STATE_NULL);
    gst_object_unref (pipeline);

    return 0;
}



