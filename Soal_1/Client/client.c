#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#define PORT 8080
char buffer[1024] = {0};
int sock;

void reglog(char command[], /*char command2[],*/ char id[], char pass[], char passid[]);
void addFile(char command[], char passid[], int sock);

int main(int argc, char const *argv[]) {
    struct sockaddr_in address;
    struct sockaddr_in serv_addr;
    char *hello = "Hello from client";
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }
  
    memset(&serv_addr, '0', sizeof(serv_addr));
  
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
      
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
  
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

    char command[1001], command2[1001], passid[1001];
    memset(passid, 0, sizeof(passid));
    char id[1001], pass[1001];
    
        read(sock, buffer, sizeof(buffer)); // Type one of the following command
        printf("%s", buffer);

        scanf("%s", command);
        send(sock, command, sizeof(command), 0);

        if(strcmp(command, "Login") == 0 || strcmp(command, "Register") == 0)
            reglog(command, /*command2,*/ id, pass, passid);

        else if(strcmp(command, "Add") == 0)
            addFile(command, passid, sock);

        else if(strcmp(command, "Quit") == 0)
            exit(EXIT_SUCCESS);

        else {
            read(sock, buffer, sizeof(buffer));
            printf("%s", buffer);
        }
    
    return 0;
}

void reglog(char command[], /*char command2[],*/ char id[], char pass[], char passid[]) {
    char strtemp[1001];
    read(sock, strtemp, sizeof(strtemp)); // Input id and pass in one line
    printf("%s", strtemp);

    scanf("%s %s", id, pass);
    strcpy(passid, id);
    strcat(passid, ":");
    strcat(passid, pass);
    strcat(passid, "\n");
    
    int len = strlen(passid);
    send(sock, passid, len, 0);

    read(sock, buffer, sizeof(buffer));
    printf("%s", buffer);

	//scanf("%s",command2);
	//send(sock, command2, sizeof(command2), 0);
}

void addFile(char command[], char id[], int sock) {
    char publisher[1001], year[1001], path[1001];
    
    read(sock, buffer, sizeof(buffer));
    printf("%s", buffer);
    scanf("%s", publisher);
    send(sock, publisher, sizeof(publisher), 0);

    read(sock, buffer, sizeof(buffer));
    printf("%s", buffer);
    scanf("%s", year);
    send(sock, year, sizeof(year), 0);

    read(sock, buffer, sizeof(buffer));
    printf("%s", buffer);
    scanf("%s", path);
    send(sock, path, sizeof(path), 0);
}
