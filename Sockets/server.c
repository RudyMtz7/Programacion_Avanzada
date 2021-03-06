
/*-----------------------------------------------------------

* Programación avanzada: Echo server

* Fecha: 26-Abr-2019

* Autor: A01701260 Luis Alfredo Rodriguez Santos

*------------------------------------------------------------*/

#include "header.h"
#include <string.h>

void serves_client(int nsfd) {
	char message_sent[50];
	char answer[50];
	
	srand(getpid());
	// do {
	// 	guess = 0;
	// 	number_guess = (rand() % 100) + 1;
	// 	printf("PID = %i <-> number to  guess = %i\n", getpid(), number_guess);
	// 	while (!guess) {
	// 		read(nsfd, &number_sent, sizeof(number_sent));
	// 		if (number_sent < number_guess) {
	// 			answer = SMALLER;
	// 		} else if (number_sent > number_guess) {
	// 			answer = BIGGER;
	// 		} else {
	// 			answer = EQUAL;
	// 			guess = 1;
	// 		}
	// 		write(nsfd, &answer, sizeof(answer));
	// 	}
	// 	read(nsfd, &answer, sizeof(answer));
	// } while (answer == CONTINUE);

    read(nsfd, &message_sent, sizeof(message_sent));
    write(nsfd, &message_sent, sizeof(message_sent));
	close(nsfd);
}

void server(char* ip, int port, char* program) {
	int sfd, nsfd, pid;
	struct sockaddr_in server_info, client_info;

	if ( (sfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
		perror(program);
		exit(-1);
	}
	
	server_info.sin_family = AF_INET;
	server_info.sin_addr.s_addr = inet_addr(ip);
	server_info.sin_port = htons(port);
	if ( bind(sfd, (struct sockaddr *) &server_info, sizeof(server_info)) < 0 ) {
		perror(program);
		exit(-1);
	}
	
	listen(sfd, 1);
	while (1) {
		int len = sizeof(client_info);
		if ( (nsfd = accept(sfd, (struct sockaddr *) &client_info, &len)) < 0 ) {
			perror(program);
			exit(-1);
		}
		
		/* CONCURRENTE
		if ( (pid = fork()) < 0 ) {
			perror(program);
		} else if (pid == 0) {
			close(sfd);
			serves_client(nsfd);
			exit(0);
		} else {
			close(nsfd);
		}
		CONCURRENTE */
		
		serves_client(nsfd); // ITERATIVO
	}
}

int main(int argc, char* argv[]) {
	char ip[15];	
	int port;
	
	strcpy(ip, DEFAULT_IP);
	port = DEFAULT_PORT;
	if (argc == 3) {
		if (strcmp(argv[1], "-d") == 0) {
			strcpy(ip, argv[2]);
		} else if (strcmp(argv[1], "-p") == 0) {
			port = atoi(argv[2]);
			if (port < 5000) {
        		printf("%s: The port must be greater than 5000.\n", argv[0]);
        		return -1;
        	}
		} else {
			printf("usage: %s [-d dir] [-p port]\n", argv[0]);
			return -1;
		}
	} else if (argc == 5) {
		if (strcmp(argv[1], "-d") == 0) {
			strcpy(ip, argv[2]);
			if (strcmp(argv[3], "-p") == 0) {
				port = atoi(argv[4]);
				if (port < 5000) {
            		printf("%s: The port must be greater than 5000.\n", argv[0]);
            		return -1;
            	}		
			} else {
				printf("usage: %s [-d dir] [-p port]\n", argv[0]);
				return -1;
			}			
		} else if (strcmp(argv[1], "-p") == 0) {
			port = atoi(argv[2]);
			if (port < 5000) {
        		printf("%s: The port must be greater than 5000.\n", argv[0]);
        		return -1;
        	}		
			if (strcmp(argv[3], "-d") == 0) {
				strcpy(ip, argv[4]);
			} else {
				printf("usage: %s [-d dir] [-p port]\n", argv[0]);
				return -1;
			}
		} else {
			printf("usage: %s [-d dir] [-p port]\n", argv[0]);
			return -1;
		}
	} else if (argc != 1) {
		printf("usage: %s [-d dir] [-p port]\n", argv[0]);
		return -1;
	}
	
	server(ip, port, argv[0]);
	
	return 0;
}