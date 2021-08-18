#include <QCoreApplication>
#include <QDBusConnection>
#include <QDBusMessage>
#include <QDBusPendingReply>
#include <QDBusUnixFileDescriptor>
#include <gio/gio.h>
#include <gio/gunixfdlist.h>

void systemSleep(GDBusConnection *connection, const gchar *senderName, const gchar *objectPath, const gchar *interfaceName, const gchar *signalName, GVariant *parameters, gpointer udata);

int main(int argc, char *argv[])
{
    const char* b = "block";
    if (argc > 1) {
        b = "delay";
    }

    QCoreApplication a(argc, argv);

#if 1
    g_autoptr(GError) error = NULL;
    g_autoptr(GDBusConnection) pconnection = g_bus_get_sync(G_BUS_TYPE_SYSTEM, NULL, &error);
    if (error) {
        printf("error:%s\n", error->message);
        return -1;
    }

    if (pconnection) {
        g_autoptr(GError) error2 = nullptr;

        GUnixFDList*        fds = NULL;

        g_autoptr(GVariantType) rtype = g_variant_type_new("(h)");

        GVariantBuilder builder;
        g_variant_builder_init(&builder, G_VARIANT_TYPE_TUPLE);
        g_variant_builder_add(&builder, "s", "sleep");
        g_variant_builder_add(&builder, "s", "peony");
        g_variant_builder_add(&builder, "s", "Test");
        g_variant_builder_add(&builder, "s", "block");

        GVariant* ret = g_dbus_connection_call_with_unix_fd_list_sync(pconnection,
                                                    "org.freedesktop.login1",
                                                    "/org/freedesktop/login1",
                                                    "org.freedesktop.login1.Manager",
                                                    "Inhibit",
                                                    g_variant_new("(ssss)", "sleep", "peony", "Test", b)
                                                    /*g_variant_builder_end(&builder)*/,
                                                    rtype,
                                                    G_DBUS_CALL_FLAGS_NONE,
                                                    G_MAXINT, NULL, &fds,
                                                    NULL, &error2);
        if (error2) {
            printf("cannot block s4: %s\n", error2->message);
        } else if (ret) {
            gint l = g_unix_fd_list_get_length(fds);
            for (auto i = 0; i < l; ++i) {
                g_autoptr(GError) error = NULL;
                int fd = g_unix_fd_list_get(fds, i, &error);
                if (error) {
                    printf("get fd error: %s\n", error->message);
                    continue;
                }

                g_dbus_connection_set_exit_on_close(pconnection, TRUE);

                g_dbus_connection_flush_sync(pconnection, NULL, NULL);
                printf("fd: %d\n", fd);
                sleep(5);

                if (fd > 0) {
                    ::close(fd);
                }


            }


            g_dbus_connection_close_sync(pconnection, NULL, &error);
            if (error) {
                printf ("close error: %s\n", error->message);
            }

            g_object_unref(fds);
            g_object_unref(pconnection);

            printf("close\n");
        }
    }
#endif

#if 0
    QDBusMessage message = QDBusMessage::createMethodCall("org.freedesktop.login1", "/org/freedesktop/login1", \
                                                        "org.freedesktop.login1.Manager", QStringLiteral("Inhibit"));
    message.setArguments(QVariantList({QStringLiteral("sleep"), "TEST", "Inhibitor Test", QStringLiteral("block")}));

    QDBusPendingReply <QDBusUnixFileDescriptor> reply = QDBusConnection::systemBus().call(message);
    if (!reply.isValid()) {
        return -1;
    }
#endif

#if 0
    GDBusConnection* conn = g_bus_get_sync(G_BUS_TYPE_SYSTEM, NULL, NULL);
    if (conn) {
        g_dbus_connection_signal_subscribe(conn,
                                           "org.freedesktop.login1",
                                           "org.freedesktop.login1.Manager",
                                           "PrepareForSleep",
                                           "/org/freedesktop/login1", NULL,
                                           G_DBUS_SIGNAL_FLAGS_NONE,
                                           systemSleep, NULL, NULL);
    }

#endif

    return a.exec();
}


void systemSleep(GDBusConnection *connection, const gchar *senderName, const gchar *objectPath, \
                const gchar *interfaceName, const gchar *signalName, GVariant *parameters, gpointer udata)
{
    g_autoptr(GDBusConnection) pconnection = g_bus_get_sync(G_BUS_TYPE_SYSTEM, NULL, NULL);
    if (pconnection) {
        g_autoptr(GError) error2 = nullptr;
        g_autoptr(GVariantType) rtype = g_variant_type_new("(h)");

        GVariant* ret = g_dbus_connection_call_sync(pconnection,
                                                    "org.freedesktop.login1",
                                                    "/org/freedesktop/login1",
                                                    "org.freedesktop.login1.Manager",
                                                    "Inhibit",
                                                    g_variant_new("(ssss)", "sleep", "peony", "block", "block"),
                                                    rtype, G_DBUS_CALL_FLAGS_NONE, G_MAXINT, nullptr, &error2);
        if (error2) {
            printf("cannot block s4: %s", error2->message);
        } else if (ret) {
            int rret = g_variant_get_uint32(ret);
            g_variant_unref(ret);
            printf("ret fd: %d\n", rret);
        }
    }


    Q_UNUSED(udata)
    Q_UNUSED(connection)
    Q_UNUSED(senderName)
    Q_UNUSED(objectPath)
    Q_UNUSED(signalName)
    Q_UNUSED(parameters)
    Q_UNUSED(interfaceName)
}
