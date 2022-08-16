LIBS=-lpthread -I$(XTERN_ROOT)/include \
	-L$(XTERN_ROOT)/dync_hook -Wl,--rpath,$(XTERN_ROOT)/dync_hook -lxtern-annot \
	-lpthread
CFLAGS=-std=gnu99 -g

all: unbalanced x264-model raytrace-model simple-create-join rand pthread-create balanced

unbalanced: unbalanced.c
		cc -o $@ $^ $(CFLAGS) $(LIBS)

balanced: balanced.c busywait.c
		cc -o $@ $^ $(CFLAGS) $(LIBS)

x264-model: x264-model.c
		cc -o $@ $^ $(CFLAGS) $(LIBS)

raytrace-model: raytrace-model.c
	        cc -o $@ $^ $(CFLAGS) $(LIBS)

simple-create-join: simple-create-join.c
	        cc -o $@ $^ $(CFLAGS) $(LIBS)

repeated-barriers: repeated-barriers.c
		cc -o $@ $^ $(CFLAGS) $(LIBS)

rand: rand.cpp
	        g++ -o $@ $^ $(LIBS)

pthread-create: pthread-create.c
	        cc -o $@ $^ $(CFLAGS) $(LIBS)

signal_wait: signal_wait.c busywait.c
		cc -o $@ $^ $(CFLAGS) $(LIBS)

diff_role: diff_role.c busywait.c
		cc -o $@ $^ $(CFLAGS) $(LIBS)
