cflag = -Wall

target = $(patsubst %.c, %.so, $(wildcard pam_module_*.c))
test_target = $(patsubst %.c, %.run, $(wildcard pam_test_*.c))

all:$(target) $(test_target)

%.run:%.c
	cc $< -o $@ $(cflag) -lpam -lpam_misc -ldjctools

%.so:%.c
	cc $< -o $@ $(cflag) -fPIC -shared -ldjctools

.PHONY:clean all target test_target pic

clean:
	@rm -fr *.so
	@rm -fr *.run
