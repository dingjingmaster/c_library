cflag = -w

demo1_src = $(wildcard demo1/*.c)
demo1_obj = $(patsubst %.c, %.o, $(demo1_src))

demo2_src = $(wildcard demo2/*.c)
demo2_obj = $(patsubst %.c, %.o, $(demo2_src))

gobject_target =\
	demo1.run \
	demo2.run

target = $(gobject_target)

all:$(target)


demo2.run:demo2.c $(demo2_obj)
	cc -o $@ $^ $(cflag) -I*/ `pkg-config --cflags --libs gobject-2.0`

demo1.run:demo1.c $(demo1_obj)
	cc -o $@ $^ $(cflag) -I*/ `pkg-config --cflags --libs gobject-2.0`

%.o:%.c
	cc -o $@ -c $< $(cflag) `pkg-config --cflags --libs gobject-2.0`


.PHONY:clean all

clean:
	@rm -fr $(obj)
	@rm -fr $(gobject_target)
