#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <stdatomic.h>
#define VRIJEME 0

int id; //identifikacijski broj
int *A; //varijabla koju povecavamo
atomic_int *M_kopija = NULL; //kopija varijable M
atomic_int *N_kopija = NULL; //kopija varijable N
atomic_int *broj = NULL; //varijable polja za Lamporta
atomic_int *ulaz = NULL;

void *obradi_dretvu(void *arg);


int main(int argc, char *argv[]){

    if (argc != 3){
        printf("Neispravan broj argumenta.\n");
        exit(0);
    }

    atomic_int N = atoi(argv[1]);
    atomic_int M = atoi(argv[2]);

    ulaz = (atomic_int*) malloc((N + 1) * sizeof(atomic_int));
    broj = (atomic_int*) malloc((N + 1) * sizeof(atomic_int));

    if (ulaz == NULL || broj == NULL){
        printf("Neuspjela alokacija memorije.\n");
        exit(0);
    }

    //zauzimanje memorije
    id = shmget(IPC_PRIVATE, sizeof(atomic_int) + 2 * (N + 1) * sizeof(atomic_int) , 0600);

    if (id == -1){
        printf("Greška: Nije uspjelo stvaranje zajedničke memorije.");
        exit(0);
    }

    //spajanje na memoriju
    A = (int *) shmat(id, NULL, 0);
    ulaz = (atomic_int*) (A + 1);
    broj = (atomic_int*) (ulaz + N + 1);
    M_kopija  = (atomic_int*) (broj + N + 1);
    N_kopija = (atomic_int*) (M_kopija + 1);
    

    //inicijalizacija varijabli
    for (int i = 1; i <= N; i++){
        ulaz[i] = 0;
        broj[i] = 0;
    }
    M_kopija = &M;
    N_kopija = &N;
    *A = 0;

    //kreiranje dretvi
    pthread_t dretve[N + 1];
    atomic_int proba;

    for (atomic_int i = 1; i <= N; i++){
        atomic_int *tmp = malloc(sizeof(atomic_int));
        *tmp = i;
        proba = pthread_create(&dretve[i], NULL, obradi_dretvu, (void *)tmp);
        if (proba != 0){
            printf("Neuspješno kreiranje dretve.\n");
            exit(0);
        }
    }


    for (atomic_int i = 1; i <= N; i++){
        atomic_int *tmp = malloc(sizeof(atomic_int));
        *tmp = i;
        proba = pthread_join(dretve[*tmp], NULL);
        if (proba != 0){
            printf("Problem sa dretvom.\n");
            exit(0);
        }
    }

    //konacnavrijednost varijable A
    printf("A = %d, Kraj programa.\n", *A);

    //oslobađanje zauzete memorije
    

    return 0;
}


void *obradi_dretvu(void *arg){
    atomic_int i = *((atomic_int *)arg);
    atomic_int k = 0;

    while (k < *M_kopija){

       ulaz[i] = 1;
       atomic_int max = broj[1];

       for (atomic_int j = 1; j <= *N_kopija; j++){
            if (broj[j] > max){
                max = broj[j];
            }
        }

        broj[i] = max + 1;
        ulaz[i] = 0;

        //cekanje za ulazak u kriticni odsjecak
        for (atomic_int j = 1; j <= *N_kopija; j++){

            if (j == i){
                continue;
            }

            while (ulaz[j] != 0){
                continue;
            }

             while (broj[j] != 0 && (broj[j] < broj[i] || (broj[j] == broj[i] && j < i ) ) ){
                continue;
            }
        }

        //kriticni odsjecak
        *A = (*A) + 1;
        printf("Dretva %d povećava A na %d\n", i, *A);
        sleep(VRIJEME);

        //izlazak iz kritičnog odsječka
        broj[i] = 0;

        k++;  
    }

    pthread_exit(NULL);
}