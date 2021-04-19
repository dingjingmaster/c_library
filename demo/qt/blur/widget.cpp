#include "widget.h"

#include <QVariant>
#include <QX11Info>
#include <X11/Xlib.h>
#include <QMouseEvent>
#include <QApplication>
#include <QPainterPath>
#include <QPaintEvent>

Widget::Widget(QWidget *parent) : QWidget(parent)
{
    setFixedSize(600, 300);
}

Widget::~Widget()
{
}

void Widget::paintEvent(QPaintEvent *e)
{
//    QColor color = this->palette().window().color();
//    QColor colorBase = this->palette().base().color();

//    int R1 = color.red();
//    int G1 = color.green();
//    int B1 = color.blue();
//    qreal a1 = 0.3;

//    int R2 = colorBase.red();
//    int G2 = colorBase.green();
//    int B2 = colorBase.blue();
//    qreal a2 = 1;

//    qreal a = 1 - (1 - a1)*(1 - a2);

//    qreal R = (a1*R1 + (1 - a1)*a2*R2) / a;
//    qreal G = (a1*G1 + (1 - a1)*a2*G2) / a;
//    qreal B = (a1*B1 + (1 - a1)*a2*B2) / a;

//    colorBase.setRed(R);
//    colorBase.setGreen(G);
//    colorBase.setBlue(B);

//    colorBase.setAlphaF(80/100.0);

//    if (qApp->property("blurEnable").isValid()) {
//        bool blurEnable = qApp->property("blurEnable").toBool();
//        if (!blurEnable) {
//            colorBase.setAlphaF(0.9);
//        }
//    } else {
//        colorBase.setAlphaF(0.9);
//    }

    QWidget::paintEvent(e);
}

void Widget::mouseMoveEvent(QMouseEvent *e)
{
    QWidget::mouseMoveEvent(e);
//    if (!mIsDraging) {
//        return;
//    }

//    qreal  dpiRatio = qApp->devicePixelRatio();
//    if (QX11Info::isPlatformX11()) {
//        Display *display = QX11Info::display();
//        Atom netMoveResize = XInternAtom(display, "_NET_WM_MOVERESIZE", False);
//        XEvent xEvent;
//        const auto pos = QCursor::pos();

//        memset(&xEvent, 0, sizeof(XEvent));
//        xEvent.xclient.type = ClientMessage;
//        xEvent.xclient.message_type = netMoveResize;
//        xEvent.xclient.display = display;
//        xEvent.xclient.window = this->winId();
//        xEvent.xclient.format = 32;
//        xEvent.xclient.data.l[0] = pos.x() * dpiRatio;
//        xEvent.xclient.data.l[1] = pos.y() * dpiRatio;
//        xEvent.xclient.data.l[2] = 8;
//        xEvent.xclient.data.l[3] = Button1;
//        xEvent.xclient.data.l[4] = 0;

//        XUngrabPointer(display, CurrentTime);
//        XSendEvent(display, QX11Info::appRootWindow(QX11Info::appScreen()), False, SubstructureNotifyMask | SubstructureRedirectMask, &xEvent);

//        XEvent xevent;
//        memset(&xevent, 0, sizeof(XEvent));

//        xevent.type = ButtonRelease;
//        xevent.xbutton.button = Button1;
//        xevent.xbutton.window = this->winId();
//        xevent.xbutton.x = e->pos().x() * dpiRatio;
//        xevent.xbutton.y = e->pos().y() * dpiRatio;
//        xevent.xbutton.x_root = pos.x() * dpiRatio;
//        xevent.xbutton.y_root = pos.y() * dpiRatio;
//        xevent.xbutton.display = display;

//        XSendEvent(display, this->effectiveWinId(), False, ButtonReleaseMask, &xevent);
//        XFlush(display);

//        if (e->source() == Qt::MouseEventSynthesizedByQt) {
//            if (!this->mouseGrabber()) {
//                this->grabMouse();
//                this->releaseMouse();
//            }
//        }

//        mIsDraging = false;
//    } else {
//        this->move((QCursor::pos() - mOffset) * dpiRatio);
//    }
}

void Widget::mousePressEvent(QMouseEvent *e)
{
    QWidget::mousePressEvent(e);
//    if (e->button() == Qt::LeftButton && !e->isAccepted()) {
//        mIsDraging = true;
//        mOffset = mapFromGlobal(QCursor::pos());
//    }
}

void Widget::mouseReleaseEvent(QMouseEvent *e)
{
    QWidget::mouseReleaseEvent(e);
//    mIsDraging = false;
}

