    #include <stdlib.h>
    #include <stdio.h>
    #include <pthread.h>
    #include <semaphore.h>
    #include "unistd.h"

    #define N 10 //número de familias.
    #define CARTAS 20 //quantidade de cartas na mochila
    #define CAPACIDADEESPERA 5 
    #define CAPACIDADEMESAS 5 

    void * f_garcom(void *arg);
    void * f_familia(void *arg);
    void * f_chef(void *arg);

    // tem que ter lock para essa variaveis
    int qtd_sala_espera = 0; 
    int qtd_mesa_comida = 0;


    //criando variaveis de semaforo
    sem_t pombo;  
    sem_t escritor; 

    int main(int argc, char **argv){
        
        int i;

        sem_init(&escritor,0,1);
        sem_init(&pombo,0,0);

        // thread de familias
        pthread_t familia[N];
        int *id;
        for(i = 0; i < N; i++){
            id = (int *) malloc(sizeof(int));
            *id = i;
        pthread_create(&(familia[i]),NULL,f_familia,  (void *) (id));
        }

        //thread de chef (temos apenas 1 chef)
        pthread_t chef;
        id = (int *) malloc(sizeof(int));
        *id = 0;
        pthread_create(&(chef),NULL,f_chef, (void*) (id));

        pthread_join(chef,NULL);
        
        //thread de garcom (temos apenas 1 chef)
        pthread_t garcom;
        id = (int *) malloc(sizeof(int));
        *id = 0;
        pthread_create(&(garcom),NULL,f_garcom, (void*) (id));

        pthread_join(garcom,NULL);
    
    }


    //Inicialmente aguardar/dorme o garçom solicitar uma comida (quando a lista de comidas tem comida, tem que pegar acordar?)
    //cozinha a comida que ele solicitou. (nao concorrente)
    //chama o garçom para pegar a comida.
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


    // dorme/aguarda o cliente chamar ele
    // quando chama, anota o pedido e vai acordar o cozinheiro
    // processo simultaneo é ele ficar coletando os pedidos
    // quando os pedidos são coletados...
    void * f_garcom(void *arg){
        
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

    // chega no local, é o primeiro a se movimentar
    // se tiver vaga na 
    void * f_familia(void *arg){
        int id = *(int*) arg;

        // se tem a 
        if(qtd_sala_espera < CAPACIDADEESPERA){

        }

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
