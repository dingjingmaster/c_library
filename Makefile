
CUR_DIR = $(shell pwd)
LIB_DIR = $(CUR_DIR)/package/lib/
HEAD_DIR = $(CUR_DIR)/package/include/
EXAMPLE_DIR = $(CUR_DIR)/package/example/

libs = -lpthread

src = $(wildcard $(CUR_DIR)/*/*.c)
obj = $(patsubst %.c, %.o, $(src))
target = $(patsubst %.c, %.run, $(wildcard $(CUR_DIR)/*/*_example.c))

all : $(target) mk_dir

%.run : $(obj)
	gcc -o $@ $^ $(libs)
%.o : %.c
	gcc -o $@ -c $< $(libs)
mk_dir :
	mkdir -p $(LIB_DIR)
	mkdir -p $(HEAD_DIR)
	mkdir -p $(EXAMPLE_DIR)
	cp $(CUR_DIR)/*/*.h $(HEAD_DIR)
	cp $(CUR_DIR)/*/*.run $(EXAMPLE_DIR)

.PHONY : clean


clean: 
	rm $(obj)
	rm $(target)
	rm -fr $(CUR_DIR)/package/





