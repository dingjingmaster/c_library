cflag = -w

demo1_src = $(wildcard dbus_name.c)
demo1_obj = $(patsubst %.c, %.o, $(demo1_src))

gobject_target =\
	dbus_name.run 

target = $(gobject_target)

all:$(target)

dbus_name.run: $(demo1_obj)
	cc -o $@ $^ $(cflag) -I*/ `pkg-config --cflags --libs dbus-1`

%.o:%.c
	cc -o $@ -c $< $(cflag) `pkg-config --cflags --libs dbus-1`


.PHONY:clean all

clean:
	@rm -fr *.o
	@rm -fr *.run
	@rm -fr */*.o
