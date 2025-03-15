#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>

#ifndef N /* nombre de lignes des matrices */
#define N 10
#endif

#ifndef K
#define K 4
#endif

void imprime(int matrice[N][N]);
void generation_aleatoire(int matrice[N][N]);
void *produit(void *l);
void *produit_K_thread(void *l);

int A[N][N], B[N][N], R[N][N];

int main(void)
{
	struct timespec tic, toc;
	double duree;

	srand(time(NULL));
	generation_aleatoire(A);
	generation_aleatoire(B);
	
	// imprime(A);
	// puts("");
	// imprime(B);
	// puts("");
	clock_gettime(CLOCK_REALTIME, &tic);
	pthread_t th[K];
	int indices[K];
	for(int i = 0; i < K; i++){
		indices[i] = i;
		pthread_create(&th[i], NULL, produit_K_thread, &indices[i]);
	}
	for(int i = 0; i < K; i++){
		pthread_join(th[i], NULL);
	}
	clock_gettime(CLOCK_REALTIME, &toc);
	duree = (toc.tv_sec - tic.tv_sec);
	duree += (toc.tv_nsec - tic.tv_nsec) / 1000000000.0;
	printf("durée : %g\n", duree);
	// imprime(R);
	return 0;
}

void imprime(int matrice[N][N])
{
	int i, j;
	for (i = 0; i < N; ++i)
		for (j = 0; j < N; ++j)
			printf("%d%c", matrice[i][j], j == N - 1 ? '\n' : '\t');
}

void generation_aleatoire(int matrice[N][N])
{
	int i, j;
	for (i = 0; i < N; ++i)
		for (j = 0; j < N; ++j)
			matrice[i][j] = rand() % 10;
}

void *produit(void *l){
	int ligne = *((int*)l);
	for(int i = 0; i < N; i++){
		R[ligne][i] = 0;
		for(int j = 0; j < N; j++){
			R[ligne][i] += A[ligne][j] * B[j][i];
		}
	}
	return NULL;
}

void *produit_K_thread(void *l){
	int ligne = *((int*)l);
	if(ligne >= N) return NULL;
	produit(l);
	int nouvelle_ligne = ligne + K;
	return produit_K_thread(&nouvelle_ligne);
}