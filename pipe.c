#include <stdlib.h>     /* exit() */
#include <unistd.h>     /* fork() and getpid() */
#include <stdio.h>      /* printf() */
#include <string.h>
#include <time.h>

int main(int argc, char **argv) {
        int pid;
        int fd[2]; //pipe

        int p, c, i, sum = 0;

        //create pipe
        if (pipe(fd) < 0) {
            exit(1); //error
        }

        printf("Input jumlah producer: ");
        scanf("%d", &p);
        printf("Input jumlah consumer: ");
        scanf("%d", &c);

        srand(time(0)); /* titik awal proses random number generator */
        
        switch (pid = fork()) {
        case 0:         /* fork returns 0 ke proses anak */
                /* tutup bagian input dari pipe */
                close(fd[0]);
                
                // tulis ke pipe
                printf("----- producer -----\n");
                int arrProducer[sizeof(p)];
                for(i = 0; i < p; i++){
                    arrProducer[i] = rand() % 50; /* random number maksimal 50 */
                    printf("[%d] Producer produces value: %d\n", getpid(), arrProducer[i]);
                }

                write(fd[1], arrProducer, sizeof(arrProducer));
                
                break;
        default:        /* fork returns pid ke proses ortu */
                
                /* tutup bagian output dari pipe */
                close(fd[1]);
                // baca yang ditulis child dari pipe
                printf("----- consumer -----\n");
                int arrConsumer[sizeof(c)];
                read(fd[0], arrConsumer, sizeof(arrConsumer)); //buffer terisi
                for(i = 0; i < c; i++){
                    printf("[%d] Consumer consumes value: %d\n", getpid(), arrConsumer[i]);
                    sum = sum + arrConsumer[i];
                }
                printf("Total values in consumer: %d", sum);
                break;
        case -1:        /* error */
                perror("fork");
                exit(1);
        }
        
        exit(0);
}
