#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
// MARCAÇAO
long contador_global = 0;  

// função executada pelas threads
void* incrementa(void* arg) {
    long n_loops = *((long*)arg);

    for (long i = 0; i < n_loops; i++) {
        contador_global++;  
    }

    return NULL;
}

int main(int argc, char* argv[]) {

    if (argc != 3) {
        printf("Uso: %s n_threads n_loops\n", argv[0]);
        return 1;
    }

    int n_threads = atoi(argv[1]);
    long n_loops = atol(argv[2]);

    pthread_t threads[n_threads];

    for (int i = 0; i < n_threads; i++) {
        pthread_create(&threads[i], NULL, incrementa, &n_loops);
    }

    for (int i = 0; i < n_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    long esperado = n_threads * n_loops;

    printf("Contador: %ld\n", contador_global);
    printf("Esperado: %ld\n", esperado);

    return 0;
}