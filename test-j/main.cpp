#include <gst/gst.h>
#include <iostream>
#include <stdio.h>
using namespace std;

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



int main(int argc, char** argv)
{
    GstElement *pipline, *source1, *source2, *compositor,*sink,*out_switch,*caps_filter1,*caps_filter2;
    GstBus *bus;
    GstMessage *msg;
    GstStateChangeReturn ret;
    GstElementFactory * comp_factory;
    // intilialize Gst 
    gst_init(&argc, &argv);

    // create elements 
    source1 = gst_element_factory_make("videotestsrc", "source1");
    source2 = gst_element_factory_make("videotestsrc", "source2");
    GstPad *temp_pad;
    temp_pad = gst_element_get_static_pad(source2, "src");

    // g_object_set(temp_pad,"format","I420","width",1920,"height",1080,NULL);


    // source2 = gst_element_factory_make("nvarguscamerasrc", "source2");
    caps_filter2 = gst_element_factory_make("capsfilter", "capsfilter2");
    caps_filter1 = gst_element_factory_make("capsfilter", "capsfilter1");

    g_object_set(source1, "pattern", 18, NULL);
    
    // g_object_set(source2, "pattern", 0, NULL);
    // g_object_set(source1,"uri","/home/meysam/Desktop/lesson1.mp4", NULL);
    // g_object_set(source2,"uri","/home/meysam/Desktop/lesson2.mp4", NULL);
    
    // sink = gst_element_factory_make("autovideosink", "sink");
    sink = gst_element_factory_make("nvoverlaysink", "sink");
    
    comp_factory = gst_element_factory_find("compositor");
    compositor = gst_element_factory_create(comp_factory,"compositor");

  


    // const GList *pads;
    GstPadTemplate *padtemplate;
    GstPad *pad1 = NULL;
    GstPad *pad2 = NULL;
    GstCaps *caps = NULL;

    padtemplate = gst_element_get_pad_template(compositor,"sink_%u");
    pad1 = gst_element_request_pad ( compositor,padtemplate, "sink_0", NULL);
    pad2 = gst_element_request_pad ( compositor,padtemplate, "sink_1", NULL);


    g_object_set(pad1,"alpha",1.0f,"xpos",0,"ypos",0,"width",960,"height",1080,NULL);
    g_object_set(pad2,"alpha",1.0f,"xpos",960,"ypos",0,"width",960,"height",1080,NULL);

   


    // return -1;
    // create the new empty pipeline 
    pipline = gst_pipeline_new("test-pipline");

    // build the pipe 
    gst_bin_add_many(GST_BIN (pipline),source1,source2,caps_filter1,caps_filter2,compositor,sink,NULL);

    if (gst_element_link(compositor,sink) != TRUE)
    {
        g_printerr("Elements could not be linked.\n");
        gst_object_unref(pipline);
        return -1;
    }

    if (gst_element_link(source2,caps_filter2) != TRUE)
    {
        g_printerr("caps_filter could not be linked.\n");
        gst_object_unref(pipline);
        return -1;
    }
    if (gst_element_link(source1,caps_filter1) != TRUE)
    {
        g_printerr("caps_filter could not be linked.\n");
        gst_object_unref(pipline);
        return -1;
    }

    GstPad *src_pad1,*src_pad2;
    // src_pad1 = gst_element_get_static_pad(source1, "src");

    

    // gst_element_link_filtered (element1, element2, caps);



    src_pad2 = gst_element_get_static_pad(caps_filter2, "src");
    src_pad1 = gst_element_get_static_pad(caps_filter1, "src");

    cout<<src_pad1<<endl;
    cout<<pad1<<endl;
    cout<<src_pad2<<endl;
    cout<<pad2<<endl;

    if( gst_pad_link(src_pad1, pad1) != GST_PAD_LINK_OK   ||
        gst_pad_link(src_pad2, pad2) != GST_PAD_LINK_OK  )
    {
        gst_printerr("Tee could not be linked\n");
        gst_object_unref(pipline);
        return -1;
    }

    gst_object_unref(src_pad1);
    gst_object_unref(src_pad2);

    GstCaps *capss;
    //video/x-raw(memory:NVMM),width=(int)1920, height=(int)1080, format=(string)NV12,framerate=(fraction)30/1
    capss = gst_caps_new_simple ("video/x-raw(memory:NVMM)",
          "format", G_TYPE_STRING, "NV12",
          "width", G_TYPE_INT, 1920,
          "height", G_TYPE_INT, 1080,
          "framerate", GST_TYPE_FRACTION, 30, 1,
          NULL);

    g_object_set(caps_filter2,"caps",capss,NULL);
    g_object_set(caps_filter1,"caps",capss,NULL);
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
        cin>>inp;
        if(inp =='a')
        {
            g_object_set(pad1,"alpha",1.0f,"xpos",0,"ypos",0,"width",1920,"height",1080,NULL);
            g_object_set(pad2,"alpha",0.0f,"xpos",0,"ypos",0,"width",1920,"height",1080,NULL);
            cout<<"insert mode(a,b): ";
        }
        if(inp=='b')
        {
            g_object_set(pad1,"alpha",0.0f,"xpos",0,"ypos",0,"width",1920,"height",1080,NULL);
            g_object_set(pad2,"alpha",1.0f,"xpos",0,"ypos",0,"width",1920,"height",1080,NULL);
            cout<<"insert mode(a,b): ";
        }
        if(inp == 'c')
        {
            g_object_set(pad1,"alpha",1.0f,"xpos",0,"ypos",0,"width",960,"height",1080,NULL);
            g_object_set(pad2,"alpha",1.0f,"xpos",960,"ypos",0,"width",960,"height",1080,NULL);
            cout<<"insert mode(a,b): ";
        }
    }

    /* Free resources */
    gst_object_unref (bus);
    gst_element_set_state (pipline, GST_STATE_NULL);
    gst_object_unref (pipline);
    return 0;
}

