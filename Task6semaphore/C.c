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
	int i = 0;
	while (i < 10) {
		semb[3].sem_op = 3;
		sleep(4);
		printf("C is made\n");
		semop(semid, &semb[3], 1);
		semb[3].sem_op = 0;
		semop(semid, &semb[3], 1);
		i++;
	}
	return 0;
}