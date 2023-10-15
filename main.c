#include "stdio.h"
#include "stdlib.h"
#include <sys/types.h>
#include <signal.h>
#include "unistd.h"
#include "sys/wait.h"
#include "string.h"
#include <time.h>

const int STRING_SIZE = 100;
#define _POSIX_SOURCE

int create_process();
int generator();
void create_pipe(int* pipe_fd);

int main(){
    srand(time(NULL));

    printf("Создайте 2 файла\n");
    char file_name1[STRING_SIZE], file_name2[STRING_SIZE];
    fgets(file_name1, STRING_SIZE, stdin);
    fgets(file_name2, STRING_SIZE, stdin);
    char* file_names[] = {file_name1, file_name2};

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
            printf("Введите строки\n");
            while(fgets(new_string, STRING_SIZE, stdin)){
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
            printf("~~~~~~~~~~~~~~~\n");
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

//     FILE *file_1;
//     char firsr_name_f[STRING_SIZE];
//     gets(firsr_name_f);
//     strcat(firsr_name_f, ".txt");
//     file_1 = fopen(firsr_name_f, "a+"); //Дописывает информацию к концу файла или создает файл для чтения/записи (по умолчанию открывается как текстовый файл).

//     FILE *file_2;
//     char second_name_f[STRING_SIZE];
//     gets(second_name_f);
//     strcat(second_name_f, ".txt");
//     file_2 = fopen(second_name_f, "a+"); //Дописывает информацию к концу файла или создает файл для чтения/записи (по умолчанию открывается как текстовый файл).
// //создали два файла

// int main(){
//     int r = 0;
    
//     printf("Создайте 2 файла\n");
//     char file_name1[STRING_SIZE], file_name2[STRING_SIZE];
//     fgets(file_name1, STRING_SIZE, stdin);
//     fgets(file_name2, STRING_SIZE, stdin);
//     char* file_names[] = {file_name1, file_name2};

//     int pipe_fd_children_1[2]; create_pipe(pipe_fd_children_1);
//     int pipe_fd_children_2[2]; create_pipe(pipe_fd_children_2);

//     pid_t pid_1 = create_process();//первый ребенок
//     if(pid_1 == 0){
//         char* ch_str[STRING_SIZE];

//         close(pipe_fd_children_2[1]);
//         close(pipe_fd_children_2[0]);
//         close(pipe_fd_children_1[1]);
//         read(pipe_fd_children_1[0], &ch_str, STRING_SIZE * sizeof(char));
        
//         printf("first_child %s", ch_str);

//         close(pipe_fd_children_1[0]);
//     }else{
//         pid_t pid_2 = create_process();//второй ребенок
//         if(pid_2 == 0){

//             char* ch_str[STRING_SIZE];

//             close(pipe_fd_children_1[1]);
//             close(pipe_fd_children_1[0]);
//             close(pipe_fd_children_2[1]);
//             read(pipe_fd_children_2[0], &ch_str, STRING_SIZE * sizeof(char));
        
//             printf("second_child %s", ch_str);
//             close(pipe_fd_children_2[0]);
//         }else{
//             char* new_srting[STRING_SIZE];
//             printf("Введите строку\n");
//             while(fgets(new_srting, STRING_SIZE, stdin)){
//                 //read_input(new_srting, STRING_SIZE);
//                 r = generator();
//                 printf("random = %d\n", r);
//                 if(r < 81){
//                     // close(pipe_fd_children_2[1]);
//                     // close(pipe_fd_children_2[0]);
//                     close(pipe_fd_children_1[0]);//close to read
//                     write(pipe_fd_children_1[1], &new_srting, STRING_SIZE * sizeof(char));
//                     //kill(pid_2, SIGKILL);
//                     close(pipe_fd_children_1[1]);//close to write
//                     printf("Отправили в 1го ребенка\n");
//                     wait(NULL); 
//                 }else{
//                     // close(pipe_fd_children_1[1]);
//                     // close(pipe_fd_children_1[0]);
//                     close(pipe_fd_children_2[0]);//close to read
//                     write(pipe_fd_children_2[1], &new_srting, STRING_SIZE * sizeof(char));
//                     kill(pid_1, SIGKILL);
//                     close(pipe_fd_children_2[1]);//close to write
//                     //printf("Отправили в 2го ребенка\n");
//                     wait(NULL);
                
//             }
//             }
            

            
//             printf("The child process done a work and finished. Goodbye!\n");   
//         }
//     }
//     return 0;
// }