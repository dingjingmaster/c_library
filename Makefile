CUR_DIR = $(shell pwd)
LIB_DIR = $(CUR_DIR)/package/lib/
HEAD_DIR = $(CUR_DIR)/package/include/
EXAMPLE_DIR = $(CUR_DIR)/package/example/

libs = -lpthread

src = $(strip $(subst $(CUR_DIR), ., $(wildcard $(CUR_DIR)/*/*.c)))
obj = $(strip $(patsubst %.c, %.o, $(src)))
target = $(strip $(subst $(CUR_DIR), ., $(patsubst %.c, %.run, $(wildcard $(CUR_DIR)/*/*_example.c))))

all:$(target) mk_dir

%.run:$(obj)
	gcc -o $@ $^ $(libs)

%.o:%.c
	gcc -o $@ -c $< 

mk_dir:
	@mkdir -p $(LIB_DIR)
	@mkdir -p $(HEAD_DIR)
	@mkdir -p $(EXAMPLE_DIR)
	@cp $(CUR_DIR)/*/*.h $(HEAD_DIR)
	@cp $(CUR_DIR)/*/*.run $(EXAMPLE_DIR)

.PRECIOUS:%.o

.PHONY:clean

clean: 
	rm -fr package/
	rm -fr $(target)
	rm -fr $(obj)
