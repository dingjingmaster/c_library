cflag = -w

demo1_src = $(wildcard demo1/*.c)
demo1_obj = $(patsubst %.c, %.o, $(demo1_src))

demo2_src = $(wildcard demo2/*.c)
demo2_obj = $(patsubst %.c, %.o, $(demo2_src))

demo3_src = $(wildcard demo3/*.c)
demo3_obj = $(patsubst %.c, %.o, $(demo3_src))

gobject_target =\
	demo1.run \
	demo2.run \
	demo3.run

target = $(gobject_target)

all:$(target)

demo3.run:demo3.c $(demo3_obj)
	cc -o $@ $^ $(cflag) -I*/ `pkg-config --cflags --libs gobject-2.0`

demo2.run:demo2.c $(demo2_obj)
	cc -o $@ $^ $(cflag) -I*/ `pkg-config --cflags --libs gobject-2.0`

demo1.run:demo1.c $(demo1_obj)
	cc -o $@ $^ $(cflag) -I*/ `pkg-config --cflags --libs gobject-2.0`

%.o:%.c
	cc -o $@ -c $< $(cflag) `pkg-config --cflags --libs gobject-2.0`


.PHONY:clean all

clean:
	@rm -fr *.run
	@rm -fr */*.o
