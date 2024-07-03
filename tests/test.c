#include <stdio.h>
#include <string.h>

void test_compare() {
    char* echo = "/echo/abceasd";
    printf("compare '/echo' -> '%d'\n", strcmp("/echo", echo));
    printf("compare '/echo/abceasd' -> '%d'\n", 
           strcmp("/echo/abceasd", echo));
    printf("compare '/ec' -> '%d'\n", 
           strcmp("/ec", echo));
    printf("strstr '/echo' -> '%d'\n", strstr(echo, "/echo/") != NULL);
    printf("strstr '/ec' -> '%d'\n", 
           strstr(echo, "/ec/") != NULL);
}

void handle_echo(char* path){
    if (strstr(path, "/echo/") != NULL) {
        char* echo = path + sizeof("/echo/") - 1;
        printf("Echo: %s\n", echo);
        return;
    }
    printf("Nada\n");
}

int main() {
    handle_echo("/echo/akasjfd");
    handle_echo("/ec/akasjfd");
}

