CC		:= gcc -mavx2 -mfma
CFLAGS          := -O1 -w -fno-inline-functions -fno-early-inlining -fno-inline-small-functions -fno-tree-loop-optimize -fno-loop-parallelize-all 
#uncomment the following lines if running on acheron 
#CFLAGS		:= -O1 -O1 -w -fno-inline-functions -fno-early-inlining -fno-inline-small-functions -fno-tree-loop-optimize
#CC		:= gcc -msse3 


all: clean k_nearest k_nearest_seq k_nearest_simd k_nearest_thread
 
k_nearest: k_nearest.c simpletimer.c parse.c 
	$(CC) -o k_nearest k_nearest.c simpletimer.c parse.c  $(INCLUDES) $(LIBS) $(CFLAGS) $(LDFLAGS)

k_nearest_seq: k_nearest_seq.c simpletimer.c parse.c 
	$(CC) -o k_nearest_seq k_nearest_seq.c simpletimer.c parse.c $(INCLUDES) $(LIBS) $(CFLAGS) $(LDFLAGS) 

k_nearest_simd: k_nearest_simd.c simpletimer.c parse.c vec.c
	$(CC) -o k_nearest_simd k_nearest_simd.c simpletimer.c parse.c vec.c $(INCLUDES) $(LIBS) $(CFLAGS) $(LDFLAGS)

k_nearest_thread: k_nearest_thread.c simpletimer.c parse.c vec.c
	$(CC) -o k_nearest_thread k_nearest_thread.c simpletimer.c parse.c vec.c $(INCLUDES) $(LIBS) $(CFLAGS) $(LDFLAGS)

clean:
	rm -f *.o k_nearest k_nearest_seq k_nearest_simd k_nearest_thread 

