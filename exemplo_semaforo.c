#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

sem_t mutex;  // declaração do semáforo (binário)

void* tarefa(void* nome) {
    char* id = (char*) nome;

    for (int i = 0; i < 3; i++) {
        printf("%s quer entrar na seção crítica...\n", id);
        sem_wait(&mutex);   // operação P (down): solicita acesso
        
        // ----- SEÇÃO CRÍTICA -----
        printf("%s entrou na seção crítica.\n", id);
        sleep(1);           // simula uso do recurso compartilhado
        printf("%s saiu da seção crítica.\n", id);
        // --------------------------
        
        sem_post(&mutex);   // operação V (up): libera o acesso
        
        // ----- SEÇÃO NÃO CRÍTICA -----
        sleep(1);
    }

    pthread_exit(NULL);
}

int main() {
    pthread_t t1, t2;

    sem_init(&mutex, 0, 1);   // inicializa o semáforo com valor 1 (livre)

    pthread_create(&t1, NULL, tarefa, "Thread 1");
    pthread_create(&t2, NULL, tarefa, "Thread 2");

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    sem_destroy(&mutex);      // destrói o semáforo

    return 0;
}
