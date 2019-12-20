cflag = -w

demo1_src = $(wildcard demo1/*.c)
demo1_obj = $(patsubst %.c, %.o, $(demo1_src))

demo2_src = $(wildcard demo2/*.c)
demo2_obj = $(patsubst %.c, %.o, $(demo2_src))

demo3_src = $(wildcard demo3/*.c)
demo3_obj = $(patsubst %.c, %.o, $(demo3_src))

demo4_src = $(wildcard demo4/*.c)
demo4_obj = $(patsubst %.c, %.o, $(demo4_src))

demo5_src = $(wildcard demo5/*.c)
demo5_obj = $(patsubst %.c, %.o, $(demo5_src))

demo6_src = $(wildcard demo6.c)
demo6_obj = $(patsubst %.c, %.o, $(demo6_src))

demo7_src = $(wildcard demo7.c)
demo7_obj = $(patsubst %.c, %.o, $(demo7_src))

demo8_src = $(wildcard demo8.c)
demo8_obj = $(patsubst %.c, %.o, $(demo8_src))

gobject_target =\
	demo8.run \
	demo7.run \
	demo6.run \
	demo4.run \
	demo3.run \
	demo2.run \
	demo1.run 

target = $(gobject_target)

all:$(target)

demo8.run:$(demo8_obj)
	cc -o $@ $^ $(cflag) -I*/ `pkg-config --cflags --libs gobject-2.0`

demo7.run:$(demo7_obj)
	cc -o $@ $^ $(cflag) -I*/ `pkg-config --cflags --libs gobject-2.0`

demo6.run:$(demo6_obj)
	cc -o $@ $^ $(cflag) -I*/ `pkg-config --cflags --libs gobject-2.0`

#demo5.run:demo5.c $(demo5_obj)
#	cc -o $@ $^ $(cflag) -I*/ `pkg-config --cflags --libs gobject-2.0`

demo4.run:demo4.c $(demo4_obj)
	cc -o $@ $^ $(cflag) -I*/ `pkg-config --cflags --libs gobject-2.0`

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
	@rm -fr *.o
	@rm -fr *.run
	@rm -fr */*.o
