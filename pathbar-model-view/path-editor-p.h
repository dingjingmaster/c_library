#ifndef PATHEDITORP_H
#define PATHEDITORP_H

#include <QObject>
#include <gio/gio.h>


namespace dingjing
{
class PathEditor;

class PathEditorJob : public QObject
{
    Q_OBJECT
public:
    ~PathEditorJob() override
    {
        g_object_unref(mDirName);
        g_object_unref(mCancellable);
    }

Q_SIGNALS:
    void finished();

public Q_SLOTS:
    void runJob();

public:
    GCancellable*           mCancellable;
    GFile*                  mDirName;
    QStringList             mSubDirs;
    PathEditor*             mEdit;
    bool                    mTriggeredByFocusInEvent;
};

}
#endif // PATHEDITORP_H
