//טליה רגינה מליכוב 310070412
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

void SIGUSR1_handler(int sig){
    char *tokptr; /* a strtok return value; */
    int proc_pid;  /* holds the pid of the client */
    int fd;
    char buf[1024]; 
    double num1;
    double num2;
    int expression;
    double result;
    char* end;

    signal(SIGUSR1,SIGUSR1_handler); 
    memset(buf,0,1024);
    sleep(1); 
    /* read the file content */ 

    int pid = fork();
	if(pid < 0){	exit(0);}

    if (pid == 0){
           
		
            fd=open("to_srv.csv",O_RDONLY);
            if(fd < 0)
            {
                 perror("cant open: com_file");
                 exit(0); }
            
            int readFile= read(fd, &buf, 1024);

            if (readFile ==-1)
	        {
                close(fd);
                exit(0);
            }

            close(fd);
           remove("to_srv.csv");
	
             /* parse file content */
            tokptr=strtok(buf," ");
            proc_pid = atoi(tokptr);
            tokptr=strtok(NULL," ");
            num1 = strtod(tokptr, &end);
            tokptr=strtok(NULL," ");
            expression = atoi(tokptr);
            tokptr=strtok(NULL," ");
            num2 = strtod(tokptr, &end);
		
            switch(expression) {

                case 1:
                    result=num1+num2;
                    break; 
	
                case 2:
                    result=num1-num2;
                    break; 
  
                case 3:
                    result=num1*num2;
                    break; 
  
                case 4:
                    result=num1/num2;
                    break; 

                default:
                    printf("error\n" );
  

            }


           /* reply with result */
         memset(buf,0,256);
         char id[256];
         sprintf(id, "%d", proc_pid);
	 char* toClient;
         toClient = (char*)calloc(15+strlen(id), sizeof(char));
         strcat(toClient, "to_client_");
         strcat(toClient,id);
         strcat(toClient, ".csv");
	 
         fd=open(toClient, O_WRONLY |O_CREAT|O_TRUNC,S_IRWXU); 
         if( fd<0 )
          {
             perror("cant open: com_file");
             exit(0); }   

            sprintf(buf,"%lf",result);
            write(fd,buf,strlen(buf)); 
            close(fd);
            kill(proc_pid,SIGUSR1);  

           

    }
    

}



int main(int argc, char **argv){

    remove("to_srv.csv");
    signal(SIGUSR1,SIGUSR1_handler); /* handles signal from other process */
    printf("server pid is= %d\n",getpid());
    while (1){
    	pause(); /* waiting for other to signal */
}
}

