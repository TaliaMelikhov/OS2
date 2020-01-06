//טליה רגינה מליכוב 310070412
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

int proc_pid;
int proc_serv;

/* handles signal from other process */
void SIGUSR1_handler(int sig) {


    int fd;
    char buf[256];
    int tmp;
    double num;
    char* end;
    char id[256];
    char* toClient;
    signal(SIGUSR1,SIGUSR1_handler);
    printf("pid=%d receives a signal %d\n",getpid(),sig); 
    sleep(1);

    sprintf(id, "%d", proc_pid);
    toClient = (char*)calloc(15+strlen(id), sizeof(char));
    strcat(toClient, "to_client_");
    strcat(toClient,id);
    strcat(toClient, ".csv");
     
    fd=open(toClient,O_RDONLY);
    if (fd < 0)
    {
       perror("cant open");
	
    	exit(0); 
    }
    
    if (read(fd,buf,256) > 0) {
        num = strtod(buf, &end);
        printf("received: %.2lf\n",num);}
    close(fd);
    remove(toClient);
    
}


void sendReq(char *data) {
       
        int fd;
        int fd1;
	int i=1;
        fd1=open("to_srv.csv",O_WRONLY);
        while(fd1>0 && i<10){
	sleep(2);
	i++;
        fd1=open("to_srv.csv",O_WRONLY);
	}
	
        if(i==10){
	perror("cant create to_srv file, its already exist");
	exit(0);
	}




    
    fd=open("to_srv.csv",O_WRONLY |O_CREAT|O_TRUNC|O_SYNC,S_IRWXU); 
    if(fd < 0)
    {
        perror("cant open");
        exit(0); 
    }
    write(fd,data,strlen(data)); 
    close(fd);
    kill(proc_serv,SIGUSR1);
}


int main(int argc, char **argv){

    
    if (argc!=5) {
                perror("number of arguments doesn't match");
		exit(0);
	}

 char buf[1024];
    double num1;
    double num2;
    int expression;
    double result;
    char* end;
      if(strtod(argv[4], &end)==0 && atoi(argv[3])==4)
	{
	perror("can't divide by zero");
	exit(0);
	}


    signal(SIGUSR1,SIGUSR1_handler); /* handles signal from other process */
    proc_serv= atoi(argv[1]);
    proc_pid = getpid();
    num1 = strtod(argv[2], &end);
    expression = atoi(argv[3]);
    num2 = strtod(argv[4], &end);
    memset(buf,0,256);
    sprintf(buf,"%d %lf %d %lf\0", proc_pid, num1, expression, num2); 
    sendReq(buf);
    pause(); /* wait for result */ 
    sleep(1);
    memset(buf,0,256);


}

