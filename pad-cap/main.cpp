#include <gst/gst.h>


static gboolean print_field(GQuark field, const GValue *value, gpointer pfx)
{
    gchar *str = gst_value_serialize(value);
    g_print("%s  %15s: %s\n", (gchar *) pfx, g_quark_to_string (field), str);
    g_free (str);
    return TRUE;
}

static void print_caps(const GstCaps *caps, const gchar *pfx)
{
    g_return_if_fail(caps != NULL);

    if(gst_caps_is_any(caps))
    {
        g_print()
    }
}

