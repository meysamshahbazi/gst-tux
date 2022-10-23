#include <gst/gst.h>

// Structure to contain all our information, so we can pass it to callbacks
typedef struct _CustomData
{
    GstElement *pipeline;
    GstElement *source;
    GstElement *convert;
    GstElement *resample;
    GstElement *sink;

} CustomData;


int main(int argc, char * argv[])
{

    return 0;
}
