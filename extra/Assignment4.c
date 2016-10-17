#include <stdio.h>
#include <stdlib.h>

#define MAXELEMS 10000000
// One million
long data[MAXELEMS];  
/* Global array to traverse */
/* test - Iterate over first "
elems" elements of array “data” 
with stride of "stride", using 
using 4x4 loop unrolling.
 */
int test(int elems, int stride) {
	long i, sx2=stride*2, sx3=stride*3, sx4=stride*4;
	long acc0 = 0, acc1 = 0, acc2 = 0, acc3 = 0;
	long length = elems, limit = length - sx4;
/* Combine 4 elements at a time */

	for (i = 0; i < limit; i += sx4) {
        acc0 = acc0 + data[i];
        acc1 = acc1 + data[i+stride];
        acc2 = acc2 + data[i+sx2];
        acc3 = acc3 + data[i+sx3];
    }
/* 
Finish any
remaining elements
 */
	for ( ; i < length; i++) {
        acc0 = acc0 + data[i];
    }
	return((acc0 + acc1) + (acc2 + acc3));
}


int main(){
    int elems=MAXELEMS, stride=1, testvalue;

    for (int i=0; i<MAXELEMS; i++){
        data[i]=i;
    }
    while (stride < MAXELEMS/4){
        testvalue = test(elems, stride);
        printf("testvalue is: %d \n", testvalue);
        stride = stride*2;
        elems = elems/2;
    }
}
