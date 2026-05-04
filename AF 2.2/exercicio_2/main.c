#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>
// MARCAÇAO
// Lê o conteúdo do arquivo filename e retorna um vetor E o tamanho dele
// Se filename for da forma "gen:%d", gera um vetor aleatório com %d elementos
//
// +-------> retorno da função, ponteiro para vetor malloc()ado e preenchido
// | 
// |         tamanho do vetor (usado <-----+
// |         como 2o retorno)              |
// v                                       v
double* load_vector(const char* filename, int* out_size);

// Avalia o resultado no vetor c. Assume-se que todos os ponteiros (a, b, e c)
// tenham tamanho size.
void avaliar(double* a, double* b, double* c, int size);

// Struct para passar dados para cada thread
typedef struct {
    double* a;
    double* b;
    double* c;
    int start;
    int end;
} ThreadData;

// Função executada por cada thread
void* sum_vectors(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    for (int i = data->start; i < data->end; ++i) {
        data->c[i] = data->a[i] + data->b[i];
    }
    return NULL;
}

int main(int argc, char* argv[]) {
    // Gera um resultado diferente a cada execução do programa
    // Se **para fins de teste** quiser gerar sempre o mesmo valor
    // descomente o srand(0)
    srand(time(NULL)); //valores diferentes
    //srand(0);        //sempre mesmo valor

    //Temos argumentos suficientes?
    if(argc < 4) {
        printf("Uso: %s n_threads a_file b_file\n"
               "    n_threads    número de threads a serem usadas na computação\n"
               "    *_file       caminho de arquivo ou uma expressão com a forma gen:N,\n"
               "                 representando um vetor aleatório de tamanho N\n",
               argv[0]);
        return 1;
    }
  
    //Quantas threads?
    int n_threads = atoi(argv[1]);
    if (!n_threads) {
        printf("Número de threads deve ser > 0\n");
        return 1;
    }
    //Lê números de arquivos para vetores alocados com malloc
    int a_size = 0, b_size = 0;
    double* a = load_vector(argv[2], &a_size);
    if (!a) {
        //load_vector não conseguiu abrir o arquivo
        printf("Erro ao ler arquivo %s\n", argv[2]);
        return 1;
    }
    double* b = load_vector(argv[3], &b_size);
    if (!b) {
        printf("Erro ao ler arquivo %s\n", argv[3]);
        return 1;
    }
    
    //Garante que entradas são compatíveis
    if (a_size != b_size) {
        printf("Vetores a e b tem tamanhos diferentes! (%d != %d)\n", a_size, b_size);
        return 1;
    }
    //Cria vetor do resultado 
    double* c = malloc(a_size*sizeof(double));

    // Paralelizar a soma
    clock_t start_time = clock();
    int actual_threads = (n_threads > a_size) ? a_size : n_threads;
    pthread_t* threads = malloc(actual_threads * sizeof(pthread_t));
    ThreadData* data = malloc(actual_threads * sizeof(ThreadData));

    // Dividir o trabalho
    int chunk = a_size / actual_threads;
    int remainder = a_size % actual_threads;
    int start = 0;
    for (int i = 0; i < actual_threads; ++i) {
        int extra = (i < remainder) ? 1 : 0;
        int end = start + chunk + extra;
        data[i].a = a;
        data[i].b = b;
        data[i].c = c;
        data[i].start = start;
        data[i].end = end;
        pthread_create(&threads[i], NULL, sum_vectors, &data[i]);
        start = end;
    }

    // Aguardar todas as threads terminarem
    for (int i = 0; i < actual_threads; ++i) {
        pthread_join(threads[i], NULL);
    }

    clock_t end_time = clock();
    double time_spent = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    printf("Tempo gasto: %f segundos com %d threads\n", time_spent, actual_threads);

    free(threads);
    free(data);

    //    +---------------------------------+
    // ** | IMPORTANTE: avalia o resultado! | **
    //    +---------------------------------+
    avaliar(a, b, c, a_size);
    

    //Importante: libera memória
    free(a);
    free(b);
    free(c);

    return 0;
}