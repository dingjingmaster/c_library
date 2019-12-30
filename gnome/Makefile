CUR_DIR = $(shell pwd)
SYS_INCLUDE = /usr/include

src = $(filter-out [g]test%, $(filter-out %_example.c, $(strip $(subst $(CUR_DIR), ., $(wildcard $(CUR_DIR)/*/*.c)))))
obj = $(strip $(patsubst %.c, %.o, $(src)))
target = $(strip $(subst $(CUR_DIR), ., $(patsubst %.c, %.run, $(wildcard $(CUR_DIR)/*/*_example.c))))

flag = -w

libs = `pkg-config --cflags --libs glib-2.0 gobject-2.0`			\
		-I$(CUR_DIR)/../common										\
		-I$(CUR_DIR)/*												\
		-I$(SYS_INCLUDE)/libgtop-2.0/								\
		-lgtop-2.0

all:$(target)
	@echo "make all done!!!"

%.run:%.o $(obj)
	cc ${flag} $(libs) -o $@ $^

%.o:%.c
	cc $(flag) $(libs) -o $@ -c $< 

.PHONY:all clean

clean: 
	@rm -fr *.o
	@rm -fr */*.o
	@rm -fr *.run
	@rm -fr */*.run
	@echo "clean all!"
