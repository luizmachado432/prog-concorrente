#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
//marcaçao
// Função imprime resultados na correção do exercício -- definida em helper.c
void imprimir_resultados(int n, int** results);

// Função recursiva (Fibonacci)
int compute(int arg) {
    if (arg < 2) {
        return arg;
    } else {
        return compute(arg - 1) + compute(arg - 2);
    }
}

// Função wrapper para uso com pthread_create
void* compute_thread(void* arg) {
    int* ret = malloc(sizeof(int));
    if (ret == NULL) {
        perror("malloc");
        pthread_exit(NULL);
    }

    int value = *((int*)arg);
    *ret = compute(value);

    return ret;
}

int main(int argc, char** argv) {
    // Verifica argumentos
    if (argc < 2) {
        printf("Uso: %s n_threads x1 x2 ... xn\n", argv[0]);
        return 1;
    }

    int n_threads = atoi(argv[1]);

    if (n_threads <= 0 || argc < 2 + n_threads) {
        printf("Uso: %s n_threads x1 x2 ... xn\n", argv[0]);
        return 1;
    }

    pthread_t threads[n_threads];
    int args[n_threads];
    int* results[n_threads];

    // Criação das threads
    for (int i = 0; i < n_threads; ++i) {
        args[i] = atoi(argv[2 + i]);

        if (pthread_create(&threads[i], NULL, compute_thread, &args[i]) != 0) {
            perror("pthread_create");
            return 1;
        }
    }

    // Join nas threads
    for (int i = 0; i < n_threads; ++i) {
        if (pthread_join(threads[i], (void**)&results[i]) != 0) {
            perror("pthread_join");
            return 1;
        }
    }

    // Impressão dos resultados (função externa obrigatória)
    imprimir_resultados(n_threads, results);

    // Liberação de memória
    for (int i = 0; i < n_threads; ++i) {
        free(results[i]);
    }

    return 0;
}