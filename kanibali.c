#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>

pthread_mutex_t monitor;
pthread_cond_t camac;
pthread_cond_t DO;
pthread_cond_t LO;

int desna_polje[30];
int lijeva_polje[30];
int camac_polje[30];
int strana;
int broj_misionara;
int broj_kanibala;
bool camac_na_obali;

void ispis_polja();
void *dretva_camac(void *arg);
void *stvaraj_misionare(void *arg);
void *misionar(void *arg);
void *kanibal(void *arg);

int main(void){

    srand(time(NULL));

    //inicijalizacija polja na nulu
    for (int i = 0; i < 30 ; i++){
        desna_polje[i] = 0;
        lijeva_polje[i] = 0;
        camac_polje[i] = 0;
    }

    //inicijalizacija monitora i redova
    pthread_mutex_init (&monitor, NULL);
	pthread_cond_init (&camac, NULL);
    pthread_cond_init (&DO, NULL);
    pthread_cond_init (&LO, NULL);

    //dretva broda
    pthread_t camac_d;
    pthread_create(&camac_d, NULL, dretva_camac, NULL);

    sleep(1);

    //dretva za stvarat misionare
    pthread_t misionari;
    pthread_create(&misionari, NULL, stvaraj_misionare, NULL);

    int i = 1;
    while(1){   //stvaranje kanibala zauvijek s razmakom od jedne sekunde

        sleep(1);
        pthread_t dretva;
        int *tmp = malloc(sizeof(int));
        *tmp = i;
        pthread_create(&dretva, NULL, kanibal, (void *)tmp);
        i++;

    }
}

void *dretva_camac(void *arg){
    
    //prvo se camac nalazi na desnoj obali
    strana = 1;

    while (1){

        pthread_mutex_lock(&monitor); //zauzmi monitor
        broj_misionara = 0; //resetiranje
        broj_kanibala = 0;
        camac_na_obali = true;

        //prazan na l/d obali
        if (strana == 1){
            printf("\nC: prazan na desnoj obali\n");
        } else {
            printf("\nC: prazan na lijevoj obali\n");
        }
        ispis_polja(strana);

        if (strana == 1){       //signaliziranje K i M da se ukrcaju
            pthread_cond_broadcast(&DO);
        } else {
            pthread_cond_broadcast(&LO);
        }

        pthread_mutex_unlock(&monitor); //otkljucavanje monitora

        pthread_mutex_lock(&monitor);

        while (broj_misionara + broj_kanibala < 3){
            pthread_cond_wait(&camac, &monitor);  //cekanje da se ukrcaju tri putnika
        }

        printf("\nC: tri putnika ukrcana, polazim za jednu sekundu\n"); //ukrcana tri putnika
        ispis_polja(strana);

        if (strana == 1){       //signaliziranje K i M da se ukrcaju
            pthread_cond_broadcast(&DO);
        } else {
            pthread_cond_broadcast(&LO);
        }


        pthread_mutex_unlock(&monitor); //otpusti monitor

        sleep(1);   //cekanje 1 sekundu prije polaska

        pthread_mutex_lock(&monitor);   //zauzmi ponovo monitor

        camac_na_obali = false;

        if (strana == 1){
            printf("\nC: prevozim s desne na lijevu obalu:");
        } else {
            printf("\nC: prevozim s lijeve na desnu obalu:");
        }
        for (int i = 0; i < 30; i++){
            if (camac_polje[i] == 0){                         //printanje da camac prevozi
                continue;
            } else {
                if (i % 2 == 0){
                    printf(" %c", camac_polje[i]);
                } else {
                    printf("%d", camac_polje[i]);
                }
            }
        }
        printf("\n");


        pthread_mutex_unlock(&monitor); //pustanje monitora

        sleep(2);  //simuliranje trajanja prijevoza sa obale na obalu

        pthread_mutex_lock(&monitor); //zauzimanje monitora

        if (strana == 1){
            printf("\nC: preveo s desne na lijevu obalu:");
        } else {
            printf("\nC: preveo s lijeve na desnu obalu:");
        }
        for (int i = 0; i < 30; i++){
            if (camac_polje[i] == 0){                         //printanje da je camac preveo
                printf("\n");
                break;
            } else {
                if (i % 2 == 0){
                    printf(" %c", camac_polje[i]);
                } else {
                    printf("%d", camac_polje[i]);
                }
            }
        }

        //brisanje putnika
        int k = 0;
        while (camac_polje[k] != 0){
            camac_polje[k] = 0;
            k++;
        }

        if (strana == 1){
            strana = 0;         //mijenjanje obale
        } else{
            strana = 1;
        }

        pthread_mutex_unlock(&monitor); //pustanje momitora

    }

}

void *stvaraj_misionare(void *arg){

    int i = 1;

    while (1){  //stvaraj dretve misionara zauvijek sa razmakom od dvije sekunde
        sleep(2);
        pthread_t dretva;
        int *tmp = malloc(sizeof(int));
        *tmp = i;
        pthread_create(&dretva, NULL, misionar, (void *)tmp);
        i++;
    }

}

void *kanibal (void *arg){

    int k = 0;
    int i = *((int *)arg);
    int obala = rand() % 2;

    pthread_mutex_lock(&monitor);

    if (obala == 1){        //dosao na desnu obalu

        for (int j = 0; j < 30; j++){   
            if (desna_polje[j] == 0){
                desna_polje[j] = 'K';
                desna_polje[j + 1] = i;
                break;
            } else{
                continue;
            }
        }
        printf("\nK%d: došao na desnu obalu\n", i);

    } else {    //dosao na lijevu obalu

         for (int j = 0; j < 30; j++){   
            if (lijeva_polje[j] == 0){
                lijeva_polje[j] = 'K';
                lijeva_polje[j + 1] = i;
                break;
            } else{
                continue;
            }
        }
        printf("\nK%d: došao na lijevu obalu\n", i);

    }
    ispis_polja(strana);

    while (obala != strana || (broj_kanibala + broj_misionara) > 7 || 
        (broj_kanibala != 0 && broj_misionara == broj_kanibala) || (!camac_na_obali) )
    {
        if (obala == 1){
            pthread_cond_wait(&DO, &monitor);
        } else {
            pthread_cond_wait(&LO, &monitor);
        }

    }

    broj_kanibala++;
    if (strana == 1){   //micanje kanibala ako je bio na desnoj obali

        while (desna_polje[k] != 'K' || desna_polje[k + 1] != i ){
            k++;
        }
        desna_polje[k] = 0;
        desna_polje[k + 1] = 0;                                  

    } else { //micanje kanibala ako je bio na lijevoj obali

        while (lijeva_polje[k] != 'K' || lijeva_polje[k + 1] != i ){
            k++;
        }
        lijeva_polje[k] = 0;
        lijeva_polje[k + 1] = 0;

    }

    k = 0;
    while (camac_polje[k] != 0){
        k++;
    }
    camac_polje[k] = 'K';
    camac_polje[k + 1] = i;

    printf("\nK%d: ušao u čamac\n", i);         //printanje ušao u čamac
    ispis_polja(strana);

    pthread_cond_signal(&camac);      //signaliziranje camcu da ispita uvjet za kretanje

    pthread_mutex_unlock(&monitor);


}

void *misionar (void *arg){

    int k = 0;
    int i = *((int *)arg);
    int obala = rand() % 2;

    pthread_mutex_lock(&monitor);

    if (obala == 1){        //dosao na desnu obalu

        for (int j = 0; j < 30; j++){   
            if (desna_polje[j] == 0){
                desna_polje[j] = 'M';
                desna_polje[j + 1] = i;
                break;
            } else{
                continue;
            }
        }
        printf("\nM%d: došao na desnu obalu\n", i);

    } else {    //dosao na lijevu obalu

         for (int j = 0; j < 30; j++){   
            if (lijeva_polje[j] == 0){
                lijeva_polje[j] = 'M';
                lijeva_polje[j + 1] = i;
                break;
            } else{
                continue;
            }
        }
        printf("\nM%d: došao na lijevu obalu\n", i);

    }
    ispis_polja(strana);

    while (obala != strana || (broj_kanibala + broj_misionara) > 7 || broj_kanibala >= 2 
            || (!camac_na_obali)){

        if (obala == 1){
            pthread_cond_wait(&DO, &monitor);
        } else {
            pthread_cond_wait(&LO, &monitor);
        }

    }

    broj_misionara++;
    if (strana == 1){   //micanje misionara ako je bio na desnoj obali

        while (desna_polje[k] != 'M' || desna_polje[k + 1] != i){
            k++;
        }
        desna_polje[k] = 0;
        desna_polje[k + 1] = 0;                                  

    } else { //micanje misionara ako je bio na lijevoj obali

        while (lijeva_polje[k] != 'M' || lijeva_polje[k + 1] != i){
            k++;
        }
        lijeva_polje[k] = 0;
        lijeva_polje[k + 1] = 0;

    }

    k = 0;
    while (camac_polje[k] != 0){
        k++;
    }
    camac_polje[k] = 'M';
    camac_polje[k + 1] = i;

    printf("\nM%d: ušao u čamac\n", i);         //printanje ušao u čamac
    ispis_polja(strana);

    pthread_cond_signal(&camac);      //signaliziranje camcu da ispita uvjet za kretanje

    pthread_mutex_unlock(&monitor);


}

void ispis_polja(int strana){

    if (strana == 0){
        printf("C[L]={");
    } else {
        printf("C[D]={");
    }

    bool prvi_putC = true;
    for (int i = 0; i < 30; i++){
        if(camac_polje[i] != 0){

            if (i % 2 == 0){
                if (prvi_putC){
                    printf("%c", camac_polje[i]);
                    prvi_putC = false;
                } else{
                    printf(" %c", camac_polje[i]);
                }
            } else{
                printf("%d", camac_polje[i]);
            }
        } else {

            continue; //nastavi

        }
        
    }
    printf("} LO={");

    bool prvi_putL = true;
    for(int i = 0; i < 30; i++){

        if(lijeva_polje[i] != 0){

            if (i % 2 == 0){
                if (prvi_putL){
                    printf("%c", lijeva_polje[i]);
                    prvi_putL = false;
                } else{
                    printf(" %c", lijeva_polje[i]);
                }
            } else{
                printf("%d", lijeva_polje[i]);
            }
        } else {

            continue; //nastavi printat

        }

    }
    printf("} DO={");


    bool prvi_putD = true;
    for(int i = 0; i < 30; i++){

        if(desna_polje[i] != 0){

            if (i % 2 == 0){
                if (prvi_putD){
                    printf("%c", desna_polje[i]);
                    prvi_putD = false;
                } else{
                    printf(" %c", desna_polje[i]);
                }
            } else{
                printf("%d", desna_polje[i]);
            }
        } else {

            continue; //nastavi printat

        }

    }
    printf("}\n");

}