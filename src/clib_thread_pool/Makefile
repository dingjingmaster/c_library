CC = g++

OBJS = ./src/cthread_pool_home.o 	\
			 ./test_main.o

LIBS = -lpthread

FLAG = -g


target = test_thread_pool

$(target) : $(OBJS)
	$(CC) -o $@ $^ $(LIBS) $(FLAG)

%.o : %.c
	$(CC) -o $@ -c $< $(LIBS) $(FLAG)

.PHONY : clean

clean : 
	rm -fr $(OBJS)
	rm -fr $(target)
	clear
