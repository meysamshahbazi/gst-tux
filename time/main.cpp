#include <gst/gst.h>

typedef struct _CustomData
{
    GstElement *playbin;
    gboolean playing;
    gboolean terminate;
    gboolean seed_enabled;
    gboolean seed_done;
    gint64 duration;

} CustomData;

static void handle_message(CustomData *data,GstMessage *msg);

int main(int argc, char *argv[])
{
    CustomData data;
    GstBus *bus;
    GstMessage *msg;
    GstStateChangeReturn ret;

    data.playbin = FALSE;
    data.terminate = FALSE;
    data.seed_done = FALSE;
    data.duration = GST_CLOCK_TIME_NONE;

    gst_init(&argc,&argv);

    data.playbin = gst_element_factory_make("playbin","playbin");

    if(!data.playbin)
    {
        g_printerr ("Not all elements could be created.\n");
        return -1;
    }

    g_object_set(data.playbin,"uri","file:/home/meysam/Desktop/car1_s.mp4",NULL);

    ret = gst_element_set_state(data.playbin,GST_STATE_PLAYING);
    if(ret == GST_STATE_CHANGE_FAILURE)
    {
        g_printerr ("Unable to set the pipeline to the playing state.\n");
        gst_object_unref (data.playbin);
        return -1;
    }   

    bus = gst_element_get_bus(data.playbin);


    return 0;
}