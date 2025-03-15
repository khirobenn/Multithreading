#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <pthread.h>

#ifndef N
#define N (1 << 27) /* 1 décalé gauche de 27, donc 2^27 */
#endif

#ifndef K
#define K (1 << 27) /* 1 décalé gauche de 27, donc 2^27 */
#endif

#define PAS (1. / K)

double calculer_pi(long n);
double f(double x);
void *calculer_segment(void *arg);

struct segment {
	double a;
	double b;
	double result;
};

int main(int argc, char *argv[])
{
	struct timespec tic, toc;
	double duree, pi;

	// Ma partie : 
	struct segment *tab_de_segment[K];
	pthread_t tab_de_threads[K];
	pi = 0;
	for(int i = 0 ; i < K; i++){
		tab_de_segment[i] = malloc(sizeof(struct segment));
		tab_de_segment[i]->a = (double) (i) * PAS;
		tab_de_segment[i]->b = (double)(i+1) * PAS;
		pthread_create(&tab_de_threads[i], NULL, calculer_segment, &tab_de_segment[i]);
	}
	
	clock_gettime(CLOCK_REALTIME, &tic);
	for(int i = 0; i < K; i++){
		pthread_join(tab_de_threads[i], NULL);

		// printf("%d : %lf, %lf == %lf\n",
		// 		i,
		// 	 	tab_de_segment[i]->a,
		// 		tab_de_segment[i]->b,
		// 		tab_de_segment[i]->result);

		pi += tab_de_segment[i]->result;
		free(tab_de_segment[i]);
	}
	pi *= 4./K;

	// Fin de ma partie

	clock_gettime(CLOCK_REALTIME, &toc);
	duree = (toc.tv_sec - tic.tv_sec);
	duree += (toc.tv_nsec - tic.tv_nsec) / 1000000000.0;

	printf("pi = %.10f\n", pi);
	printf("PI = %.10f\n", atan(1.) * 4.);
	printf("durée : %g\n", duree);
	return 0;
}

double f(double x)
{
	return sqrt(1 - x * x);
}

double calculer_pi(long n)
{
	double x = 0.,  res = 0.;
	for (; x < 1 - PAS; x += PAS)
		res += (f(x) + f(x + PAS)) / 2.;
	return 4. * res * PAS;
}

void *calculer_segment(void *arg){
	struct segment** segment = (struct segment**) arg;
	double a = (*segment)->a;
	double b = (*segment)->b;
	(*segment)->result = (f(a) + f(b)) / 2.;
	return NULL;
}
