#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


//       (pai)      
//         |        
//    +----+----+
//    |         |   
// filho_1   filho_2


// ~~~ printfs  ~~~
// pai (ao criar filho): "Processo pai criou %d\n"
//    pai (ao terminar): "Processo pai finalizado!\n"
//  filhos (ao iniciar): "Processo filho %d criado\n"

// Obs:
// - pai deve esperar pelos filhos antes de terminar!


int main(int argc, char** argv) {

    pid_t pid1, pid2;

    // cria o primeiro filho
    fflush(stdout);
    pid1 = fork();

    if (pid1 == 0) {
        // processo filho 1
        sleep(1); // espera o pai criar o segundo filho
        printf("Processo filho %d criado\n", getpid());
        //tem que pesquisar mais sobre isso, mas pelo que entendi é uma boa pratica pra tipo evitar problemas de criar processos que nao pedi (nao entendi muito bem o pq)
        exit(0);
    } else {
        // processo pai
        printf("Processo pai criou %d\n", pid1);
    }

    // cria o segundo filho
    fflush(stdout);
    pid2 = fork();

    if (pid2 == 0) {
        // processo filho 2
        sleep(1); // espera o pai terminar
        printf("Processo filho %d criado\n", getpid());
        exit(0);
    } else {
        // processo pai
        printf("Processo pai criou %d\n", pid2);
    }

    // espera os dois filhos terminarem
    wait(NULL);
    wait(NULL);

    printf("Processo pai finalizado!\n");

    return 0;
}