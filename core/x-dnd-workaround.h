#ifndef XDNDWORKAROUND_H
#define XDNDWORKAROUND_H

#include <QtGlobal>

#include <QObject>
#include <QPointer>
#include <xcb/xcb.h>
#include <QByteArray>
#include <QAbstractNativeEventFilter>

class QDrag;

class XdndWorkaround : public QAbstractNativeEventFilter
{
public:
    explicit XdndWorkaround ();
    ~XdndWorkaround () override;
    bool nativeEventFilter (const QByteArray& eventType, void* message, long* result) override;
    static QByteArray atomName (xcb_atom_t atom);
    static xcb_atom_t internAtom (const char* name, int len = -1);
    static QByteArray windowProperty (xcb_window_t window, xcb_atom_t propAtom, xcb_atom_t typeAtom, int len);
    static void setWindowProperty (xcb_window_t window, xcb_atom_t propAtom, xcb_atom_t typeAtom, void* data, int len, int format = 8);

private:
    bool clientMessage (xcb_client_message_event_t* event);
    bool selectionNotify (xcb_selection_notify_event_t* event);

private:
    bool selectionRequest (xcb_selection_request_event_t* event);
    bool genericEvent (xcb_ge_generic_event_t* event);
    void buttonRelease ();

    QPointer<QDrag>     mLastDrag;

    bool                mXinput2Enabled;
    int                 mXinputOpCode;
    int                 mXinputEventBase;
    int                 mXinputErrorBase;
};

#endif // XDNDWORKAROUND_H
