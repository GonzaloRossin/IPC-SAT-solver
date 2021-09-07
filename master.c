#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/select.h>

typedef struct
{
    pid_t pid;
    int sender;    // Devuelve resultados, slaveToMaster.
    int receiver; // Manda tarea, masterToSlave.
    int ntasks;        // Tareas siendo ejecutadas por el esclavo.
    int flagEOF;
} slave_t;
#define SLAVE_INIT 5
#define SLAVE_COUNT(c) ((c<SLAVE_INIT)? c : SLAVE_INIT) 
#define READ 0
#define WRITE 1
#define PATH_SLAVE "./Slave"
#define INITIAL_TASKS 1
#define BUFFER_SIZE 4000
#define HANDLE_ERROR(msg)   \
    do                      \
    {                       \
        perror(msg);        \
        exit(EXIT_FAILURE); \
    } while (0)

int createSlaves(char** paths,int dimSlaves, slave_t slaves[]);
int getRunningSlaves(slave_t slaves[],int slaveCount);

int main(int argc,char** argv){

    int totalTasks=argc-1,completedTasks=0,runningSlaves;
    
    char** paths=argv;
    int slaveCount=SLAVE_COUNT(totalTasks);
    slave_t slaves[slaveCount];
    createSlaves(paths,slaveCount,slaves);
    runningSlaves=getRunningSlaves(slaves,slaveCount);
    fd_set readSet;
    while(runningSlaves>0){
        char buffer[BUFFER_SIZE]={0};
        FD_ZERO(&readSet);
        int nfds=0, i, j;
        for(i=0;i<slaveCount;i++){
            slave_t slave=slaves[i];
            if (slave.flagEOF==0){
                FD_SET(slaves[i].sender,&readSet);
                if(nfds<=slaves[i].sender)
                    nfds=slaves[i].sender;
            }
        }
        int retval=select(nfds+1,&readSet,NULL,NULL,NULL);
        if(retval==-1){
            HANDLE_ERROR("Error at select function");
        }
        for(j=0;j<slaveCount;j++){
            if(FD_ISSET(slaves[j].sender,&readSet)){
                int bytesRead=read(slaves[j].sender,buffer,BUFFER_SIZE);
                if(bytesRead==-1){
                HANDLE_ERROR("error at reading from slave");
                }
                if(bytesRead==0){
                    slaves[j].flagEOF=1;
                }
                buffer[bytesRead]=0;
                printf("%s\n",buffer);
            }
        }
        //runningSlaves=getRunningSlaves(slaves,slaveCount);
    }
    return 0;
}

int createSlaves(char** paths,int dimSlaves, slave_t slaves[])
{
    int i;
    for (i = 0; i < dimSlaves; i++)
    {
        int masterToSlave[2],slaveToMaster[2];
        pipe(masterToSlave);
        if (pipe(masterToSlave) < 0)
        {
            HANDLE_ERROR("Pipe Error Master");
        }
        slaves[i].receiver = masterToSlave[WRITE];

        pipe(slaveToMaster);
        if (pipe(slaveToMaster) < 0)
        {
            HANDLE_ERROR("Pipe Error Slave ");
        }
        slaves[i].sender = slaveToMaster[READ];
        slaves[i].flagEOF = 0;
        int pid;
        if ((pid = fork()) == -1)
        {
            HANDLE_ERROR("Fork Error");
        }
        else if (pid == 0)
        {
            if (close(masterToSlave[WRITE]) < 0)
            {
                HANDLE_ERROR("Error closing slave WRITE to master");
            }
            if (dup2(masterToSlave[READ],STDIN_FILENO) < 0)//REDIRECCION STDIN DEL SLAVE AL PIPE
            {
                HANDLE_ERROR("Error dupping pipe");
            }

            if (close(slaveToMaster[READ]) < 0)
            {
                HANDLE_ERROR("Error closing slave WRITE to master");
            }
            if (dup2(slaveToMaster[WRITE],STDOUT_FILENO) < 0) //REDIRECCION STDOUT DEL SLAVE AL PIPE
            {
                HANDLE_ERROR("Error dupping pipe");
            }
            char* slaveArguments[3];
            slaveArguments[0]=PATH_SLAVE;
            slaveArguments[1]=paths[i+1];
            slaveArguments[2]=NULL;
            if (execv(PATH_SLAVE, slaveArguments) < 0)
            {
                HANDLE_ERROR("Execv Error");
            }
        }
        else if (pid < 0)
        {
            HANDLE_ERROR("Fork Error");
        }
        if (close(masterToSlave[READ]) < 0)
        {
            HANDLE_ERROR("Error closing master READ to slave");
        }
        if (close(slaveToMaster[WRITE]) < 0)
        {
            HANDLE_ERROR("Error closing slave WRITE to master");
        }
        if(pid>0)
            slaves[i].pid = pid;
    }
    return 0;
}
int getRunningSlaves(slave_t slaves[],int slaveCount){
    int result=0, i;
    for(i=0;i<slaveCount;i++){
        if(!slaves[i].flagEOF)
            result++;
    }
    return result;   
}