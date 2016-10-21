#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <math.h>
#include <sys/time.h> 
#include "simpletimer.h"
#include "parse.h"
#include "vec.h"

/* Number of bytes in a vector */
#define VBYTES 32

/* Number of elements in a vector */
#define VSIZE VBYTES/sizeof(data_t)

/* Vector data type */
typedef data_t vec_t __attribute__ ((vector_size(VBYTES)));

typedef union {
    vec_t v;
    data_t d[VSIZE];
} pack_t;


data_t features[ROWS][FEATURE_LENGTH] __attribute__((aligned(32)));
data_t timer_ref_MD,timer_ref_ED,timer_ref_CS;
data_t timer_opt_MD,timer_opt_ED,timer_opt_CS;

data_t  abs_diff(data_t x, data_t y){
    data_t diff = x-y;
    return fabs(diff);
}

vec_t simd_abs_diff(vec_t x, vec_t y) {
  pack_t temp;
  int i;

  temp.v = x-y;
  for (i=0; i<VSIZE; i++) temp.d[i]=fabs(temp.d[i]);
  return temp.v;
}

data_t mult(data_t x,data_t y){
    data_t m = x*y;
    return m;
}

data_t manhattan_distance(data_t *x, data_t *y, int length){
    data_t distance=0;
    int i =0;
    for(i=0;i<length;i++){
        distance+=abs_diff(x[i],y[i]);
    }
    return distance;
}

data_t simd_manhattan_distance(data_t *x, data_t *y, int length){
    int i,cnt;
    pack_t temp;
    vec_t accum, diff, zero;
    data_t result=0;
    data_t *data_x=x;
    data_t *data_y=y;

    /* Initialize accum to 0 */
    for (i = 0; i < VSIZE; i++) {
        temp.d[i] = 0;
    }
    accum = temp.v;
    // What is the use of this variable?
    zero = accum;
    cnt = 0;

    /* SIMD */
    // Why use a while loop?
    while (cnt <= length-VSIZE) {
        vec_t x0 = *((vec_t *) data_x);
        vec_t y0 = *((vec_t *) data_y);

        diff = simd_abs_diff(x0, y0);
        accum += diff;

        data_x += VSIZE;
        data_y += VSIZE;
        cnt += VSIZE;
    }
    /* remainder */
    while (cnt < length) {
        result += fabs(*data_x - *data_y);
	cnt++; 
        // What does this if satement do?
	if (cnt<length) 
       		*data_x++; *data_y++;
    }

    temp.v = accum;
    for (i = 0; i < VSIZE; i++)
        result += temp.d[i];
    return result;
}
data_t squared_eucledean_distance(data_t *x,data_t *y, int length){
	data_t distance=0;
	int i = 0;
	for(i=0;i<length;i++){
		distance+= mult(abs_diff(x[i],y[i]),abs_diff(x[i],y[i]));
	}
	return distance;
}

// Edited SED function for SIMD
data_t squared_eucledean_distance_SIMD(data_t *x,data_t *y, int length){
	int i = 0;
        pack_t tempVec;
        vec_t diff;
        data_t *data_x = x, *data_y = y, result=0, abs_temp;


        for(i=0;i< VSIZE; i++){
            tempVec.d[i] = 0;
        }

        for(i=0;i<=length-VSIZE;i+=VSIZE){
            vec_t x0 = *((vec_t *) data_x);
            vec_t y0 = *((vec_t *) data_y);
            diff = simd_abs_diff(x0, y0);
            diff = diff* diff;
            tempVec.v += diff;
            data_x += VSIZE;
            data_y += VSIZE;
        }

        // remainder
        /*
        for( ;i<length; i++){
            printf("i\n");
            abs_temp = fabs(*data_x - *data_y);
            result += abs_temp * abs_temp;
            *data_x++;
            *data_y++;
        }
        */
        for(i=0;i<VSIZE;i++){
            result += tempVec.d[i];
        }
	return result;
}

data_t norm(data_t *x, int length){
    data_t n = 0;
    int i=0;
    for (i=0;i<length;i++){
        n += mult(x[i],x[i]);
    }
    n = sqrt(n);
    return n;
}

data_t cosine_similarity(data_t *x, data_t *y, int length){
    data_t sim=0;
    int i=0;
    for(i=0;i<length;i++){
        sim += mult(x[i],y[i]);
    }
    sim = sim / mult(norm(x,FEATURE_LENGTH),norm(y,FEATURE_LENGTH));
    return sim;
}


data_t *ref_classify_MD(unsigned int lookFor, unsigned int *found) {
    data_t *result =(data_t*)malloc(sizeof(data_t)*(ROWS-1));
    struct timeval stv, etv;
    int i,closest_point=0;
    data_t min_distance,current_distance;

	timer_start(&stv);
	min_distance = manhattan_distance(features[lookFor],features[0],FEATURE_LENGTH);
    	result[0] = min_distance;
	for(i=1;i<ROWS-1;i++){
		current_distance = manhattan_distance(features[lookFor],features[i],FEATURE_LENGTH);
        	result[i]=current_distance;
		if(current_distance<min_distance){
			min_distance=current_distance;
			closest_point=i;
		}
	}
    timer_ref_MD = timer_end(stv);
    printf("Calculation using reference MD took: %10.6f \n", timer_ref_MD);
    *found=closest_point;
    return result;
}

//Modify this function
data_t *opt_classify_MD(unsigned int lookFor, unsigned int *found) {
    data_t *result =(data_t*)malloc(sizeof(data_t)*(ROWS-1));
    struct timeval stv, etv;
    int i,closest_point=0;
    data_t min_distance,current_distance;

        timer_start(&stv);
        min_distance = simd_manhattan_distance(features[lookFor],features[0],FEATURE_LENGTH);
    	result[0] = min_distance;
        for(i=1;i<ROWS-1;i++){
                current_distance = simd_manhattan_distance(features[lookFor],features[i],FEATURE_LENGTH);
                result[i]=current_distance;
                if(current_distance<min_distance){
                        min_distance=current_distance;
                        closest_point=i;
                }
        }
    timer_opt_MD = timer_end(stv);
    printf("Calculation using optimized MD took: %10.6f \n", timer_opt_MD);
    *found = closest_point;
    return result;
}

//Don't touch this function
data_t *ref_classify_ED(unsigned int lookFor, unsigned int *found) {
    data_t *result =(data_t*)malloc(sizeof(data_t)*(ROWS-1));
    struct timeval stv, etv;
    int i,closest_point=0;
    data_t min_distance,current_distance;

	timer_start(&stv);
	min_distance = squared_eucledean_distance(features[lookFor],features[0],FEATURE_LENGTH);
    	result[0] = min_distance;
	for(i=1;i<ROWS-1;i++){
		current_distance = squared_eucledean_distance(features[lookFor],features[i],FEATURE_LENGTH);
        	result[i]=current_distance;
		if(current_distance<min_distance){
			min_distance=current_distance;
			closest_point=i;
		}
	}

    timer_ref_ED = timer_end(stv);
    printf("Calculation using reference ED took: %10.6f \n", timer_ref_ED);
    *found = closest_point;
    return result;
}

//Modify this function
data_t *opt_classify_ED(unsigned int lookFor, unsigned int *found) {
    data_t *result =(data_t*)malloc(sizeof(data_t)*(ROWS-1));
    struct timeval stv, etv;
    int i,j,closest_point=0, init, cnt;
    pack_t tempVec;
    vec_t diff0, diff1, diff2, diff3, x0, x1, x2, x3, y0, y1, y2, y3;
    data_t min_distance,current_distance, abs_diff_temp0, abs_diff_temp1, abs_diff_temp2, abs_diff_temp3;
    data_t tempR0=0, tempR1=0, tempR2=0, tempR3=0, *data_x, *data_y;


	timer_start(&stv);
    //FROM HERE
        min_distance = squared_eucledean_distance_SIMD(features[lookFor],features[0],FEATURE_LENGTH);
    	result[0] = min_distance;

        for(i=1;i<ROWS-1;i++){
            data_x = features[lookFor];
            data_y = features[i];
            // Initiaize the counter to 0
            current_distance=0;
            for(j=0;j<VSIZE;j++){
                tempVec.d[j]=0;
            }

            // Main SIMD operation
            for(j=0;j<=FEATURE_LENGTH-VSIZE; j+=VSIZE){
                x0 = *((vec_t *) data_x);
                y0 = *((vec_t *) data_y);
                 /*
                x1 = *((vec_t *) (data_x+VSIZE));
                y1 = *((vec_t *) (data_y+VSIZE));
                x2 = *((vec_t *) (data_x+VSIZE*2));
                y2 = *((vec_t *) (data_y+VSIZE*2));
                x3 = *((vec_t *) (data_x+VSIZE*3));
                y3 = *((vec_t *) (data_y+VSIZE*3));
                // */
                diff0 = simd_abs_diff(x0, y0);
                 /*
                diff1 = simd_abs_diff(x1, y1);
                diff2 = simd_abs_diff(x2, y2);
                diff3 = simd_abs_diff(x3, y3);
                // */
                diff0 = diff0* diff0;
                 /*
                diff1 = diff1* diff1;
                diff2 = diff2* diff2;
                diff3 = diff3* diff3;
                // */
                tempVec.v += diff0;
                 /*
                tempVec.v += diff1;
                tempVec.v += diff2;
                tempVec.v += diff3;
                // */

                data_x += VSIZE;
                data_y += VSIZE;
            }
            for(j=0;j<VSIZE;j++){
                current_distance += tempVec.d[j];
            }


	    //current_distance = squared_eucledean_distance_SIMD(features[lookFor],features[i],FEATURE_LENGTH);
            result[i]=current_distance;
	    if(current_distance<min_distance){
		min_distance=current_distance;
		closest_point=i;
	    }
        }

    //TO HERE
    timer_opt_ED = timer_end(stv);
    printf("Calculation using optimized ED took: %10.6f \n", timer_opt_ED);
    *found = closest_point;
    return result;
}


//Don't touch this function
data_t *ref_classify_CS(unsigned int lookFor, unsigned int* found) {
    data_t *result =(data_t*)malloc(sizeof(data_t)*(ROWS-1));
    struct timeval stv, etv;
    int i,closest_point=0;
    data_t min_distance,current_distance;

	timer_start(&stv);
	min_distance = cosine_similarity(features[lookFor],features[0],FEATURE_LENGTH);
    	result[0] = min_distance;
	for(i=1;i<ROWS-1;i++){
		current_distance = cosine_similarity(features[lookFor],features[i],FEATURE_LENGTH);
        	result[i]=current_distance;
		if(current_distance<min_distance){
			min_distance=current_distance;
			closest_point=i;
		}
	}
    timer_ref_CS = timer_end(stv);
    printf("Calculation using reference CS took: %10.6f \n", timer_ref_CS);
    *found = closest_point;
    return result;
}

//Modify this function 
data_t *opt_classify_CS(unsigned int lookFor, unsigned int *found) {
    data_t *result =(data_t*)malloc(sizeof(data_t)*(ROWS-1));
    struct timeval stv, etv;
    int i,closest_point=0;
    data_t min_distance,temp_dist0, temp_dist1, temp_dist2, temp_dist3, temp_dist4, temp_dist5, temp_dist6;

    timer_start(&stv);

    //MODIFY FROM HERE
	min_distance = cosine_similarity(features[lookFor],features[0],FEATURE_LENGTH);
    	result[0] = min_distance;
	for(i=0;i<ROWS-7;i+=7) {
		temp_dist0 = cosine_similarity(features[lookFor],features[i],FEATURE_LENGTH);
		temp_dist1 = cosine_similarity(features[lookFor],features[i+1],FEATURE_LENGTH);
		temp_dist2 = cosine_similarity(features[lookFor],features[i+2],FEATURE_LENGTH);
		temp_dist3 = cosine_similarity(features[lookFor],features[i+3],FEATURE_LENGTH);
		temp_dist4 = cosine_similarity(features[lookFor],features[i+4],FEATURE_LENGTH);
		temp_dist5 = cosine_similarity(features[lookFor],features[i+5],FEATURE_LENGTH);
		temp_dist6 = cosine_similarity(features[lookFor],features[i+6],FEATURE_LENGTH);
        	result[i]=temp_dist0;
        	result[i+1]=temp_dist1;
        	result[i+2]=temp_dist2;
        	result[i+3]=temp_dist3;
        	result[i+4]=temp_dist4;
        	result[i+5]=temp_dist5;
        	result[i+6]=temp_dist6;


		if(temp_dist0<min_distance){
			min_distance=temp_dist0;
			closest_point=i;
		}
		if(temp_dist1<min_distance){
			min_distance=temp_dist1;
			closest_point=i+1;
		}
		if(temp_dist2<min_distance){
			min_distance=temp_dist2;
			closest_point=i+2;
		}
		if(temp_dist3<min_distance){
			min_distance=temp_dist3;
			closest_point=i+3;
		}
		if(temp_dist4<min_distance){
			min_distance=temp_dist4;
			closest_point=i+4;
		}
		if(temp_dist5<min_distance){
			min_distance=temp_dist5;
			closest_point=i+5;
		}
		if(temp_dist6<min_distance){
			min_distance=temp_dist6;
			closest_point=i+6;
		}
	}
    //TO HERE
    timer_opt_CS = timer_end(stv);
    printf("Calculation using oprimized CS took: %10.6f \n", timer_opt_CS);
    *found = closest_point;
    return result;
}

typedef data_t (*(*classifying_funct)(unsigned int lookFor, unsigned int* found));

int check_correctness(classifying_funct a, classifying_funct b, unsigned int lookFor, unsigned int *found) {
    unsigned int r=1, i, a_found, b_found;
    data_t *a_res = a(lookFor, &a_found);
    data_t *b_res = b(lookFor, &b_found);
    
    for(i=0;i<ROWS-1;i++)
        if(fabs(a_res[i]-b_res[i])>0.001) {
           return 0;		
 	}
    if (a_found != b_found) return 0;
    *found=a_found;
    return 1; 
}

int main(int argc, char **argv){
	char* dataset_name=DATASET;
	int i,j;
        struct timeval stv, etv;
	unsigned int lookFor=ROWS-1, located;     
	//PARSE CSV
	
	//holds the information regarding author and title
	char metadata[ROWS][2][20];

	timer_start(&stv); 	
	parse_csv(dataset_name, features, metadata); 
	printf("Parsing took %9.6f s \n\n", timer_end(stv));	 

    printf("Classifying using MD:");
    printf("<Record %d, author =\"%s\", title=\"%s\">\n",lookFor,metadata[lookFor][0],metadata[lookFor][1]);
    if(check_correctness(ref_classify_MD,opt_classify_MD, lookFor, &located)){
        printf("opt_classify_MD is correct, speedup: %10.6f\n\n",timer_ref_MD/timer_opt_MD);        
    }
    else
        printf("opt_classify_MD is incorrect! \n"); // , speedup: %10.6f\n\n",timer_ref_MD/timer_opt_MD);
    printf("Best match: ");
    printf("<Record %d, author =\"%s\", title=\"%s\">\n\n",located,metadata[located][0],metadata[located][1]);

    printf("Classifying using ED:");
    printf("<Record %d, author =\"%s\", title=\"%s\">\n",lookFor,metadata[lookFor][0],metadata[lookFor][1]);
    if(check_correctness(ref_classify_ED,opt_classify_ED, lookFor, &located)) {
        printf("opt_classify_ED is correct, speedup: %10.6f\n\n",timer_ref_ED/timer_opt_ED);        
    }
    else
        printf("opt_classify_ED id incorrect!\n\n");
    printf("Best match: ");
    printf("<Record %d, author =\"%s\", title=\"%s\">\n\n",located,metadata[located][0],metadata[located][1]);

    printf("Classifying using CS (cosine similarity):");
    printf("<Record %d, author =\"%s\", title=\"%s\">\n",lookFor,metadata[lookFor][0],metadata[lookFor][1]);
    if(check_correctness(ref_classify_CS,opt_classify_CS, lookFor, &located)) {
        printf("opt_classify_CS is correct, speedup: %10.6f\n\n",timer_ref_CS/timer_opt_CS);        
    }
    else
        printf("opt_classify_CS id incorrect!\n\n");
    printf("Best match: ");
    printf("<Record %d, author =\"%s\", title=\"%s\">\n\n",located,metadata[located][0],metadata[located][1]);

}
