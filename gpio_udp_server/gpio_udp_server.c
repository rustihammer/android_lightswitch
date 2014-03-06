/* Sample UDP server */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <wiringPi.h>
#include <sys/wait.h>

/* minimum required number of parameters */
#define MIN_REQUIRED 0
#define VERSION "v1.2"
#define BUILD_DATE "May 05, 2014"

/* display usage */
void help() {
   printf("Usage: gpio_udp_server [options]\n");
   printf(" Options:\n");
   printf("\t--help: This help text\n");
   printf("\t--version: Display version\n"); 
}

int main(int argc, char**argv)
{

	if (argc > 1) {
		if (strcmp("--help", argv[1]) == 0) {
			help();
			return 1;
		}
		if (strcmp("--version", argv[1]) == 0) {
			printf("Version %s compiled at %s\n", VERSION, BUILD_DATE);
			return 1;
		}
	}
	

	int sockfd,n;
	struct sockaddr_in servaddr,cliaddr;
	socklen_t len;
	char mesg[1000];
	int GPIO1_status;
	pid_t childpid; /* variable to store the child's pid */
	int status;     /* parent process: child's exit status */

	wiringPiSetup();
	pinMode (1, OUTPUT) ;

	sockfd=socket(AF_INET,SOCK_DGRAM,0);

	bzero(&servaddr,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
	servaddr.sin_port=htons(32000);
	bind(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr));

	printf("Starting GPIO udp server!!\n");

   for (;;)
   {
      len = sizeof(cliaddr);
      n = recvfrom(sockfd,mesg,1000,0,(struct sockaddr *)&cliaddr,&len);
      sendto(sockfd,mesg,n,0,(struct sockaddr *)&cliaddr,sizeof(cliaddr));
      mesg[n] = 0;

        if(strcmp(mesg, "on\n") == 0) {
	        printf("Recevied command: %s",mesg);
                digitalWrite (1, LOW) ;
        } else if(strcmp(mesg, "off\n") == 0){
	        printf("Recevied command: %s",mesg);
                digitalWrite (1, HIGH) ;
        } else if(strcmp(mesg, "toggle\n") == 0){
	        printf("Recevied command: %s",mesg);
		GPIO1_status = digitalRead(1);
		if (GPIO1_status == 0)
			digitalWrite (1, HIGH) ;
		else
			digitalWrite (1, LOW) ;

        } else if(strcmp(mesg, "on_for_2_min\n") == 0){

	        printf("Recevied command: %s",mesg);
                childpid = fork();

           if (childpid >= 0) /* fork succeeded */
            {
                if (childpid == 0) /* fork() returns 0 to the child process */
                {
                    digitalWrite (1, LOW) ;
                    printf("CHILD: I am the child process!\n");
                    printf("CHILD: Sleeping for 2 mins...\n");
                    sleep(5); /* sleep for 1 second */
                    exit(0); /* child exits with user-provided return code */
                }
                else /* fork() returns new pid to the parent process */
                {
                    wait(&status); /* wait for child to exit, and store its status */
                    digitalWrite (1, HIGH) ;
                    printf("PARENT: I am the parent process!\n");
                }
            }
            else /* fork returns -1 on failure */
            {
                perror("fork"); /* display error message */
                exit(0);
            }

        } else {
             printf("Do not understand your command: %s\n",mesg);
        }

	printf("Recevied command: %s",mesg);
   }
}
