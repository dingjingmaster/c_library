cflag = -w

src = $(wildcard *.c)
target = $(patsubst %.c, %.run, $(src))

all:$(target)

%.run:%.c
	cc -o $@ $^ $(cflag) `pkg-config --cflags --libs gtk+-3.0`

.PHONY:clean all

clean:
	@rm -fr *.run
