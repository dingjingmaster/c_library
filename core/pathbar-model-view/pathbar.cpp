#include "pathbar.h"

#include "path-editor.h"
#include "pathbar-p.h"

#include <QMenu>
#include <QTimer>
#include <QDebug>
#include <QClipboard>
#include <QScrollBar>
#include <QToolButton>
#include <QScrollArea>
#include <QHBoxLayout>
#include <QApplication>
#include <QResizeEvent>
#include <QContextMenuEvent>


namespace dingjing
{

PathBar::PathBar(QWidget* parent) : QWidget(parent), mTempPathEdit(nullptr), mToggledBtn(nullptr)
{
    QHBoxLayout* topLayout = new QHBoxLayout(this);
    topLayout->setContentsMargins(0, 0, 0, 0);
    topLayout->setSpacing(0);
    bool rtl(layoutDirection() == Qt::RightToLeft);

    // the arrow button used to scroll to start of the path
    mScrollToStart = new QToolButton(this);
    mScrollToStart->setArrowType(rtl ? Qt::RightArrow : Qt::LeftArrow);
    mScrollToStart->setAutoRepeat(true);
    mScrollToStart->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::MinimumExpanding);
    connect(mScrollToStart, &QToolButton::clicked, this, &PathBar::onScrollButtonClicked);
    topLayout->addWidget(mScrollToStart);

    // there might be too many buttons when the path is long, so make it scrollable.
    mScrollArea = new QScrollArea(this);
    mScrollArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    mScrollArea->setFrameShape(QFrame::NoFrame);
    mScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    mScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    mScrollArea->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    mScrollArea->verticalScrollBar()->setDisabled(true);
    connect(mScrollArea->horizontalScrollBar(), &QAbstractSlider::valueChanged, this, &PathBar::setArrowEnabledState);
    topLayout->addWidget(mScrollArea, 1); // stretch factor=1, make it expandable

    // the arrow button used to scroll to end of the path
    mScrollToEnd = new QToolButton(this);
    mScrollToEnd->setArrowType(rtl ? Qt::LeftArrow : Qt::RightArrow);
    mScrollToEnd->setAutoRepeat(true);
    mScrollToEnd->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::MinimumExpanding);
    connect(mScrollToEnd, &QToolButton::clicked, this, &PathBar::onScrollButtonClicked);
    topLayout->addWidget(mScrollToEnd);

    // container widget of the path buttons
    mButtonsWidget = new QWidget(this);
    mButtonsWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    mButtonsLayout = new QHBoxLayout(mButtonsWidget);
    mButtonsLayout->setContentsMargins(0, 0, 0, 0);
    mButtonsLayout->setSpacing(0);
    mButtonsLayout->setSizeConstraint(QLayout::SetFixedSize); // required when added to scroll area according to QScrollArea doc.
    mScrollArea->setWidget(mButtonsWidget); // make the buttons widget scrollable if the path is too long
}

void PathBar::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);
    if(event->oldSize().width() != event->size().width()) {
        updateScrollButtonVisibility();
        QTimer::singleShot(0, this, SLOT(ensureToggledVisible()));
    }
}

void PathBar::wheelEvent(QWheelEvent* event)
{
    QWidget::wheelEvent(event);
    QAbstractSlider::SliderAction action = QAbstractSlider::SliderNoAction;
    int vDelta = event->angleDelta().y();
    if(vDelta > 0) {
        if(mScrollToStart->isEnabled()) {
            action = QAbstractSlider::SliderSingleStepSub;
        }
    }
    else if(vDelta < 0) {
        if(mScrollToEnd->isEnabled()) {
            action = QAbstractSlider::SliderSingleStepAdd;
        }
    }
    mScrollArea->horizontalScrollBar()->triggerAction(action);
}

void PathBar::mousePressEvent(QMouseEvent* event)
{
    QWidget::mousePressEvent(event);
    if(event->button() == Qt::LeftButton) {
        openEditor();
    }
    else if(event->button() == Qt::MiddleButton) {
        PathButton* btn = qobject_cast<PathButton*>(childAt(event->x(), event->y()));
        if(btn != nullptr) {
            mScrollArea->ensureWidgetVisible(btn,
                                             1); // a harmless compensation for a miscalculation in Qt
            Q_EMIT middleClickChdir(pathForButton(btn));
        }
    }
}

void PathBar::contextMenuEvent(QContextMenuEvent* event)
{
    QMenu* menu = new QMenu(this);
    connect(menu, &QMenu::aboutToHide, menu, &QMenu::deleteLater);

    QAction* action = menu->addAction(tr("&Edit Path"));
    connect(action, &QAction::triggered, this, &PathBar::openEditor);

    action = menu->addAction(tr("&Copy Path"));
    connect(action, &QAction::triggered, this, &PathBar::copyPath);

    menu->popup(mapToGlobal(event->pos()));
}

void PathBar::updateScrollButtonVisibility()
{
    // Wait for the horizontal scrollbar to be completely shaped.
    // Without this, the enabled state of arrow buttons might be
    // wrong when the pathbar is created for the first time.
    QTimer::singleShot(0, this, SLOT(setScrollButtonVisibility()));
}

void PathBar::setScrollButtonVisibility()
{
    bool showScrollers;
    if(mTempPathEdit != nullptr) {
        showScrollers = false;
    } else {
        showScrollers = (mButtonsLayout->sizeHint().width() > width());
    }
    mScrollToStart->setVisible(showScrollers);
    mScrollToEnd->setVisible(showScrollers);
    if(showScrollers) {
        QScrollBar* sb = mScrollArea->horizontalScrollBar();
        int value = sb->value();
        mScrollToStart->setEnabled(value != sb->minimum());
        mScrollToEnd->setEnabled(value != sb->maximum());
        // align scroll buttons horizontally
        mScrollToStart->setMaximumHeight(qMax(mButtonsWidget->height(), mScrollToStart->minimumSizeHint().height()));
        mScrollToEnd->setMaximumHeight(qMax(mButtonsWidget->height(), mScrollToEnd->minimumSizeHint().height()));
    }
}

FilePath PathBar::pathForButton(PathButton* btn)
{
    std::string fullPath;
    int buttonCount = mButtonsLayout->count() - 1; // the last item is a spacer
    for(int i = 0; i < buttonCount; ++i) {
        if(!fullPath.empty() && fullPath.back() != '/') {
            fullPath += '/';
        }
        PathButton* elem = static_cast<PathButton*>(mButtonsLayout->itemAt(i)->widget());
        fullPath += elem->name();
        if(elem == btn)
            break;
    }
    return FilePath::fromPathStr(fullPath.c_str());
}

void PathBar::onButtonToggled(bool checked)
{
    if(checked) {
        PathButton* btn = static_cast<PathButton*>(sender());
        mToggledBtn = btn;
        mCurrentPath = pathForButton(btn);
        Q_EMIT chdir(mCurrentPath);

        // since scrolling to the toggled buton will happen correctly only when the
        // layout is updated and because the update is disabled on creating buttons
        // in setPath(), the update status can be used as a sign to know when to wait
        if(updatesEnabled()) {
            mScrollArea->ensureWidgetVisible(btn, 1);
        } else {
            QTimer::singleShot(0, this, SLOT(ensureToggledVisible()));
        }
    }
}

void PathBar::ensureToggledVisible()
{
    if(mToggledBtn != nullptr && mTempPathEdit == nullptr) {
        mScrollArea->ensureWidgetVisible(mToggledBtn, 1);
    }
}

void PathBar::onScrollButtonClicked()
{
    QToolButton* btn = static_cast<QToolButton*>(sender());
    QAbstractSlider::SliderAction action = QAbstractSlider::SliderNoAction;
    if(btn == mScrollToEnd) {
        action = QAbstractSlider::SliderSingleStepAdd;
    }
    else if(btn == mScrollToStart) {
        action = QAbstractSlider::SliderSingleStepSub;
    }
    mScrollArea->horizontalScrollBar()->triggerAction(action);
}

void PathBar::setPath(FilePath path)
{
    if(mCurrentPath == path) { // same path, do nothing
        return;
    }

    auto oldPath = std::move(mCurrentPath);
    mCurrentPath = std::move(path);
    // check if we already have a button for this path
    int buttonCount = mButtonsLayout->count() - 1; // the last item is a spacer
    if(oldPath && mCurrentPath.isPrefixOf(oldPath)) {
        for(int i = buttonCount - 1; i >= 0; --i) {
            auto btn = static_cast<PathButton*>(mButtonsLayout->itemAt(i)->widget());
            if(pathForButton(btn) == mCurrentPath) {
                btn->setChecked(true); // toggle the button
                /* we don't need to emit chdir signal here since later
                 * toggled signal will be triggered on the button, which
                 * in turns emit chdir. */
                return;
            }
        }
    }

    /* FIXME: if the new path is the subdir of our full path, actually
     *        we can append several new buttons rather than re-create
     *        all of the buttons. This can reduce flickers. */

    setUpdatesEnabled(false);
    mToggledBtn = nullptr;
    // we do not have the path in the buttons list
    // destroy existing path element buttons and the spacer
    QLayoutItem* item;
    while((item = mButtonsLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    // create new buttons for the new path
    auto btnPath = mCurrentPath;
    while(btnPath) {
        std::string name;
        QString displayName;
        auto parent = btnPath.parent();
        // FIXME: some buggy uri types, such as menu://, fail to return NULL when there is no parent path.
        // Instead, the path itself is returned. So we check if the parent path is the same as current path.
        auto isRoot = !parent.isValid() || parent == btnPath;
        if(isRoot) {
            displayName = QString::fromUtf8(btnPath.displayName().get());
            name = btnPath.toString().get();
        }
        else {
            displayName = QString::fromUtf8(btnPath.baseName().get());
            // NOTE: "name" is used for making the path from its components in PathBar::pathForButton().
            // In places like folders inside trashes of mounted volumes, FilePath::baseName() cannot be
            // used for making a full path. On the other hand, the base name of FilePath::displayName()
            // causes trouble when a file name contains newline or tab.
            //
            // Therefore, we simply set "name" to the last component of FilePath::toString().
            auto pathStr = QString::fromUtf8(btnPath.toString().get());
            pathStr = pathStr.section(QLatin1Char('/'), -1);
            name = pathStr.toStdString();
        }
        // double ampersands to distinguish them from mnemonics
        displayName.replace(QLatin1Char('&'), QLatin1String("&&"));
        auto btn = new PathButton(name, displayName, isRoot, mButtonsWidget);
        btn->show();
        connect(btn, &QAbstractButton::toggled, this, &PathBar::onButtonToggled);
        mButtonsLayout->insertWidget(0, btn);
        if(isRoot) { // this is the root element of the path
            break;
        }
        btnPath = parent;
    }
    mButtonsLayout->addStretch(1); // add a spacer at the tail of the buttons

    // we don't want to scroll vertically. make the scroll area fit the height of the buttons
    // FIXME: this is a little bit hackish :-(
    mScrollArea->setFixedHeight(mButtonsLayout->sizeHint().height());
    updateScrollButtonVisibility();

    // to guarantee that the button will be scrolled to correctly,
    // it should be toggled only after the layout update starts above
    buttonCount = mButtonsLayout->count() - 1;
    if(buttonCount > 0) {
        PathButton* lastBtn = static_cast<PathButton*>(mButtonsLayout->itemAt(buttonCount - 1)->widget());
        // we don't have to emit the chdir signal since the "onButtonToggled()" slot will be triggered by this.
        lastBtn->setChecked(true);
    }

    setUpdatesEnabled(true);
}

void PathBar::openEditor()
{
    if(mTempPathEdit == nullptr) {
        mTempPathEdit = new PathEditor(this);
        delete layout()->replaceWidget(mScrollArea, mTempPathEdit, Qt::FindDirectChildrenOnly);
        mScrollArea->hide();
        mScrollToStart->setVisible(false);
        mScrollToEnd->setVisible(false);
        mTempPathEdit->setText(QString::fromUtf8(mCurrentPath.toString().get()));

        connect(mTempPathEdit, &PathEditor::returnPressed, this, &PathBar::onReturnPressed);
        connect(mTempPathEdit, &PathEditor::editingFinished, this, &PathBar::closeEditor);
    }
    mTempPathEdit->selectAll();
    QApplication::clipboard()->setText(mTempPathEdit->text(), QClipboard::Selection);
    QTimer::singleShot(0, mTempPathEdit, SLOT(setFocus()));
}

void PathBar::closeEditor()
{
    if(mTempPathEdit == nullptr) {
        return;
    }
    // If a menu has popped up synchronously (with QMenu::exec), the path buttons may be drawn
    // but the path-edit may not disappear until the menu is closed. So, we hide it here.
    // However, since hiding the path-edit makes it lose focus and emit editingFinished(),
    // we should first disconnect from it to avoid recursive calling of the current function.
    mTempPathEdit->disconnect();
    mTempPathEdit->setVisible(false);
    delete layout()->replaceWidget(mTempPathEdit, mScrollArea, Qt::FindDirectChildrenOnly);
    mScrollArea->show();
    if(mButtonsLayout->sizeHint().width() > width()) {
        mScrollToStart->setVisible(true);
        mScrollToEnd->setVisible(true);
    }

    mTempPathEdit->deleteLater();
    mTempPathEdit = nullptr;
    updateScrollButtonVisibility();

    Q_EMIT editingFinished();
}

void PathBar::copyPath()
{
    QApplication::clipboard()->setText(QString::fromUtf8(mCurrentPath.toString().get()));
}

void PathBar::onReturnPressed()
{
    QByteArray pathStr = mTempPathEdit->text().toLocal8Bit();
    setPath(FilePath::fromPathStr(pathStr.constData()));
}

void PathBar::setArrowEnabledState(int value)
{
    if(mButtonsLayout->sizeHint().width() > width()) {
        QScrollBar* sb = mScrollArea->horizontalScrollBar();
        mScrollToStart->setEnabled(value != sb->minimum());
        mScrollToEnd->setEnabled(value != sb->maximum());
    }
}

}
