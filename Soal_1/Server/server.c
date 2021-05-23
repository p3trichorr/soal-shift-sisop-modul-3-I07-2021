#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

#define PORT 8080
bool isLogin = false;
char strtemp[1001];

void login(char command[], /*char command2[],*/ char passid[], int new_socket);
void addFile(char passid[], int new_socket);

int main(int argc, char const *argv[]) {
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char *hello = "Hello from server";
      
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
      
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) {
    	perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    char command[1001], command2[1001], passid[1001];
    memset(passid, 0, sizeof(passid));
    strcpy(strtemp, "\nType in the available command:\n1) Register\n2) Login\n3) Quit\nCommand: ");
    send(new_socket, strtemp, sizeof(strtemp), 0);
        
    read(new_socket, command, sizeof(command));
        
    if(strcmp(command, "Login") == 0 || strcmp(command, "Register") == 0)
        login(command, /*command2,*/ passid, new_socket);

    if(strcmp(command, "Add") == 0)
        addFile(passid, new_socket);

    else if(strcmp(command, "Quit"))
        exit(EXIT_SUCCESS);

    else {
        strcpy(strtemp, "Invalid Input\n");
        send(new_socket, strtemp, sizeof(strtemp), 0);
    }
    return 0;
}

void login(char command[], /*char command2[],*/ char passid[], int new_socket) {
    FILE *akun = fopen("akun.txt", "a+"); 
    char str[1001];    
    bool check = true;

    strcpy(strtemp, "Input id and password in one line :\n");
    send(new_socket, strtemp, sizeof(strtemp), 0);

    int idread = read(new_socket, passid, 1024);
    if(strcmp(command, "Register") == 0) {
        // To check on the system
        while(fgets(str, 100, akun)) { 
            if(strcmp(str, passid) == 0) {
                strcpy(strtemp, "The inputted id and password is already listed in the system\n");
                send(new_socket, strtemp, sizeof(strtemp), 0);
                check = false;
                fclose(akun);
            }
        }
        if(check) {
            fprintf(akun, "%s", passid);
            strcpy(strtemp, "The account is successfully created in the system\n");
            send(new_socket, strtemp, sizeof(strtemp), 0);
        }
    }
    else if(strcmp(command, "Login") == 0) {
        while(fgets(str, 100, akun) != NULL ) {
            if(strcmp(str, passid) == 0) {
                strcpy(strtemp, "\nLog in successful\n1. Add File\n2. Quit\nCommand: ");
                send(new_socket, strtemp, sizeof(strtemp), 0);
                isLogin = true;
				fclose(akun);

				// read(new_socket, command2, sizeof(command2));
				// if(strcmp(command2, "Add") == 0){
				// 	addFile(passid, new_socket);
				// }
				// else if(strcmp(command2, "Quit"))
				// 	exit(EXIT_SUCCESS);
            }
        }
        if(!isLogin) {
            strcpy(strtemp, "Id and password is not listed in the system\n");
            send(new_socket, strtemp, sizeof(strtemp), 0);
        }
    }  
}

void addFile(char passid[], int new_socket) {
    char str[1001];
    FILE *FILES = fopen("file.tsv", "a");
    FILE *log = fopen("running.log", "a");
    
    strcpy(strtemp, "Publisher: ");
    send(new_socket, strtemp, sizeof(strtemp), 0);
    read(new_socket, str, sizeof(str));
    fprintf(FILES, "Publisher: %s\n", str);

    strcpy(strtemp, "Publication year: ");
    send(new_socket, strtemp, sizeof(strtemp), 0);
    read(new_socket, str, sizeof(str));
    fprintf(FILES, "Publication year: %s\n", str);

    strcpy(strtemp, "Filepath: ");
    send(new_socket, strtemp, sizeof(strtemp), 0);
    read(new_socket, str, sizeof(str));
    fprintf(FILES, "Filepath: %s\n\n", str);

    fclose(FILES);
}
