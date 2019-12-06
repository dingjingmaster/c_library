cflag = -w

heads = \
	-I/usr/include/glib-2.0/

demo1_src = $(wildcard dbus_name/*.c)
demo1_obj = $(patsubst %.c, %.o, $(demo1_src))

demo2_src = $(wildcard dbus_recv_send/*.c)
demo2_obj = $(patsubst %.c, %.o, $(demo2_src))

gobject_target =\
	dbus_recv_send/listen.run\
	dbus_recv_send/send.run\
	dbus_name/dbus_name.run 

target = $(gobject_target)

all:$(target)

dbus_recv_send/send.run:dbus_recv_send/send.o
	cc -o $@ $^ $(cflag) $(heads) `pkg-config --cflags --libs dbus-1`

dbus_recv_send/listen.run:dbus_recv_send/listen.o
	cc -o $@ $^ $(cflag) $(heads) `pkg-config --cflags --libs dbus-1 glib-2.0 dbus-glib-1`

dbus_name/dbus_name.run: $(demo1_obj)
	cc -o $@ $^ $(cflag) $(heads) `pkg-config --cflags --libs dbus-1`

%.o:%.c
	cc -o $@ -c $< $(cflag) $(heads) `pkg-config --cflags --libs dbus-1 glib-2.0 dbus-glib-1`


.PHONY:clean all

clean:
	@rm -fr *.o
	@rm -fr *.run
	@rm -fr */*.o
	@rm -fr */*.run
