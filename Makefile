CC = gcc
CPP = g++
CH_DIR = cd
CP = cp

CUR_DIR = $(shell pwd)
LIB_DIR = $(CUR_DIR)/lib/
HEAD_DIR = $(CUR_DIR)/head/
DEMO_DIR = $(CUR_DIR)/demo/

LIB_LIST_DIR = $(CUR_DIR)/src/clib_list/
LIB_STRING_DIR = $(CUR_DIR)/src/clib_string/
LIB_TREE_DIR = $(CUR_DIR)/src/clib_tree/

OBJS = $(LIB_LIST_DIR) 					\
			 $(LIB_STRING_DIR) 				\
			 $(LIB_TREE_DIR)


all : mk_dir libs



mk_dir :
	mkdir $(LIB_DIR)
	mkdir $(HEAD_DIR)
	mkdir $(DEMO_DIR)


libs : $(OBJS)
	$(CH_DIR) $< && make
	$(CP) $</*.a $(LIB_DIR)
	$(CP) $</*.h $(HEAD_DIR)
	$(CP) $</*.out $(DEMO_DIR)



.PHONY : clean

clean: makeClean
	rm -fr $(LIB_DIR)
	rm -fr $(HEAD_DIR)
	rm -fr $(DEMO_DIR)


makeClean : $(OBJS)
	$(CH_DIR) $< && make clean



