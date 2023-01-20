    #include <stdlib.h>
    #include <stdio.h>
    #include <pthread.h>
    #include <semaphore.h>
    #include "unistd.h"

    #define N 10 //número de usuários

    #define CARTAS 20 //quantidade de cartas na mochila


    void * f_usuario(void *arg);
    void * f_chef(void *arg);

    int cartasBuffer = 0;
    //criando variaveis de semaforo
    sem_t pombo;  
    sem_t escritor; 

    int main(int argc, char **argv){
        
        int i;

        sem_init(&escritor,0,1);
        sem_init(&pombo,0,0);

        pthread_t usuario[N];
        int *id;
        for(i = 0; i < N; i++){
            id = (int *) malloc(sizeof(int));
            *id = i;
        pthread_create(&(usuario[i]),NULL,f_usuario,  (void *) (id));
        }

        pthread_t pombo;
        id = (int *) malloc(sizeof(int));
        *id = 0;
        pthread_create(&(pombo),NULL,f_chef, (void*) (id));

        pthread_join(pombo,NULL);
    
    }


    //Inicialmente está em A, aguardar/dorme a mochila ficar cheia (20 cartas)
    //Leva as cartas para B e volta para A
    //Acordar os usuários   
    void * f_chef(void *arg){
    
        while(1){

            //aqui recebe a permissao para atuar
            sem_wait(&pombo);

            //tem que decrescentar as cartas.
            printf("pombo: entregando as cartas de A para B.\n");
            sleep(5);
            cartasBuffer = 0;

            //volta a domir devolvendo a posicao.
            sem_post(&pombo);
            //acorda os usuarios/escritores.
            sem_post(&escritor);
        
        }
    }

    //Escreve uma carta
    //Caso o pombo não esteja em A ou a mochila estiver cheia, então dorme	
    //Posta sua carta na mochila do pombo
    //Caso a mochila fique cheia, acorda o ṕombo
    void * f_usuario(void *arg){
        
        while(1){
        
        //aqui pega a permissao da escrita.
        sem_wait(&escritor); 
        
            cartasBuffer++; 
            printf("escritor: escrevendo as cartas. QTD: %d\n", cartasBuffer);
            sleep(1);

        if(cartasBuffer == 20){
            //acordar os pombos dando a permissao de escrita.

            printf("escritor: Escrevendo a ultima carta, e chamando o pombo...\n");
            sleep(1);
            sem_post(&pombo);
        }
        //aqui tem que freezar os escritores
        else{
            //aqui devolve a permissao da escrita.
            sem_post(&escritor); 
        }
        }
    }
