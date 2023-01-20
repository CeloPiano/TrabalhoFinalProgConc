#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

#define PR 1 //número de produtores
#define CN 1 // número de consumidores
#define N 5  //tamanho do buffer


void * produtor(void * meuid);
void * consumidor (void * meuid);

int buffer = 0; //inicializando o buffer para prints e simulação...

//criando variaveis de semaforo
sem_t empty;  
sem_t full; 

pthread_mutex_t a = PTHREAD_MUTEX_INITIALIZER;

void main(argc, argv)
int argc;
char *argv[];
{

  int erro;
  int i, n, m;
  int *id;

  pthread_t tid[PR];
   
  sem_init(&empty,0,N);  //o empty começa cheio, já que nao temos nenhuma coisa produzida no buffer ainda...
  sem_init(&full,0,0);   //o full começa cheio...

  for (i = 0; i < PR; i++)
  {
    id = (int *) malloc(sizeof(int));
    *id = i;
    erro = pthread_create(&tid[i], NULL, produtor, (void *) (id));

    if(erro)
    {
      printf("erro na criacao do thread %d\n", i);
      exit(1);
    }
  }

  pthread_t tCid[CN];

  for (i = 0; i < CN; i++)
  {
    id = (int *) malloc(sizeof(int));
    *id = i;
    erro = pthread_create(&tCid[i], NULL, consumidor, (void *) (id));

    if(erro)
    {
      printf("erro na criacao do thread %d\n", i);
      exit(1);
    }
  }
 
  pthread_join(tid[0],NULL);

}

void * produtor (void* pi)
{
 
  while(1)
  {

    sem_wait(&empty); //decresce no empty.
    sleep(drand48()*3); //sleep para simular

      //lock para acessar a variavel buffer sem problemas
      pthread_mutex_lock(&a);
        buffer++;
        printf("Produtor: Produzi no buffer, TAMANHO = %d \n \n", buffer);
      pthread_mutex_unlock(&a);

    sem_post(&full); //acrescenta no full.

  }
  pthread_exit(0);
  
}

void * consumidor (void* pi)
{
 while(1)
  {

    sem_wait(&full); //como aqui consome, decrescenta uma posicao do full 
    sleep(drand48()*3);

      //lock para retirar do buffer sem problemas.
      pthread_mutex_lock(&a);
        buffer--;
        printf("Consumidor: Retirei do Buffer, TAMANHO = %d \n \n", buffer);
      pthread_mutex_unlock(&a);
      
    sem_post(&empty); //como aqui consome,acrescenta depois de consumir no empty
  }
  pthread_exit(0);
  
}
