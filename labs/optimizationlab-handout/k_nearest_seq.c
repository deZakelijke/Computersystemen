#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <math.h>
#include <sys/time.h> 
#include "simpletimer.h"
#include "parse.h"
#include "vec.h"

data_t features[ROWS][FEATURE_LENGTH] __attribute__((aligned(32)));
data_t timer_ref_MD,timer_ref_ED,timer_ref_CS;
data_t timer_opt_MD,timer_opt_ED,timer_opt_CS;

data_t abs_diff(data_t x, data_t y){
    data_t diff = x-y;
    return fabs(diff);
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

data_t squared_eucledean_distance(data_t *x,data_t *y, int length){
	data_t distance=0;
        data_t abs_diff_xy;
	int i = 0;
	for(i=0;i<length;i++){
		distance+= mult(abs_diff(x[i],y[i]),abs_diff(x[i],y[i]));
	}
	return distance;
}
data_t squared_eucledean_distance_better(data_t *x,data_t *y, int length){
	data_t distance=0;
	int i = 0;
        data_t abs_diff_xy;

	for(i=0;i<length;i++){
                abs_diff_xy = fabs(x[i]-y[i]);
		distance+= abs_diff_xy*abs_diff_xy;
	}
	return distance;
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
    data_t sim=0, norm_x=0, norm_y=0;
    int i=0;
    for(i=0;i<length;i++){
        sim += mult(x[i],y[i]);
    }
    sim = sim / mult(norm(x,FEATURE_LENGTH),norm(y,FEATURE_LENGTH));
    return sim;
}

// New funtion
data_t cosine_similarity_better(data_t *x, data_t *y, int length, data_t norm_x){
    data_t sim=0, norm_y=0;
    int i=0;
    for(i=0;i<length;i++){
        sim += x[i]*y[i];
        norm_y += y[i]*y[i];
    }
    norm_y = sqrt(norm_y);
    //sim = sim / mult(norm(x,FEATURE_LENGTH),norm(y,FEATURE_LENGTH));
    sim = sim  / (norm_x * norm_y);
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
    int i,j,closest_point=0, mask = 0x7FFFFFFF; 
    data_t min_distance;
    data_t abs_diff_temp0, abs_diff_temp1, abs_diff_temp2, abs_diff_temp3;
    data_t abs_diff_temp4, abs_diff_temp5, abs_diff_temp6, abs_diff_temp7;
    data_t abs_diff_temp8, abs_diff_temp9, abs_diff_temp10;
    data_t tempR0=0, tempR1=0, tempR2=0, tempR3=0;
    data_t tempR4=0, tempR5=0, tempR6=0, tempR7=0;
    data_t tempR8=0, tempR9=0, tempR10=0;

    /* not used, but variable still present in code that is not used
    int tmp_cl=0;
    data_t temp_min;
    */
	timer_start(&stv);
    //FROM HERE
	min_distance = squared_eucledean_distance_better(features[lookFor],features[0],FEATURE_LENGTH);
    	result[0] = min_distance;
	for(i=0;i<ROWS-7;i+=7){
                for(j=0;j<FEATURE_LENGTH;j+=4){
                    // Tried to eliminate fabs() by using a bitwise operation.
                    //abs_diff_temp = data_t*((int(features[lookFor][j]-features[i][j])&mask)); 
                    abs_diff_temp0 = fabs(features[lookFor][j]-features[i][j]);
                    abs_diff_temp1 = fabs(features[lookFor][j]-features[i+1][j]);
                    abs_diff_temp2 = fabs(features[lookFor][j]-features[i+2][j]);
                    abs_diff_temp3 = fabs(features[lookFor][j]-features[i+3][j]);
	            abs_diff_temp4 = fabs(features[lookFor][j]-features[i+4][j]);
                    abs_diff_temp5 = fabs(features[lookFor][j]-features[i+5][j]);
                    abs_diff_temp6 = fabs(features[lookFor][j]-features[i+6][j]);


                    /*
                    Tried to unroll to 11
                    abs_diff_temp7 = fabs(features[lookFor][j]-features[i+7][j]);
                    abs_diff_temp8 = fabs(features[lookFor][j]-features[i+8][j]);
                    abs_diff_temp9 = fabs(features[lookFor][j]-features[i+9][j]);
                    abs_diff_temp10 = fabs(features[lookFor][j]-features[i+10][j]);
                    */
                    tempR0+= abs_diff_temp0*abs_diff_temp0;
		    tempR1+= abs_diff_temp1*abs_diff_temp1;
		    tempR2+= abs_diff_temp2*abs_diff_temp2;
		    tempR3+= abs_diff_temp3*abs_diff_temp3;
                    tempR4+= abs_diff_temp4*abs_diff_temp4;
                    tempR5+= abs_diff_temp5*abs_diff_temp5;
		    tempR6+= abs_diff_temp6*abs_diff_temp6;
                    /*
		    tempR7+= abs_diff_temp7*abs_diff_temp7;
                    tempR8+= abs_diff_temp8*abs_diff_temp8;
		    tempR9+= abs_diff_temp9*abs_diff_temp9;
		    tempR10+= abs_diff_temp10*abs_diff_temp10;
                    */

                    // Unroll the inner loop
                    abs_diff_temp0 = fabs(features[lookFor][j+1]-features[i][j+1]);
                    abs_diff_temp1 = fabs(features[lookFor][j+1]-features[i+1][j+1]);
                    abs_diff_temp2 = fabs(features[lookFor][j+1]-features[i+2][j+1]);
                    abs_diff_temp3 = fabs(features[lookFor][j+1]-features[i+3][j+1]);
	            abs_diff_temp4 = fabs(features[lookFor][j+1]-features[i+4][j+1]);
                    abs_diff_temp5 = fabs(features[lookFor][j+1]-features[i+5][j+1]);
                    abs_diff_temp6 = fabs(features[lookFor][j+1]-features[i+6][j+1]);
                    tempR0+= abs_diff_temp0*abs_diff_temp0;
		    tempR1+= abs_diff_temp1*abs_diff_temp1;
		    tempR2+= abs_diff_temp2*abs_diff_temp2;
		    tempR3+= abs_diff_temp3*abs_diff_temp3;
                    tempR4+= abs_diff_temp4*abs_diff_temp4;
                    tempR5+= abs_diff_temp5*abs_diff_temp5;
		    tempR6+= abs_diff_temp6*abs_diff_temp6;
                    abs_diff_temp0 = fabs(features[lookFor][j+2]-features[i][j+2]);
                    abs_diff_temp1 = fabs(features[lookFor][j+2]-features[i+1][j+2]);
                    abs_diff_temp2 = fabs(features[lookFor][j+2]-features[i+2][j+2]);
                    abs_diff_temp3 = fabs(features[lookFor][j+2]-features[i+3][j+2]);
	            abs_diff_temp4 = fabs(features[lookFor][j+2]-features[i+4][j+2]);
                    abs_diff_temp5 = fabs(features[lookFor][j+2]-features[i+5][j+2]);
                    abs_diff_temp6 = fabs(features[lookFor][j+2]-features[i+6][j+2]);
                    tempR0+= abs_diff_temp0*abs_diff_temp0;
		    tempR1+= abs_diff_temp1*abs_diff_temp1;
		    tempR2+= abs_diff_temp2*abs_diff_temp2;
		    tempR3+= abs_diff_temp3*abs_diff_temp3;
                    tempR4+= abs_diff_temp4*abs_diff_temp4;
                    tempR5+= abs_diff_temp5*abs_diff_temp5;
		    tempR6+= abs_diff_temp6*abs_diff_temp6;
                    abs_diff_temp0 = fabs(features[lookFor][j+3]-features[i][j+3]);
                    abs_diff_temp1 = fabs(features[lookFor][j+3]-features[i+1][j+3]);
                    abs_diff_temp2 = fabs(features[lookFor][j+3]-features[i+2][j+3]);
                    abs_diff_temp3 = fabs(features[lookFor][j+3]-features[i+3][j+3]);
	            abs_diff_temp4 = fabs(features[lookFor][j+3]-features[i+4][j+3]);
                    abs_diff_temp5 = fabs(features[lookFor][j+3]-features[i+5][j+3]);
                    abs_diff_temp6 = fabs(features[lookFor][j+3]-features[i+6][j+3]);
                    tempR0+= abs_diff_temp0*abs_diff_temp0;
		    tempR1+= abs_diff_temp1*abs_diff_temp1;
		    tempR2+= abs_diff_temp2*abs_diff_temp2;
		    tempR3+= abs_diff_temp3*abs_diff_temp3;
                    tempR4+= abs_diff_temp4*abs_diff_temp4;
                    tempR5+= abs_diff_temp5*abs_diff_temp5;
		    tempR6+= abs_diff_temp6*abs_diff_temp6;

	        }
                result[i] = tempR0;
                result[i+1] = tempR1;
                result[i+2] = tempR2;
                result[i+3] = tempR3;
                result[i+4] = tempR4;
                result[i+5] = tempR5;
                result[i+6] = tempR6;
                /*
                result[i+7] = tempR7;
                result[i+8] = tempR8;
                result[i+9] = tempR9;
                result[i+10] = tempR10;
                */
                tempR0=0, tempR1=0, tempR2=0, tempR3=0;
                tempR4=0, tempR5=0, tempR6=0;
                tempR7=0, tempR8=0, tempR9=0, tempR10=0;
		//result[i]=squared_eucledean_distance_better(features[lookFor],features[i],FEATURE_LENGTH);
                /*
                if(result[i]<result[i+1]){
                    temp_min = result[i];
                    closest_point = i;
                    tmp_cl = i;
                }else{
                    temp_min = result[i+1];
                    closest_point = i+1;
                    tmp_cl = i+1;
                }
                if(result[i+2]<min_distance){
                    min_distance = result[i+2];
                    closest_point = i+2;
                }
                if(result[i+3]<temp_min){
                    temp_min = result[i+3];
                    tmp_cl = i+3;
                }
                if(temp_min<min_distance){
                    min_distance = temp_min;
                    closest_point = tmp_cl;
                }
                */
                if(result[i]<min_distance){
			min_distance=result[i];
			closest_point=i;
		}
		if(result[i+1]<min_distance){
			min_distance=result[i+1];
			closest_point=i+1;
		}
		if(result[i+2]<min_distance){
			min_distance=result[i+2];
			closest_point=i+2;
		}
		if(result[i+3]<min_distance){
			min_distance=result[i+3];
			closest_point=i+3;
		}
                if(result[i+4]<min_distance){
			min_distance=result[i+4];
			closest_point=i+4;
		}
		if(result[i+5]<min_distance){
			min_distance=result[i+5];
			closest_point=i+5;
		}
		if(result[i+6]<min_distance){
			min_distance=result[i+6];
			closest_point=i+6;
		}
                /*
		if(result[i+7]<min_distance){
			min_distance=result[i+7];
			closest_point=i+7;
		}
		if(result[i+8]<min_distance){
			min_distance=result[i+8];
			closest_point=i+8;
		}
		if(result[i+9]<min_distance){
			min_distance=result[i+9];
			closest_point=i+9;
		}
		if(result[i+10]<min_distance){
			min_distance=result[i+10];
			closest_point=i+10;
		}
                */
	}
        /*
        Used to wrap up the unrolled loop
        for (i-=(ROWS%11)-1;i<ROWS-1;i++){
            printf("i: %d\n",i);
                result[i] = 0;
                for(j=0;j<FEATURE_LENGTH;j++){
                    abs_diff_temp0 = fabs(features[lookFor][j]-features[i][j]);
		    result[i]+= abs_diff_temp0*abs_diff_temp0;
                }
		//result[i]= squared_eucledean_distance_better(features[lookFor],features[i],FEATURE_LENGTH);
		if(result[i]<min_distance){
			min_distance=result[i];
			closest_point=i;
		}
        }
        */
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
    int i, j, closest_point=0;
    data_t min_distance,temp_dist0, temp_dist1, temp_dist2, temp_dist3, temp_dist4, temp_dist5, temp_dist6;
    data_t feat_norm=0;
    data_t temp_sim0, temp_sim1, temp_sim2, temp_sim3, temp_sim4, temp_sim5, temp_sim6;
    data_t temp_norm0, temp_norm1, temp_norm2, temp_norm3, temp_norm4, temp_norm5, temp_norm6;

    timer_start(&stv);

    //MODIFY FROM HERE
        for(i=0;i< FEATURE_LENGTH; i++){
            feat_norm += features[lookFor][i] * features[lookFor][i];
        }
        feat_norm = sqrt(feat_norm);

	min_distance = cosine_similarity_better(features[lookFor],features[0],FEATURE_LENGTH, feat_norm);
    	result[0] = min_distance;
	for(i=0;i<ROWS-7;i+=7) {
                temp_sim0=0, temp_sim1=0, temp_sim2=0, temp_sim3=0;
                temp_sim4=0, temp_sim5=0, temp_sim6=0;
                temp_norm0=0, temp_norm1=0, temp_norm2=0, temp_norm3=0;
                temp_norm4=0, temp_norm5=0, temp_norm6=0;

                for(j=0;j<FEATURE_LENGTH-3;j+=4){
                    temp_sim0 += features[lookFor][j] * features[i][j];
                    temp_norm0 += features[i][j] * features[i][j];
                    temp_sim1 += features[lookFor][j] * features[i+1][j];
                    temp_norm1 += features[i+1][j] * features[i+1][j];
                    temp_sim2 += features[lookFor][j] * features[i+2][j];
                    temp_norm2 += features[i+2][j] * features[i+2][j];
                    temp_sim3 += features[lookFor][j] * features[i+3][j];
                    temp_norm3 += features[i+3][j] * features[i+3][j];
                    temp_sim4 += features[lookFor][j] * features[i+4][j];
                    temp_norm4 += features[i+4][j] * features[i+4][j];
                    temp_sim5 += features[lookFor][j] * features[i+5][j];
                    temp_norm5 += features[i+5][j] * features[i+5][j];
                    temp_sim6 += features[lookFor][j] * features[i+6][j];
                    temp_norm6 += features[i+6][j] * features[i+6][j];
                    
                    temp_sim0 += features[lookFor][j+1] * features[i][j+1];
                    temp_norm0 += features[i][j+1] * features[i][j+1];
                    temp_sim1 += features[lookFor][j+1] * features[i+1][j+1];
                    temp_norm1 += features[i+1][j+1] * features[i+1][j+1];
                    temp_sim2 += features[lookFor][j+1] * features[i+2][j+1];
                    temp_norm2 += features[i+2][j+1] * features[i+2][j+1];
                    temp_sim3 += features[lookFor][j+1] * features[i+3][j+1];
                    temp_norm3 += features[i+3][j+1] * features[i+3][j+1];
                    temp_sim4 += features[lookFor][j+1] * features[i+4][j+1];
                    temp_norm4 += features[i+4][j+1] * features[i+4][j+1];
                    temp_sim5 += features[lookFor][j+1] * features[i+5][j+1];
                    temp_norm5 += features[i+5][j+1] * features[i+5][j+1];
                    temp_sim6 += features[lookFor][j+1] * features[i+6][j+1];
                    temp_norm6 += features[i+6][j+1] * features[i+6][j+1];

                    temp_sim0 += features[lookFor][j+2] * features[i][j+2];
                    temp_norm0 += features[i][j+2] * features[i][j+2];
                    temp_sim1 += features[lookFor][j+2] * features[i+1][j+2];
                    temp_norm1 += features[i+1][j+2] * features[i+1][j+2];
                    temp_sim2 += features[lookFor][j+2] * features[i+2][j+2];
                    temp_norm2 += features[i+2][j+2] * features[i+2][j+2];
                    temp_sim3 += features[lookFor][j+2] * features[i+3][j+2];
                    temp_norm3 += features[i+3][j+2] * features[i+3][j+2];
                    temp_sim4 += features[lookFor][j+2] * features[i+4][j+2];
                    temp_norm4 += features[i+4][j+2] * features[i+4][j+2];
                    temp_sim5 += features[lookFor][j+2] * features[i+5][j+2];
                    temp_norm5 += features[i+5][j+2] * features[i+5][j+2];
                    temp_sim6 += features[lookFor][j+2] * features[i+6][j+2];
                    temp_norm6 += features[i+6][j+2] * features[i+6][j+2];

                    temp_sim0 += features[lookFor][j+3] * features[i][j+3];
                    temp_norm0 += features[i][j+3] * features[i][j+3];
                    temp_sim1 += features[lookFor][j+3] * features[i+1][j+3];
                    temp_norm1 += features[i+1][j+3] * features[i+1][j+3];
                    temp_sim2 += features[lookFor][j+3] * features[i+2][j+3];
                    temp_norm2 += features[i+2][j+3] * features[i+2][j+3];
                    temp_sim3 += features[lookFor][j+3] * features[i+3][j+3];
                    temp_norm3 += features[i+3][j+3] * features[i+3][j+3];
                    temp_sim4 += features[lookFor][j+3] * features[i+4][j+3];
                    temp_norm4 += features[i+4][j+3] * features[i+4][j+3];
                    temp_sim5 += features[lookFor][j+3] * features[i+5][j+3];
                    temp_norm5 += features[i+5][j+3] * features[i+5][j+3];
                    temp_sim6 += features[lookFor][j+3] * features[i+6][j+3];
                    temp_norm6 += features[i+6][j+3] * features[i+6][j+3];

                }
                temp_norm0 = sqrt(temp_norm0);
                temp_dist0 = temp_sim0 / (feat_norm * temp_norm0);
                temp_norm1 = sqrt(temp_norm1);
                temp_dist1 = temp_sim1 / (feat_norm * temp_norm1);
                temp_norm2 = sqrt(temp_norm2);
                temp_dist2 = temp_sim2 / (feat_norm * temp_norm2);
                temp_norm3 = sqrt(temp_norm3);
                temp_dist3 = temp_sim3 / (feat_norm * temp_norm3);
                temp_norm4 = sqrt(temp_norm4);
                temp_dist4 = temp_sim4 / (feat_norm * temp_norm4);
                temp_norm5 = sqrt(temp_norm5);
                temp_dist5 = temp_sim5 / (feat_norm * temp_norm5);
                temp_norm6 = sqrt(temp_norm6);
                temp_dist6 = temp_sim6 / (feat_norm * temp_norm6);

		/*
                temp_dist0 = cosine_similarity_better(features[lookFor],features[i],FEATURE_LENGTH, feat_norm);
		temp_dist1 = cosine_similarity_better(features[lookFor],features[i+1],FEATURE_LENGTH, feat_norm);
		temp_dist2 = cosine_similarity_better(features[lookFor],features[i+2],FEATURE_LENGTH, feat_norm);
		temp_dist3 = cosine_similarity_better(features[lookFor],features[i+3],FEATURE_LENGTH, feat_norm);
		temp_dist4 = cosine_similarity_better(features[lookFor],features[i+4],FEATURE_LENGTH, feat_norm);
		temp_dist5 = cosine_similarity_better(features[lookFor],features[i+5],FEATURE_LENGTH, feat_norm);
		temp_dist6 = cosine_similarity_better(features[lookFor],features[i+6],FEATURE_LENGTH, feat_norm);
                */

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
    unsigned int i, a_found, b_found;
    data_t *a_res = a(lookFor, &a_found);
    data_t *b_res = b(lookFor, &b_found);
    
    for(i=0;i<ROWS-1;i++){
        if(a_res[i]!=b_res[i])
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
