CC = gcc
cflag = 
gtk2_src = $(wildcard gtk2/*.c)
gtk2_target = $(patsubst %.c, %_gtk2.run, $(gtk2_src))

target = $(gtk2_target)

all:$(target)

%_gtk2.run:%.c
	$(CC) -o $@ $^ $(cflag) `pkg-config --cflags --libs gtk+-2.0`

.PHONY:clean all

clean:
	@rm -fr $(gtk2_target)
	@echo 'clean all done!'
