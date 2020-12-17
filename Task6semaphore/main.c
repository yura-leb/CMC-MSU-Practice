#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/sem.h>
#include <sys/ipc.h>

int main(int argc, char const *argv[])
{
	key_t key;
	int semid, pid;
	struct sembuf semb[] = {{0, 0, 0}, {1, 0, 0}, {2, 0, 0}, {3, 0, 0}};
	key = ftok("Документы/Task6/test.txt", 'o');
	semid = semget(key, 4, 0644 | IPC_CREAT);
	semctl(semid, 0, SETVAL, (int) 0);
	semctl(semid, 1, SETVAL, (int) 0);
	semctl(semid, 2, SETVAL, (int) 0);
	semctl(semid, 3, SETVAL, (int) 0);
	int i = 0;
	while (i < 10) {
		semb[2].sem_op = -2;
		semb[3].sem_op = -2;
		semop(semid, &semb[2], 2);
		printf("Product is made\n");
		semb[0].sem_op = -1;
		semb[1].sem_op = -1;
		semb[2].sem_op = -1;
		semb[3].sem_op = -1;
		semop(semid, semb, 4);
		i++;
	}	
	semctl(semid, 0, IPC_RMID, (int) 0);
	semctl(semid, 1, IPC_RMID, (int) 0);
	semctl(semid, 2, IPC_RMID, (int) 0);
	semctl(semid, 3, IPC_RMID, (int) 0);

	return 0;
}