cflag = -Wall

target = $(patsubst %.c, %.so, $(wildcard *.c))

all:$(target)

%.so:%.c
	cc $< -o $@ $(cflag) -fPIC -shared

.PHONY:clean all

clean:
	@rm -fr *.so
