#ifndef PATHEDITOR_H
#define PATHEDITOR_H

#include "core/global.h"

#include <gio/gio.h>
#include <QLineEdit>

class QCompleter;
class QStringListModel;

namespace dingjing
{

class PathEditorJob;

class PathEditor : public QLineEdit
{
    Q_OBJECT
public:
    explicit PathEditor(QWidget* parent = nullptr);
    ~PathEditor() override;

protected:
    bool event(QEvent* e) override;
    void focusInEvent(QFocusEvent* e) override;
    void focusOutEvent(QFocusEvent* e) override;

private Q_SLOTS:
    void onTextChanged(const QString& text);
    void onTextEdited(const QString& text);

private:
    void freeCompleter();
    void onJobFinished();
    void selectNextCompletionRow(bool downward);
    void reloadCompleter(bool triggeredByFocusInEvent = false);

private:
    QString                     mCurrentPrefix;
    QString                     mLastTypedText;
    QCompleter*                 mCompleter;
    GCancellable*               mCancellable;
    QStringListModel*           mModel;
};

}
#endif // PATHEDITOR_H
