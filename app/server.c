#define _GNU_SOURCE 
#include <assert.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

const char* HTTP_VERSION = "HTTP/1.1";

enum Method {
    GET,
    POST,
};

enum Method parse_method(char* str) {
    if (strcmp(str, "GET") == 0) {
        return GET;
    }
    else {
        return POST;
    }
}

typedef struct {
    char* name;
    char* value;
} Header;

typedef struct {
    enum Method method;
    char* target;
    char* version;
    char* headers;
    char* body;
} Request;


int parse_request(Request* request, char* buffer){
    int offset = 0;
    char* method = strtok(buffer, " ");
    offset += strlen(method) + 1;

    char* path = strtok(0, " ");
    offset += strlen(path) + 1;

    char* version = strtok(0, "\r\n");
    offset += strlen(version) + 2;

    char* header = &buffer[offset];

    char* p = strstr(header, "\r\n\r\n");
    if (p == NULL) {
        printf("Malformed request");
        return - 1;
    }
    int pos = p - header;
    header[pos] = 0;
    char* body = &header[pos + 4];

    request->method = parse_method(method);
    request->version = version;
    request->target = path;
    request->headers = header;
    request->body = body;
    return 0;
}

typedef struct {
    int a;

} Response;


int main() {
    // Disable output buffering
    setbuf(stdout, NULL);
    setbuf(stderr, NULL);

    // You can use print statements as follows for debugging, they'll be visible when running tests.
    printf("Logs from your program will appear here!\n");

    // Uncomment this block to pass the first stage

    int server_fd;
    socklen_t client_addr_len;
    struct sockaddr_in client_addr;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        printf("Socket creation failed: %s...\n", strerror(errno));
        return 1;
    }

    // Since the tester restarts your program quite often, setting SO_REUSEADDR
    // ensures that we don't run into 'Address already in use' errors
    int reuse = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
        printf("SO_REUSEADDR failed: %s \n", strerror(errno));
        return 1;
    }

    struct sockaddr_in serv_addr = { .sin_family = AF_INET ,
                                     .sin_port = htons(4221),
                                     .sin_addr = { htonl(INADDR_ANY) },
                                    };

    if (bind(server_fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) != 0) {
        printf("Bind failed: %s \n", strerror(errno));
        return 1;
    }

    int connection_backlog = 5;
    if (listen(server_fd, connection_backlog) != 0) {
        printf("Listen failed: %s \n", strerror(errno));
        return 1;
    }

    printf("Waiting for a client to connect...\n");
    client_addr_len = sizeof(client_addr);

    int fd = accept(server_fd, (struct sockaddr *) &client_addr, &client_addr_len);
    printf("Client connected\n");

    char buffer[1024];
    int received = recv(fd, &buffer, 1024, 0);
    if (received == 0) {
        goto END;
    }
    else if (received < 0) {
        printf("Error receiving data: %s \n", strerror(errno));
        return 1;
    }
    assert(received < 1024);

    Request request = {0};
    printf("Parsing Request...\n%s\n", buffer);
    int parse_result = parse_request(&request, &buffer[0]);
    if (parse_result < 0) goto END;


    char *response;
    printf("Target: %s\n", request.target);
    if (strcmp(request.target, "/index.html") == 0) {
        response = "HTTP/1.1 200 OK\r\n\r\n";
    }
    else if (strcmp(request.target, "/") == 0) {
        response = "HTTP/1.1 200 OK\r\n\r\n";
    }
    else if (strstr(request.target, "/echo/") != NULL) {
        char* echo = request.target + sizeof("/echo/") - 1;
        printf("echo: %s\n", echo);
        asprintf(&response, "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: %lu\r\n\r\n%s", strlen(echo), echo);
        printf("response: %s\n", response);
    }
    else {
        response = "HTTP/1.1 404 Not Found\r\n\r\n";
    }
    int len = strlen(response);
    int bytes_sent = send(fd, response, len, 0);

END:
    close(server_fd);

    return 0;
}

