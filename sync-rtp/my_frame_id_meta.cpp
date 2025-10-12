// #include "my_frame_id_meta.h" // Assuming you put the definitions above in a header

// // --- Boilerplate Functions for GStreamer Metadata ---

// static gboolean
// my_frame_id_meta_init (GstMeta * meta, gpointer params, GstBuffer * buffer)
// {
//   GstMyFrameIdMeta *mymeta = (GstMyFrameIdMeta *) meta;
//   mymeta->frame_id = 0; // Initialize ID to 0
//   return TRUE;
// }

// static gboolean
// my_frame_id_meta_free (GstMeta * meta, GstBuffer * buffer)
// {
//   // No complex memory to free here, but required boilerplate
//   return TRUE;
// }

// static gboolean
// my_frame_id_meta_transform (GstBuffer * dest, GstMeta * meta, GstBuffer * src,
//     GQuark type, gpointer data)
// {
//   GstMyFrameIdMeta *dmeta = (GstMyFrameIdMeta *) meta;

//   // The ID must be copied to the destination buffer to be kept
//   if (GST_META_TRANSFORM_IS_COPY (type)) {
//     GstMyFrameIdMeta *new_meta = gst_buffer_add_my_frame_id_meta (dest, dmeta->frame_id);
//     return (new_meta != NULL);
//   }

//   return TRUE;
// }

// // 
// GType
// gst_my_frame_id_meta_api_get_type (void)
// {
//   static volatile GType type;
//   static const gchar *tags[] = { "frame", "id", NULL };

//   // if (g_once_init_enter (&type)) {
//     GType _type = gst_meta_api_type_register ("GstMyFrameIdMetaAPI", tags);
//     g_once_init_leave (&type, _type);
//   // }
//   return type;
// }

// const GstMetaInfo *
// gst_my_frame_id_meta_get_info (void)
// {

//   /*
//   struct _GstMetaInfo {
//   GType                      api;
//   GType                      type;
//   gsize                      size;

//   GstMetaInitFunction        init_func;
//   GstMetaFreeFunction        free_func;
//   GstMetaTransformFunction   transform_func;

//   /* No padding needed, GstMetaInfo is always allocated by GStreamer and is
//    * not subclassable or stack-allocatable, so we can extend it as we please
//    * just like interfaces 
// };
  
//  */
//   auto type = gst_my_frame_id_meta_api_get_type();
  
//   static const GstMetaInfo info = {
//     (GstMetaAPIFunc) gst_my_frame_id_meta_api_get_type,
//     type,
//     32,
//     (GstMetaInitFunction) my_frame_id_meta_init,
//     (GstMetaFreeFunction) my_frame_id_meta_free,
//     (GstMetaTransformFunction) my_frame_id_meta_transform,
//   };

//   return &info;
// }

// // 3. The actual helper function to call
// GstMyFrameIdMeta *
// gst_buffer_add_my_frame_id_meta (GstBuffer *buffer, guint64 frame_id)
// {
//   GstMyFrameIdMeta *meta = (GstMyFrameIdMeta *)
//       gst_buffer_add_meta (buffer, GST_MY_FRAME_ID_META_INFO, NULL);
//   meta->frame_id = frame_id;
//   return meta;
// }


