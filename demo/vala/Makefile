cflag = 

src = $(wildcard *.vala)
target = $(patsubst %.vala, %.run, $(src))

all:$(target)

%.run:%.vala
	valac -o $@ $^ $(cflag)

.PHONY:clean all

clean:
	@rm -fr *.run
