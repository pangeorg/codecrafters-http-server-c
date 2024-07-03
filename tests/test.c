#include <stdlib.h>
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

char* read_header_value(char* key, char* headers) {
    // find header key
    char *key_start = strstr(headers, key);
    if (key_start == NULL) return NULL;
    printf("keystart\n'%s'\n", key_start);

    // find value
    char* value_start = strstr(key_start, ": ");
    if (value_start == NULL) return NULL;
    value_start += 2;
    printf("valstart\n'%s'\n", value_start);

    int len = 0;
    while (value_start[len] != '\r' && value_start[len] != '\n') {
        len++;
    }
    printf("len of value: %d\n", len);

    char* value = malloc(sizeof(char) * len);
    memcpy(value, value_start, len);
    printf("value: '%s'\n", value);

    return value;
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
    read_header_value("User-Agent", "Host: localhost:4221\r\nUser-Agent: foobar/1.2.3\r\nAccept: */*\r\n");
}

