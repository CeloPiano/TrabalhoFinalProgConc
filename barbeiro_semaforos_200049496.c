/*
 * Problema do barbeiro dorminhoco.

  tem que ter 2 barreiras

  um para o salao
    uma para o cliente entrar (tem que ver cadeiras vazias)

  um para a cadeira
    uma para o barbeiro atender (com uma permissao apenas como se fosse um lock
 */ 


#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define N_CLIENTES 1000
#define N_CADEIRAS 5

sem_t sem_cadeiras;
sem_t sem_barbeiro_cadeira;
sem_t corte_full;
sem_t corte_empty;

void * f_barbeiro(void *v) {

  while(1) {
    //...Esperar/Dormindo algum cliente sentar na cadeira do barbeiro (e acordar o barbeiro)
    sem_wait(&corte_empty); //aqui descresce de empty que começa cbeio
      printf("Barbeiro cortou o cabelo de um cliente\n"); 
      sleep(1); //Cortar o cabelo do cliente 
    sem_post(&corte_full);
    
    //...Liberar/desbloquear o cliente
  }
  pthread_exit(0);
}

void* f_cliente(void* v) {
  int id = *(int*) v;
  sleep(2);
  if(sem_trywait(&sem_cadeiras) == 0){ //conseguiu pegar uma cadeira de espera
     
     printf("Cliente %d entrou na barbearia \n",id);
    sleep(2);
     
     //... pegar/sentar a cadeira do barbeiro
    sem_wait(&sem_barbeiro_cadeira); //permissao para a cadeira começando com 1
      printf("Cliente %d sentou na cadeira do barbeiro...\n",id);
      sleep(1);

     //... liberar a sua cadeira de espera
    sem_post(&sem_cadeiras); //libera a cadeira de sentar
      printf("Cliente %d liberou sua cadeira de espera...\n",id);
      sleep(1);

     //... acordar o barbeiro para cortar seu cabelo
    sem_post(&corte_empty); // da a permissao para o barbeiro agir
      printf("Cliente %d cortando o cabelo...\n",id);
      sleep(1);

     //... aguardar o corte do seu cabelo
    sem_wait(&corte_full); //aqui espera terminar o corte.
     
     //... liberar a cadeira do barbeiro
    sem_post(&sem_barbeiro_cadeira);
     
      printf("Cliente %d cortou o cabelo e foi embora \n",id);

  }else{//barbearia cheia
	printf("Barbearia cheia, cliente %d indo embora\n",id);
  }

  pthread_exit(0);
}

int main() {
  pthread_t thr_clientes[N_CLIENTES], thr_barbeiro;
  int i, id[N_CLIENTES];

  sem_init(&sem_barbeiro_cadeira, 0, 1);
  sem_init(&corte_empty, 0, 0);
  sem_init(&corte_full, 0, 0);

  sem_init(&sem_cadeiras, 0, N_CADEIRAS);
  
  for (i = 0; i < N_CLIENTES; i++) {
    id[i] = i;
    pthread_create(&thr_clientes[i], NULL, f_cliente, (void*) &id[i]);
  }

  pthread_create(&thr_barbeiro, NULL, f_barbeiro, NULL);
  
  for (i = 0; i < N_CLIENTES; i++) 
    pthread_join(thr_clientes[i], NULL);

  /* Barbeiro assassinado */
  
  return 0;
}

