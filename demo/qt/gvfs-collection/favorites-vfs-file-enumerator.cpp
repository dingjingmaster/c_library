#include "favorites-vfs-file-enumerator.h"


G_DEFINE_TYPE_WITH_PRIVATE(FavoritesVFSFileEnumerator, vfs_favorites_file_enumerator, G_TYPE_FILE_ENUMERATOR)

static void vfs_favorites_file_enumerator_init (FavoritesVFSFileEnumerator* self)
{
    FavoritesVFSFileEnumeratorPrivate* priv = (FavoritesVFSFileEnumeratorPrivate*) vfs_favorites_file_enumerator_get_instance_private(self);

    self->priv = priv;



}

static void vfs_favorites_file_enumerator_class_init (FavoritesVFSFileEnumeratorClass *klass)
{
    GObjectClass *gobject_class = G_OBJECT_CLASS(klass);
    GFileEnumeratorClass *enumerator_class = G_FILE_ENUMERATOR_CLASS(klass);

//    gobject_class->dispose = enumerator_dispose;

//    enumerator_class->next_file = enumerate_next_file;

//    //async
//    enumerator_class->next_files_async = enumerate_next_files_async;
//    enumerator_class->next_files_finish = enumerate_next_files_finished;

//    enumerator_class->close_fn = enumerator_close;
}
