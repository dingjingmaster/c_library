lib_name = libdjctools.a
current = $(shell pwd)
lib_dir = $(current)/package/lib/
head_dir = $(current)/package/include/djctool/
linux_heade = /usr/include/
debug_dir = $(current)/package/debug/
example_dir = $(current)/package/example/

heads = \
    -I$(current)/common/					\

install_lib_dir = \
	/usr/lib/								\

install_head_dir = \
	/usr/include/djctool/

libs = \
	-lpthread

target_flag = \
	-w

flag = \
	-Wall									\
	-Werror									\
	-Wno-error=format-security

debug_flag = \
	-p										\
	-g3										\
	-Wall									\
	-Werror									\

src = $(filter-out [g]test%, $(filter-out %-example.c, $(strip $(subst $(current), ., $(wildcard $(current)/*/*.c)))))
obj = $(strip $(patsubst %.c, %.o, $(src)))
target = $(strip $(subst $(current), ., $(patsubst %.c, %.run, $(wildcard $(current)/*/*-example.c))))

debug_obj = $(strip $(patsubst %.c, %.o_debug, $(src)))
debug_target = $(strip $(subst $(current), ., $(patsubst %.c, %.run_debug, $(wildcard $(current)/*/*-example.c))))

all:$(target) static_lib mk_dir
	@cd frame && make all
	@cd tools && make all

help:
	@echo '帮助:'
	@echo '    make help ---- 显示本帮助' 
	@echo '    make all ---- 编译所有的库文件为.a(静态读), 并把头文件进行打包'
	@echo '    make frame ---- 编译frame并打包到指定目录'
	@echo '    make demo ---- C语言一些库的例子，有些可能是图形界面相关，编译后可用单独运行'
	@echo '    make tools ---- 常用工具'
	@echo '    make install ---- 将库文件安装到/lib下，将库文件对应的头文件安装到/usr/inclde下'
	@echo '    make clean ---- 清空所有编译产生的二进制文件'

demo:
	@cd demo && make all

frame:
	@cd frame && make all

tools:
	@cd tools && make all

install:all
	@mkdir -p $(install_head_dir)
	cp $(head_dir)/* -r $(install_head_dir)
	cp $(lib_dir)/* $(install_lib_dir)
	@cd tools && make install
	@echo "install done!"

debug:$(debug_target) mk_dir_debug

static_lib: $(obj)
	ar rcs -o $(lib_name) $^

%.run_debug:%.o_debug $(debug_obj)
	cc $(debug_flag) $(heads) $(libs) -o $@ $^ $(libs) $(libs)

%.run:%.o $(obj)
	cc ${target_flag} $(heads) $(libs) -o $@ $^ $(libs)

%.o_debug:%.c
	cc $(debug_flag) $(heads) $(libs) -o $@ -c $<

%.o:%.c
	cc $(flag) $(heads) $(libs) -o $@ -c $< 

mk_dir:
	@mkdir -p $(lib_dir)
	@mkdir -p $(head_dir)
	@mkdir -p $(example_dir)
	@cp $(current)/*/*.h $(head_dir)
	@cp $(current)/*/*.run $(example_dir)
	@rm -f $(lib_dir)/*
	@mv $(lib_name) $(lib_dir)

mk_dir_debug:
	@mkdir -p $(lib_dir)
	@mkdir -p $(head_dir)
	@mkdir -p $(debug_dir)
	@cp $(current)/*/*.h $(head_dir)
	@cp $(current)/*/*.run_debug $(debug_dir)

.PHONY:clean demo tools help all frame install

clean: 
	@rm -fr $(obj)
	@rm -fr $(target)
	@rm -fr $(debug_obj)
	@rm -fr $(debug_target)
	@rm -f */*.o
	@rm -fr package/
	@cd frame && make clean
	@cd demo && make clean
	@echo "clean all!"
