LIB_NAME = libdjctools.a
CUR_DIR = $(shell pwd)
LIB_DIR = $(CUR_DIR)/package/lib/
HEAD_DIR = $(CUR_DIR)/package/include/djctool/
DEBUG_DIR = $(CUR_DIR)/package/debug/
EXAMPLE_DIR = $(CUR_DIR)/package/example/

HEADS = -I$(CUR_DIR)/common/

INSTALL_LIB_DIR = /usr/lib/
INSTALL_HEAD_DIR = /usr/include/djctool/

libs = -lpthread
flag = -Wall -Werror \
	   -Wno-error=format-security
debug_flag = -Wall -Werror -g3 -p

src = $(filter-out [g]test%, $(filter-out %_example.c, $(strip $(subst $(CUR_DIR), ., $(wildcard $(CUR_DIR)/*/*.c)))))
obj = $(strip $(patsubst %.c, %.o, $(src)))
target = $(strip $(subst $(CUR_DIR), ., $(patsubst %.c, %.run, $(wildcard $(CUR_DIR)/*/*_example.c))))

debug_obj = $(strip $(patsubst %.c, %.o_debug, $(src)))
debug_target = $(strip $(subst $(CUR_DIR), ., $(patsubst %.c, %.run_debug, $(wildcard $(CUR_DIR)/*/*_example.c))))


help:
	@echo '帮助:'
	@echo '    make help ---- 显示本帮助' 
	@echo '    make all ---- 编译所有的库文件为.a(静态读), 并把头文件进行打包'
	@echo '    make frame ---- 编译frame并打包到指定目录'
	@echo '    make demo ---- C语言一些库的例子，有些可能是图形界面相关，编译后可用单独运行'
	@echo '    make install ---- 将库文件安装到/lib下，将库文件对应的头文件安装到/usr/inclde下'
	@echo '    make clean ---- 清空所有编译产生的二进制文件'

all:$(target) static_lib mk_dir
	@cd frame && make all

demo:
	@cd demo && make all

frame:
	@cd frame && make all

install:all
	@mkdir -p $(INSTALL_HEAD_DIR)
	cp $(HEAD_DIR)/* -r $(INSTALL_HEAD_DIR)
	cp $(LIB_DIR)/* $(INSTALL_LIB_DIR)
	@echo "install done!"

debug:$(debug_target) mk_dir_debug

static_lib: $(obj)
	ar rcs -o $(LIB_NAME) $^

%.run_debug:%.o_debug $(debug_obj)
	cc $(debug_flag) $(HEADS) -o $@ $^ $(libs)

%.run:%.o $(obj)
	cc ${flag} $(HEADS) -o $@ $^ $(libs)

%.o_debug:%.c
	cc $(debug_flag) $(HEADS) -o $@ -c $<

%.o:%.c
	cc $(flag) $(HEADS) -o $@ -c $< 

mk_dir:
	@mkdir -p $(LIB_DIR)
	@mkdir -p $(HEAD_DIR)
	@mkdir -p $(EXAMPLE_DIR)
	@cp $(CUR_DIR)/*/*.h $(HEAD_DIR)
	@cp $(CUR_DIR)/*/*.run $(EXAMPLE_DIR)
	@mv $(LIB_NAME) $(LIB_DIR)

mk_dir_debug:
	@mkdir -p $(LIB_DIR)
	@mkdir -p $(HEAD_DIR)
	@mkdir -p $(DEBUG_DIR)
	@cp $(CUR_DIR)/*/*.h $(HEAD_DIR)
	@cp $(CUR_DIR)/*/*.run_debug $(DEBUG_DIR)

.PHONY:clean demo help all frame install

clean: 
	@rm -fr $(obj)
	@rm -fr $(target)
	@rm -fr $(debug_obj)
	@rm -fr $(debug_target)
	@rm -fr package/
	@cd frame && make clean
	@cd demo && make clean
	@echo "clean all!"
