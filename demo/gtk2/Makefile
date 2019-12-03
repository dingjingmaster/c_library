CC = gcc
cflag = 

src = $(wildcard *.c)
target = $(patsubst %.c, %.run, $(src))

all:$(target)

%.run:%.c
	$(CC) -o $@ $^ $(cflag) `pkg-config --cflags --libs gtk+-2.0`

.PHONY:clean all

clean:
	@rm -fr *.run
	@echo 'clean all done!'
