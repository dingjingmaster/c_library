#include "path-editor.h"

#include "path-editor-p.h"

#include <QDir>
#include <QTimer>
#include <QDebug>
#include <QThread>
#include <QKeyEvent>
#include <QCompleter>
#include <QStringBuilder>
#include <QStringListModel>
#include <QAbstractItemView>

namespace dingjing
{

void PathEditorJob::runJob()
{
    GError* err = nullptr;
    GFileEnumerator* enu = g_file_enumerate_children(mDirName,
                           // G_FILE_ATTRIBUTE_STANDARD_NAME","
                           G_FILE_ATTRIBUTE_STANDARD_DISPLAY_NAME","
                           G_FILE_ATTRIBUTE_STANDARD_TYPE,
                           G_FILE_QUERY_INFO_NONE, mCancellable,
                           &err);
    if(enu) {
        while(!g_cancellable_is_cancelled(mCancellable)) {
            GFileInfo* inf = g_file_enumerator_next_file(enu, mCancellable, &err);
            if(inf) {
                GFileType type = g_file_info_get_file_type(inf);
                if(type == G_FILE_TYPE_DIRECTORY) {
                    const char* name = g_file_info_get_display_name(inf);
                    // FIXME: encoding conversion here?
                    mSubDirs.append(QString::fromUtf8(name) + QLatin1String("/"));
                }
                g_object_unref(inf);
            }
            else {
                if(err) {
                    g_error_free(err);
                    err = nullptr;
                }
                else { /* EOF */
                    break;
                }
            }
        }
        g_file_enumerator_close(enu, mCancellable, nullptr);
        g_object_unref(enu);
    }
    mSubDirs.sort(Qt::CaseInsensitive);
    // finished! let's update the UI in the main thread
    Q_EMIT finished();
    QThread::currentThread()->quit();
}


PathEditor::PathEditor(QWidget* parent):
    QLineEdit(parent),
    mCompleter(new QCompleter()),
    mCancellable(nullptr),
    mModel(new QStringListModel())
{
    mCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    // we sorted the subdir list case-insensitively, so we do the same thing
    // here for performance improvements (see Qt doc)
    mCompleter->setModelSorting(QCompleter::CaseInsensitivelySortedModel);
    setCompleter(mCompleter);
    mCompleter->setModel(mModel);
    connect(this, &PathEditor::textChanged, this, &PathEditor::onTextChanged);
    connect(this, &PathEditor::textEdited, this, &PathEditor::onTextEdited);
}

PathEditor::~PathEditor()
{
    delete mCompleter;
    if(mModel) {
        delete mModel;
    }
    if(mCancellable) {
        g_cancellable_cancel(mCancellable);
        g_object_unref(mCancellable);
    }
}

void PathEditor::focusInEvent(QFocusEvent* e)
{
    QLineEdit::focusInEvent(e);
    // build the completion list only when we have the keyboard focus
    reloadCompleter(true);
}

void PathEditor::focusOutEvent(QFocusEvent* e)
{
    QLineEdit::focusOutEvent(e);
    // free the completion list since we don't need it anymore
    freeCompleter();
}

bool PathEditor::event(QEvent* e)
{
    if(e->type() == QEvent::KeyPress) {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(e);
        int key = keyEvent->key();
        // Stop Qt from moving the keyboard focus to the next widget when "Tab" is pressed.
        // Instead, we need to do auto-completion in this case.
        if((key == Qt::Key_Tab && keyEvent->modifiers() == Qt::NoModifier)
           || key == Qt::Key_Backtab) {
            e->accept();
            // pressing of "Tab" should be filtered out first; hence the single-shot timer
            QTimer::singleShot(0, mCompleter, [this, key] {
                if(!mCompleter->popup()->isVisible()) {
                    // NOTE: To open the popup, we should insert the current text because the text
                    // may not be typed by the user but set by selecting a row from a previous popup,
                    // in which case, QCompleter::complete() would show only the previous popup.

                    mLastTypedText = text(); // as if text is typed; matches will be updated
                    selectAll();
                    setModified(false); // remove the undo history
                    insert(mLastTypedText);
                }
                else {
                    selectNextCompletionRow(key != Qt::Key_Backtab);
                }
            });
            return true;
        }
        // When the completer popup is visible and "Escape" is pressed, restore the last typed text
        // in addition to closing the popup
        if(key == Qt::Key_Escape && mCompleter->popup()->isVisible() && mLastTypedText!= text()) {
            e->accept();
            QTimer::singleShot(0, mCompleter, [this] {
                mCompleter->popup()->hide();
                setText(mLastTypedText);
            });
            return true;
        }
    }
    return QLineEdit::event(e);
}

void PathEditor::selectNextCompletionRow(bool downward)
{
    int rows = mCompleter->completionCount(); // it is fast because of sorting
    auto popup = mCompleter->popup();
    if(!popup->selectionModel()->hasSelection()) {
        if(mCompleter->setCurrentRow(downward ? 0 : rows - 1)) {
            popup->setCurrentIndex(mCompleter->currentIndex());
            // If there is no ambiguity, insert the text to trigger the next popup.
            // TODO: Just close the popup if users prefer that instead.
            if(rows == 1) {
                mLastTypedText = text();
                selectAll();
                setModified(false);
                insert(mLastTypedText);
            }
        }
    }
    else {
        int selectedRow = popup->selectionModel()->selectedRows().at(0).row();
        mCompleter->setCurrentRow(selectedRow); // it can be selected by user
        if(downward) {
            if(!mCompleter->setCurrentRow(mCompleter->currentRow() + 1)) {
                mCompleter->setCurrentRow(0);
            }
        }
        else if(!mCompleter->setCurrentRow(mCompleter->currentRow() - 1)) {
            mCompleter->setCurrentRow(rows - 1);
        }
        popup->setCurrentIndex(mCompleter->currentIndex());
    }
}

void PathEditor::onTextEdited(const QString& text)
{
    // just replace start tilde with home path if text is changed by user
    if(text == QLatin1String("~") || text.startsWith(QLatin1String("~/"))) {
        QString txt(text);
        txt.replace(0, 1, QDir::homePath());
        mLastTypedText = txt;
        setText(txt); // emits textChanged()
        return;
    }
    mLastTypedText = text;
}

void PathEditor::onTextChanged(const QString& text)
{
    if(text == QLatin1String("~") || text.startsWith(QLatin1String("~/"))) {
        // do nothing with a start tilde because neither Fm::FilePath nor autocompletion
        // understands it; instead, wait until textChanged() is emitted again without it
        // WARNING: replacing tilde may not be safe here
        return;
    }
    // If the text is typed, the last slash means searching child directories.
    // But, since a slash is appended to the name of each directory match, if the text is
    // changed by auto-completion, we should ignore the last slash to remain in the parent directory.
    int pos = text.lastIndexOf(QLatin1Char('/'), mLastTypedText == text ? -1 : -2);
    if(pos >= 0) {
        ++pos;
    }
    else {
        pos = text.length();
    }
    QString newPrefix = text.left(pos);
    if(mCurrentPrefix != newPrefix) {
        mCurrentPrefix = newPrefix;
        // only build the completion list if we have the keyboard focus
        // if we don't have the focus now, then we'll rebuild the completion list
        // when focusInEvent happens. this avoid unnecessary dir loading.
        if(hasFocus()) {
            reloadCompleter(false);
        }
    }
}

void PathEditor::reloadCompleter(bool triggeredByFocusInEvent)
{
    // parent dir has been changed, reload dir list
    // if(mCurrentPrefix[0] == "~") { // special case for home dir
    // cancel running dir-listing jobs, if there's any
    if(mCancellable) {
        g_cancellable_cancel(mCancellable);
        g_object_unref(mCancellable);
    }

    // create a new job to do dir listing
    PathEditorJob* mJob = new PathEditorJob();
    mJob->mEdit = this;
    mJob->mTriggeredByFocusInEvent = triggeredByFocusInEvent;
    // need to use fm_file_new_for_commandline_arg() rather than g_file_new_for_commandline_arg().
    // otherwise, our own vfs, such as menu://, won't be loaded.
    mJob->mDirName = g_file_new_for_commandline_arg(mCurrentPrefix.toLocal8Bit().constData());
    // qDebug("load: %s", g_file_get_uri(data->dirName));
    mCancellable = g_cancellable_new();
    mJob->mCancellable = (GCancellable*)g_object_ref(mCancellable);

    // launch a new worker thread to handle the job
    QThread* thread = new QThread();
    mJob->moveToThread(thread);
    connect(mJob, &PathEditorJob::finished, this, &PathEditor::onJobFinished, Qt::BlockingQueuedConnection);
    // connect(job, &PathEditorJob::finished, thread, &QThread::quit);
    connect(thread, &QThread::started, mJob, &PathEditorJob::runJob);
    connect(thread, &QThread::finished, thread, &QObject::deleteLater);
    connect(thread, &QThread::finished, mJob, &QObject::deleteLater);
    thread->start(QThread::LowPriority);
}

void PathEditor::freeCompleter()
{
    if(mCancellable) {
        g_cancellable_cancel(mCancellable);
        g_object_unref(mCancellable);
        mCancellable = nullptr;
    }
    mModel->setStringList(QStringList());
}

// This slot is called from main thread so it's safe to access the GUI
void PathEditor::onJobFinished()
{
    PathEditorJob* data = static_cast<PathEditorJob*>(sender());
    if(!g_cancellable_is_cancelled(data->mCancellable)) {
        // update the completer only if the job is not cancelled
        QStringList::iterator it;
        for(it = data->mSubDirs.begin(); it != data->mSubDirs.end(); ++it) {
            // qDebug("%s", it->toUtf8().constData());
            *it = (mCurrentPrefix % *it);
        }
        mModel->setStringList(data->mSubDirs);
        // trigger completion manually
        if(hasFocus() && !data->mTriggeredByFocusInEvent) {
            mCompleter->complete();
        }
    } else {
        mModel->setStringList(QStringList());
    }
    if(mCancellable) {
        g_object_unref(mCancellable);
        mCancellable = nullptr;
    }
}


}
