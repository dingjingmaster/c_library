CC = gcc
CPP = g++
STATIC_LIB = ar

LIB_OBJ = clib_list.o
LIST_TEST = clib_list_test.o
LIST_Demo = clib_list_test

LIB_PATH = -L "./"
HEAD_PATH = -I "./"
LIBS = -l "list"

TARGET_LIB = liblist.a


all : $(TARGET_LIB) 		\
				$(LIST_Demo)

$(TARGET_LIB) : $(LIB_OBJ)
	$(STATIC_LIB) rs $@ $^
	

$(LIST_Demo) : $(LIST_TEST)
	$(CC) -o $@ $^ $(LIBS) $(HEAD_PATH) $(LIB_PATH)

%.o : %.c
	$(CC) -o $@ -c $<

.PHONY : clean

clean:
	rm -fr $(LIB_OBJ)
	rm -fr $(LIST_TEST)
	rm -fr $(LIST_Demo)
	rm -fr $(TARGET_LIB)

