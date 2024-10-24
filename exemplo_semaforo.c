#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#define NUM_THREADS 2
#define NUM_INCREMENTS 5

int shared_var = 0; // Variável compartilhada
sem_t sem; // Declaração do semáforo

void* increment(void* arg) {
    int thread_id = *(int*)arg; // cada thread recebe um id único
    for(int i = 0; i < NUM_INCREMENTS; i++) {
        sem_wait(&sem); // Entrada na região crítica
        printf("Thread %d entrou na região crítica \n", thread_id);
        shared_var++; // Incrementa a variável compartilhada
        printf("Thread %d saiu da região crítica e deixou shared_var valendo %d\n", thread_id, shared_var);
        sem_post(&sem); // Saída da região crítica
    }
    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];

    // Inicializa o semáforo controlado apenas dentro do processo com valor 1
    sem_init(&sem, 0, 1);

    // Cria os threads
    for(int i = 0; i < NUM_THREADS; i++) {
        thread_ids[i] = i + 1; // Define um ID para cada thread
        //cria as threads, determinando que todas executarão a função increment
        pthread_create(&threads[i], NULL, increment, &thread_ids[i]);
    }

    // Espera os threads terminarem
    for(int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Destroi o semáforo
    sem_destroy(&sem);

    // Imprime o valor final da variável compartilhada
    printf("Valor final da variável compartilhada: %d\n", shared_var);

    return 0;
}
