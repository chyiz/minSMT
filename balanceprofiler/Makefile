all:: install

install::
	gcc -rdynamic -Wall -shared -fpic -o balance-profiler.so balance-profiler.c busywait.c $(shell pkg-config --cflags glib-2.0) -lpthread -lrt -ldl -lglib-2.0
