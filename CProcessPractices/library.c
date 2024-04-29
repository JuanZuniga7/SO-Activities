#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/stat.h>
#include <stdbool.h>

int createSHM(size_t const size) {
    int const shm_id = shmget(IPC_PRIVATE, size, IPC_CREAT | S_IRUSR | S_IWUSR);
    return shm_id;
}

void tree(const pid_t pid){
    char command[50];
    sprintf(command, "pstree -pl %d", pid);
    system(command);
}

bool validateFather(const pid_t pid) {
    return pid == getpid();
}

pid_t createProcess(void) {
    const pid_t pid = fork();
    if(pid == -1){
        printf("Error: Unable to create a new process\n");
        exit(EXIT_FAILURE);
    }
    return pid;
}

void main(int argc, char **argv) {
    const int shm_id = createSHM(sizeof(pid_t) * 9);
    pid_t *pointer = (pid_t *) shmat(shm_id, 0, 0);
    pointer[0] = getpid();
    pointer[1] = createProcess();
    if(!validateFather(pointer[0])){
        pointer[1] = getpid();
        pointer[6] = createProcess();
    }
    if(validateFather(pointer[0])){
        for (int i = 2; i < 6; ++i) {
            if(validateFather(pointer[0])) {
                pointer[i] = createProcess();
                if (!validateFather(pointer[0])) {
                    pointer[i] = getpid();
                    if(i == 3){
                        pointer[7] = createProcess();
                    }
                    if(i == 5){
                        pointer[8] = createProcess();
                    }
                    break;
                }
            }else {
                break;
            }
        }
    }
    if(validateFather(pointer[0])){
        tree(pointer[0]);
    }else{
        pause();
    }
    exit(EXIT_SUCCESS);
}
