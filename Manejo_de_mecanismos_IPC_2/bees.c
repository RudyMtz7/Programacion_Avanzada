/*----------------------------------------------------------------

*

* Programación avanzada: El oso y las abejas.

* Fecha: 12-Abril-2018

* Autor: A01701260 Luis Alfredo Rodriguez Santos

*

*--------------------------------------------------------------*/


#include "header.h"
#include <time.h>

void a_Bee(char* program) {
	int semid, i, k;
	key_t key;
	
	if ( (key = ftok("/dev/null", 65)) == (key_t) -1 ) {
		perror(program);
		exit(-1);
	}
	
	if ( (semid = semget(key, 4, 0666))  < 0 ) {
		perror(program);
		exit(-1);
	}
	
	srand( getpid() );
	k = 10;
	while (k-- > 0) {
		printf("Bee %i trying to access the buffer.\n", getpid());
		mutex_wait(semid, BEES);
		printf("Bee %i accessing the buffer.\n", getpid());
		
		i = (rand() % 5) + 1;
		printf("Bee %i trying to put %i product(s) - free space = %i.\n", getpid(), i, semctl(semid, FREESPACE, GETVAL, 0));
		sem_wait(semid, FREESPACE, i);
		printf("Bee %i put their products.\n", getpid());
		sem_signal(semid, OCCUPIED, i);
		
		mutex_signal(semid, BEES);
		printf("Bee %i is going to sleep.\n", getpid());
		
		sleep((rand() % 10) + 1);
	}
	exit(0);
}

int main(int argc, char* argv[]) {
	int amount = 0, semid, i, pid;
	key_t key;
	
	if (argc != 2) {
		printf("usage: %s amount\n", argv[0]);
		return -1;
	}
	
	amount = atoi(argv[1]);
	if (amount < 1) {
		printf("%s: The amount must be a positive number greater than zero.\n", argv[0]);
		return -1;
	}
	
	for (i = 0; i < amount; i++) {
		if ( (pid = fork()) < 0 ) {
			perror("fork");
			return -1;
		} else if (pid == 0) {
			a_Bee(argv[0]);
		} else {
		}
	}
	return 0;
}