CC = gcc
cflag = 

gobject_src = $(wildcard *.c)
gobject_target = $(patsubst %.c, %.run, $(gobject_src))
src = $(wildcard */*.c)
obj = $(patsubst %.c, %.o, $(src))

target = $(gobject_target)

all:$(target)

%.run:%.c $(obj)
	$(CC) -o $@ $^ $(cflag) -I*/ `pkg-config --cflags --libs gobject-2.0`

%.o:%.c
	$(CC) -o $@ -c $< $(cflag) `pkg-config --cflags --libs gobject-2.0`


.PHONY:clean all

clean:
	@rm -fr $(obj)
	@rm -fr $(gobject_target)
	@echo 'clean all done!'
