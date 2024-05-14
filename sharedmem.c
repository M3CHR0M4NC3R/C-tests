#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>

struct grid {
	int board[5][5];
};
int stupid(struct grid *input){
    int r = rand()%25;
    int temp;
    int i,j,k;
    pid_t pid;
    i=r%5;
    j=r/5;
    temp=input->board[i][j];
    temp=(temp*temp)/temp;
    do{
        k = rand()%100;
    } while (k != 4);
    temp++;
    input->board[i][j]=temp;
    return r;
}
void initgrid(struct grid *input){
    int i,j=0;
    for(i=0;i<5;i++){
        for(j=0;j<5;j++){
            input->board[i][j]=0;}
    }
    return;
}
void printgrid(struct grid *input){
    int i,j=0;
    for(i=0;i<5;i++){
        putchar('\n');
        for(j=0;j<5;j++){
            printf("[%5d] ",input->board[i][j]);
        }
        putchar('\n');
    }
    return;
}
int boardTotal(struct grid *input){
    int total = 0;
    int i,j;
    for(i=0;i<5;i++){
        for(j=0;j<5;j++){
            total+=input->board[i][j];
        }
    }
    return total;
}

int main(){
    struct grid *goober;
    srand(time(NULL));
    int p=0;
    int b;
    pid_t pid;
    /*create a block of shared memory the size of my struct*/
	int boardID = shmget(IPC_PRIVATE, sizeof(struct grid),0770);

    /*attach to the shared mem, and get the pointer to it*/
    goober = shmat(boardID, NULL, 0);
    initgrid(goober);
    //printgrid(goober);
    for (b = 0; b<1000;b++){
        pid = fork();
        if(pid==0){
            struct grid *childPointer=shmat(boardID, NULL, 0);
            for(p=0;p<50000;p++){
                 // Move cursor up 3 lines

                stupid(childPointer);
                //printgrid(childPointer);
            }
            shmdt(childPointer);
            exit(0);
        }else{
            //printf("%d\n",pid);
        }
    }
    while (wait(NULL) != -1) {
        // nothing
    }
    
	printf("Board ID = %d\n",boardID);
    printf("Total: %d\n",boardTotal(goober));
    printgrid(goober);
    shmctl(boardID, IPC_RMID, NULL);
	return 0;
}