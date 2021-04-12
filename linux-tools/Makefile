curDir = $(shell pwd)
bin = $(wildcard $(curDir)/linux-tools-*)

all:
	@echo "make all"

install:
	@install -m a+rx $(bin) "/usr/bin/"
	

.PHONY:all install clean

clean: 
