#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <signal.h>

#define MAX_BUF 1024
#define PORT 50005
#define MAX_WAIT 120

int main(int argc, char *argv[])
{
	int sock, elkarrizketa, n;
	struct sockaddr_in zerb_helb, bez_helb;
	socklen_t helb_tam;
	char buf[MAX_BUF],comandoa[5];
	struct timeval timer;
	fd_set rset;

	if((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		perror("Errorea socketa sortzean");
		exit(1);
	}
	
	memset(&zerb_helb, 0, sizeof(zerb_helb));
	zerb_helb.sin_family = AF_INET;
	zerb_helb.sin_addr.s_addr = htonl(INADDR_ANY);
	zerb_helb.sin_port = htons(PORT);
	
	if(bind(sock, (struct sockaddr *) &zerb_helb, sizeof(zerb_helb)) < 0)
	{
		perror("Errorea socketari helbidea esleitzean");
		exit(1);
	}
	
	signal(SIGCHLD, SIG_IGN);
	
        
        
	while(1)
	{
		helb_tam = sizeof(bez_helb);
                
                
		if((n=recvfrom(sock, buf, MAX_BUF, 0, (struct sockaddr *) &bez_helb, &helb_tam)) !=0)
		{
			perror("Errorea lehen mezua jasotzean");
			exit(1);
		}
		
		
		//if(n==0)continue;
		
		if(fork() == 0)	// Prozesu umeak egin beharrekoa.
		{
			close(sock);
			if((elkarrizketa = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
			{
				perror("Errorea prozesu umearen socketa sortzean");
				exit(1);
			}
			
			if(connect(elkarrizketa, (struct sockaddr *) &bez_helb, helb_tam) < 0)
			{
				perror("Errorea bezeroarekin konektatzean");
				exit(1);
			}
			
			do
			{
                            
                                if((n=recvfrom(elkarrizketa, buf, MAX_BUF, 0, (struct sockaddr *) &bez_helb, &helb_tam)) < 0)
                                {
                                    perror("Errorea mezua jasotzean");
                                    exit(1);
                                }
                                
                                strncpy(comandoa,buf,4);
                                
                                switch(comandoa){
                                
                                    case ERRG: printf("pipopipo\n");break;
                                
                                    case USER: printf("USEEER !!");break;
                                }
				/**if(write(elkarrizketa, buf, n) < n)
				{
					perror("Errorea erantzuna bidaltzean");
					exit(1);
				}
			
				timer.tv_sec = MAX_WAIT;
				timer.tv_usec = 0;
				FD_ZERO(&rset);
				FD_SET(elkarrizketa, &rset);
				if((n=select(elkarrizketa+1,&rset,NULL,NULL,&timer)) < 0)
				{
					perror("Errorea mezuaren zain");
					exit(1);
				}
				else if(n == 0)
				{
					fprintf(stderr,"Itxoite denbora maximoa (%d s.) agortuta. Bezeroa bukatutzat jo da.",MAX_WAIT);
					exit(0);
				}*/
			
                            
                            
                        } while((n=read(elkarrizketa, buf, MAX_BUF)) > 0);
			
                        
                        close(elkarrizketa);
                        
                        
			if(n < 0)
			{
				perror("Errorea mezua jasotzean");
				exit(1);
			}
			exit(0);
		}
	}
}
