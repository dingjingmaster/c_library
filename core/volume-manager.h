//
// Created by dingjing on 2021/3/30.
//

#ifndef LIBCORE_DINGJING_VOLUMEMANAGER_H
#define LIBCORE_DINGJING_VOLUMEMANAGER_H
#include <mutex>
#include <vector>
#include <QObject>
#include <gio/gio.h>

#include "job.h"
#include "global.h"
#include "file-path.h"
#include "icon-info.h"
#include "smart-ptr.hpp"

namespace dingjing
{

class EXPORT_IMPORT_LIBRARY Volume : public GVolumePtr
{
public:
    explicit Volume(GVolume* gvol, bool addRef): GVolumePtr{gvol, addRef}{}

    explicit Volume(GVolumePtr gvol): GVolumePtr{std::move(gvol)} {}

    CStrPtr name() const
    {
        return CStrPtr {g_volume_get_name(get())};
    }

    CStrPtr uuid() const
    {
        return CStrPtr {g_volume_get_uuid(get())};
    }

    std::shared_ptr<const IconInfo> icon() const
    {
        return IconInfo::fromGIcon(GIconPtr{g_volume_get_icon(get()), false});
    }

    // GDrive *	g_volume_get_drive(get());
    GMountPtr mount() const
    {
        return GMountPtr{g_volume_get_mount(get()), false};
    }

    bool canMount() const
    {
        return g_volume_can_mount(get());
    }

    bool shouldAutoMount() const
    {
        return g_volume_should_automount(get());
    }

    FilePath activationRoot() const
    {
        return FilePath{g_volume_get_activation_root(get()), false};
    }

    bool canEject() const
    {
        return g_volume_can_eject(get());
    }

    CStrPtr device() const
    {
        return CStrPtr {g_volume_get_identifier(get(), G_VOLUME_IDENTIFIER_KIND_UNIX_DEVICE)};
    }

    CStrPtr label() const
    {
        return CStrPtr {g_volume_get_identifier(get(), G_VOLUME_IDENTIFIER_KIND_LABEL)};
    }
};

class EXPORT_IMPORT_LIBRARY Mount : public GMountPtr
{
public:
    explicit Mount(GMount* mnt, bool addRef): GMountPtr{mnt, addRef} {
    }

    explicit Mount(GMountPtr gmnt): GMountPtr{std::move(gmnt)} {
    }

    CStrPtr name() const {
        return CStrPtr{g_mount_get_name(get())};
    }

    CStrPtr uuid() const {
        return CStrPtr{g_mount_get_uuid(get())};
    }

    std::shared_ptr<const IconInfo> icon() const {
        return IconInfo::fromGIcon(GIconPtr{g_mount_get_icon(get()), false});
    }

    // GIcon *	g_mount_get_symbolic_icon(get());
    // GDrive *	g_mount_get_drive(get());
    FilePath root() const {
        return FilePath{g_mount_get_root(get()), false};
    }

    GVolumePtr volume() const {
        return GVolumePtr{g_mount_get_volume(get()), false};
    }

    FilePath defaultLocation() const {
        return FilePath{g_mount_get_default_location(get()), false};
    }

    bool canUnmount() const {
        return g_mount_can_unmount(get());
    }
};

class EXPORT_IMPORT_LIBRARY VolumeManager : public QObject
{
    Q_OBJECT
public:
    explicit VolumeManager();

    ~VolumeManager() override;

    const std::vector<Volume>& volumes() const
    {
        return volumes_;
    }

    const std::vector<Mount>& mounts() const
    {
        return mounts_;
    }

    static std::shared_ptr<VolumeManager> globalInstance();

Q_SIGNALS:
    void volumeAdded(const Volume& vol);
    void volumeRemoved(const Volume& vol);
    void volumeChanged(const Volume& vol);

    void mountAdded(const Mount& mnt);
    void mountRemoved(const Mount& mnt);
    void mountChanged(const Mount& mnt);

public Q_SLOTS:

    void onGetGVolumeMonitorFinished();

private:

    class GetGVolumeMonitorJob : public Job
    {
    public:
        GetGVolumeMonitorJob() {}
        GVolumeMonitorPtr monitor_;
    protected:
        void exec() override;
    };

    static void _onGVolumeAdded(GVolumeMonitor* /*mon*/, GVolume* vol, VolumeManager* _this)
    {
        _this->onGVolumeAdded(vol);
    }
    void onGVolumeAdded(GVolume* vol);

    static void _onGVolumeRemoved(GVolumeMonitor* /*mon*/, GVolume* vol, VolumeManager* _this)
    {
        _this->onGVolumeRemoved(vol);
    }
    void onGVolumeRemoved(GVolume* vol);

    static void _onGVolumeChanged(GVolumeMonitor* /*mon*/, GVolume* vol, VolumeManager* _this)
    {
        _this->onGVolumeChanged(vol);
    }
    void onGVolumeChanged(GVolume* vol);

    static void _onGMountAdded(GVolumeMonitor* /*mon*/, GMount* mnt, VolumeManager* _this)
    {
        _this->onGMountAdded(mnt);
    }
    void onGMountAdded(GMount* mnt);

    static void _onGMountRemoved(GVolumeMonitor* /*mon*/, GMount* mnt, VolumeManager* _this)
    {
        _this->onGMountRemoved(mnt);
    }
    void onGMountRemoved(GMount* mnt);

    static void _onGMountChanged(GVolumeMonitor* /*mon*/, GMount* mnt, VolumeManager* _this)
    {
        _this->onGMountChanged(mnt);
    }
    void onGMountChanged(GMount* mnt);

private:
    GVolumeMonitorPtr monitor_;

    std::vector<Volume> volumes_;
    std::vector<Mount> mounts_;

    static std::mutex mutex_;
    static std::weak_ptr<VolumeManager> globalInstance_;
};
}



#endif //LIBCORE_DINGJING_VOLUMEMANAGER_H
