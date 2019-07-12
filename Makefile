CUR_DIR = $(shell pwd)
LIB_DIR = $(CUR_DIR)/package/lib/
HEAD_DIR = $(CUR_DIR)/package/include/
EXAMPLE_DIR = $(CUR_DIR)/package/example/

libs = -lpthread
flag = -Wall
debug_flag = -Wall -Werror -g3 -p

src = $(filter-out [g]test%, $(filter-out %_example.c, $(strip $(subst $(CUR_DIR), ., $(wildcard $(CUR_DIR)/*/*.c)))))
obj = $(strip $(patsubst %.c, %.o, $(src)))
debug_obj = $(strip $(patsubst %.c, %.o_debug, $(src)))
target = $(strip $(subst $(CUR_DIR), ., $(patsubst %.c, %.run, $(wildcard $(CUR_DIR)/*/*_example.c))))
debug_target = $(strip $(subst $(CUR_DIR), ., $(patsubst %.c, %.run_debug, $(wildcard $(CUR_DIR)/*/*_example.c))))

all:$(target) mk_dir

debug:$(debug_target)

%.run_debug:%.o_debug $(debug_obj)
	cc $(debug_flag) -o $@ $^ $(libs)

%.run:%.o $(obj)
	cc ${flag} -o $@ $^ $(libs)

%.o_debug:%.c
	cc $(debug_flag) -o $@ -c $< 

%.o:%.c
	cc $(flag) -o $@ -c $< 

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
	rm -fr $(obj)
	rm -fr $(target)
	rm -fr $(debug_obj)
	rm -fr $(debug_target)
