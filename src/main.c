#include "stdio.h"
#include "stdlib.h"
#include <sys/types.h>
#include <signal.h>
#include "unistd.h"
#include "sys/wait.h"
#include "string.h"
#include <time.h>
#include <fcntl.h>
#include <sys/stat.h>

const int STRING_SIZE = 100;
#define _POSIX_SOURCE

int create_process();
int generator();
void create_pipe(int* pipe_fd);
void read_input(char *buffer, size_t bufferSize);

int main(){
    int i = 0;
    srand(time(NULL));

    //printf("Создайте 2 файла\n");
    write(1, "Создайте 2 файла\n", sizeof(char) * strlen("Создайте 2 файла\n"));
    char file_name1[STRING_SIZE], file_name2[STRING_SIZE];
    read(STDIN_FILENO, file_name1, STRING_SIZE);
    read(STDIN_FILENO, file_name2, STRING_SIZE);
    char* file_names[] = {file_name1, file_name2};

    int filenam1 = open(file_name1, O_WRONLY);
    int filenam2 = open(file_name2, O_WRONLY);

    //int file_descriptor_1 = fileno(filenam1);
    //int file_descriptor_2 = fileno(filenam2);

    int pipe_fd_children_1[2]; create_pipe(pipe_fd_children_1);
    int pipe_fd_children_2[2]; create_pipe(pipe_fd_children_2);

    pid_t pid_1 = create_process();//первый ребенок
    if (pid_1 == 0) {
        dup2(pipe_fd_children_1[0], 0);
        close(pipe_fd_children_1[1]);
        execl("child", "", file_names[0], NULL);
        perror("exec");
        exit(-1);
    } else {
        pid_t pid_2 = create_process();//второй ребенок
        if (pid_2 == 0) {
            dup2(pipe_fd_children_2[0], 0);
            close(pipe_fd_children_2[1]);
            execl("child", "", file_names[1], NULL);
            perror("exec");
            exit(-1);
        } else {
            close(pipe_fd_children_1[0]);
            close(pipe_fd_children_2[0]);
    
            char new_string[STRING_SIZE];
            char c;
            write(1, "Введите строки\n", sizeof(char) * strlen("Введите строки\n"));
            //printf("Введите строки\n");
            while(read(0, &c, 1) != 0){
                read_input(new_string, STRING_SIZE);
                if (new_string[ strlen(new_string) - 1 ] != '\n')
                    new_string[ strlen(new_string) ] = '\n';        
                int r = generator();
                if (r < 81) {
                    write(pipe_fd_children_1[1], new_string, sizeof(char) * strlen(new_string));
                } else {
                    write(pipe_fd_children_2[1], new_string, sizeof(char) * strlen(new_string));
                }
            }
            char eof[2];
            eof[0] = 1;
            eof[1] = '\n';
            write(pipe_fd_children_1[1], eof, sizeof(char) * strlen(eof));
            write(pipe_fd_children_2[1], eof, sizeof(char) * strlen(eof));

            write(1, "конец работы :)\n", sizeof(char) * strlen("конец работы :)\n"));
            //printf("конец работы :)\n");
            wait(NULL); 
        }
    }
    return 0;
}



int create_process() {
    pid_t pid = fork();
    if (-1 == pid)
    {
        perror("Error while fork");
    }
    return pid;
}

void create_pipe(int* pipe_fd) {
    if (pipe(pipe_fd) == -1)
    {
        perror("Failed to create pipe");
        exit(-1);
    }
}

int generator(){
    int rng = rand() % 100;
    return rng;
}

void read_input(char *buffer, size_t bufferSize) {
    ssize_t bytesRead;
    bytesRead = read(STDIN_FILENO, buffer, bufferSize - 1);

    if (bytesRead == -1) {
        perror("Read error");
        exit(-1);
    }

    for (int i = bytesRead; i < bufferSize; i++) {
        buffer[i] = 0;
    }
}