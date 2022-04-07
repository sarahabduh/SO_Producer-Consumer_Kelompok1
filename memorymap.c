#include <sys/mman.h>   /* mmap()  */
#include <stdlib.h>     /* exit() */
#include <unistd.h>     /* fork() and getpid() */
#include <stdio.h>      /* printf() */
#include <sys/wait.h>   /*wait*/
#include <errno.h>	    /* errno */
#include <time.h>       /* time */

#define MSGSIZE 3

int main(int argc, char **argv) {
        int pid;
        int i, p, c, sum = 0;
        
         srand(time(0)); /* titik awal proses random number generator */
         
        int size = MSGSIZE * sizeof(int);
        //buat memory map
        void *addr = mmap(0, size, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
        printf("Mapped at %p\n", addr);
        int *shared = addr;
        
        printf("Input jumlah producer: ");
        scanf("%d", &p);
        printf("Input jumlah consumer: ");
        scanf("%d", &c);
        
        switch (pid = fork()) {
        case 0:         /*  proses anak */
                printf("Proses producer \n");
                //isi data di memory map
                for(i = 0; i < p; i++){
                    shared[i] = rand() % 50; /* random number maksimal 50 */
                    printf("[%d] Producer produces value: %d\n", getpid(), shared[i]);
                }
                
                break;
        default:        /*  ortu */
                printf("Proses consumer\n");
                int pidWait,status;
                /* wait sampai child selesai */
                while (pidWait = wait(&status)) {   
                    if (pidWait == pid)  /* child sukses selesai*/
                        //cetak isi memory map
                        for(i = 0; i < c; i++){
                            printf("[%d] Consumer consumes value: %d\n", getpid(), shared[i]);
                            sum = sum + shared[i];
                        }
                        printf("Total values in consumer: %d\n", sum);

                        break; //keluar dari loop wait
                    if ((pidWait == -1) && (errno != EINTR)) {
                        /* ada error*/
                        perror("waitpid");
                        exit(1);
                    }
                }
                break;
        case -1:        /* error */
                perror("fork");
                exit(1);
        }
        exit(0);
}
