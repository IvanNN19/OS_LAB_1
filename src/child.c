#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "sys/wait.h"
#include "string.h"

const int MAX_LENGTH = 100;

int main(int argc, char* argv[]) {
    char* filename[MAX_LENGTH];
    strcpy(filename, argv[1]);

    // Перенаправили вывод в файл
    int file_descriptor = fileno(filename);
    dup2(file_descriptor, 1);
    printf("```Начало работы```\n");
    char vowels[] = {'a', 'e', 'i', 'o', 'u', 'y'};
    char string[MAX_LENGTH];
    char eof = 1;
    while (fgets(string, MAX_LENGTH, stdin)) {
        if (string[0] == eof) {
            close(file_descriptor);
            fclose(filename);
            return 0;
        }

        for (int index = 0; index < strlen(string); ++index) {
            if (memchr(vowels, string[index], 6) == NULL) {
                printf("%c", string[index]);
            }
        }
    }
}





// #include "stdio.h"
// #include "stdlib.h"
// #include "unistd.h"
// #include "string.h"
// //#include "sys/wait.h"

// #define STRING_SIZE 100

// void read_input(char *buffer, size_t bufferSize);

// int main(){
//     int i = 0;
//     char gl[] = {'a', 'e', 'i', 'o', 'u', 'y'};
//     char filename[] = "one";
//     char ch_str[STRING_SIZE];
//     read_input(ch_str, STRING_SIZE);

//     //меняем стандартный ввод на файловый
//     FILE* file = fopen(filename, "w");
//     int file_descriptor = fileno(file);//what????
//     dup2(file_descriptor, 1);//what??????

//     while(ch_str){
//         if(ch_str[i] == '\n'){
//             printf("\n");
//             break;
//         }
//         if(memchr(gl, ch_str[i], 6) == NULL){
//             printf("%c", ch_str[i]);
//         }
//         i++;

//     }

//     return 0;
// }

// void read_input(char *buffer, size_t bufferSize) {
//     ssize_t bytesRead;
//     bytesRead = read(STDIN_FILENO, buffer, bufferSize - 1);

//     if (bytesRead == -1) {
//         perror("Read error");
//         exit(-1);
//     }

//     for (int i = bytesRead; i < bufferSize; i++) {
//         buffer[i] = 0;
//     }
// }