#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

//                          (principal)
//                               |
//              +----------------+--------------+
//              |                               |
//           filho_1                         filho_2
//              |                               |
//    +---------+-----------+          +--------+--------+
//    |         |           |          |        |        |
// neto_1_1  neto_1_2  neto_1_3     neto_2_1 neto_2_2 neto_2_3

// ~~~ printfs  ~~~
//      principal (ao finalizar): "Processo principal %d finalizado\n"
// filhos e netos (ao finalizar): "Processo %d finalizado\n"
//    filhos e netos (ao inciar): "Processo %d, filho de %d\n"

// Obs:
// - netos devem esperar 5 segundos antes de imprmir a mensagem de finalizado (e terminar)
// - pais devem esperar pelos seu descendentes diretos antes de terminar

int main(int argc, char** argv) {

    pid_t pid;

    // Criar 2 filhos
    for (int i = 0; i < 2; i++) {

        fflush(stdout);
        pid = fork();

        if (pid == 0) {
            // criando os processos filho
            printf("Processo %d, filho de %d\n", getpid(), getppid());

            pid_t neto;

            // for pra riar 3 netos
            for (int j = 0; j < 3; j++) {

                fflush(stdout);
                neto = fork();

                if (neto == 0) {
                    // Processo neto
                    printf("Processo %d, filho de %d\n", getpid(), getppid());

                    sleep(5);

                    printf("Processo %d finalizado\n", getpid());
                    exit(0);
                }
            }

            // filho esta esperando os 3 netos
            for (int j = 0; j < 3; j++) {
                wait(NULL);
            }

            printf("Processo %d finalizado\n", getpid());
            exit(0);
        }
    }

    //processo principal ta esperando os 2 filhos
    for (int i = 0; i < 2; i++) {
        wait(NULL);
    }

    printf("Processo principal %d finalizado\n", getpid());    
    return 0;
}