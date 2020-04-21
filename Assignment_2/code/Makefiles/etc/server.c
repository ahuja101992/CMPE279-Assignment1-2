// Server side C/C++ program to demonstrate Socket programming 
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <sys/types.h>
#include <pwd.h>
#include <sys/wait.h>
#define PORT 86
int main(int argc, char const *argv[]) 
{ 
    int server_fd, new_socket, valread; 
    struct sockaddr_in address; 
    int opt = 1; 
    int addrlen = sizeof(address); 
    char buffer[1024] = {0}; 
    char *hello = "Hello from server"; 
    char args[4];
printf("argv[0][2] %d\n", argv[0][2]);	 
    if(argv[0][2]==100)
    {
	printf("here\n");
        valread = read( argv[0][1] , buffer, 1024); 
        printf("%s\n",buffer ); 
        send(argv[0][1] , hello , strlen(hello) , 0 ); 
        printf("Hello message sent\n");
	    return 0;  
    }       
    // Creating socket file descriptor 
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    { 
        perror("socket failed"); 
        exit(EXIT_FAILURE); 
    } 
       
    // Forcefully attaching socket to the port 80 
     if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
                                                   &opt, sizeof(opt))) /// removing coz of old linux kernal 
    { 
        perror("setsockopt"); 
        exit(EXIT_FAILURE); 
    } 
    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = INADDR_ANY; 
    address.sin_port = htons( PORT ); 
       
    // Forcefully attaching socket to the port 8080 
    if (bind(server_fd, (struct sockaddr *)&address,  
                                 sizeof(address))<0) 
    { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    } 
    if (listen(server_fd, 3) < 0) 
    { 
        perror("listen"); 
        exit(EXIT_FAILURE); 
    } 
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,  
                       (socklen_t*)&addrlen))<0) 
    { 
        perror("accept"); 
        exit(EXIT_FAILURE); 
    } 
    int newProcess= fork();
    if(newProcess==0)
    { 
        setuid(65534);
        printf("here.... \n");
        printf("updated UID is   : %d", getuid());
 	    args[0] = new_socket;
        args[1] = new_socket;
        args[2] = 100;
	args[3] = NULL;// last null to end the execl array args
	execl(argv[0], args, NULL);
    }else if(newProcess!=-1){
        printf("Parent is waiting...\n");
        wait(NULL);
    }else{
        perror("Error while calling the fork function");
    }
    return 0; 
} 
