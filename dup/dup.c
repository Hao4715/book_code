#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

int main(int argc, char* argv[]) {
    if ( argc != 3 ) {
        printf("please input correct argument\n");
        return 0;
    }

    char *ip = argv[1];
    int port = atoi(argv[2]);

    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server_addr,client_addr;
    socklen_t client_len = sizeof(client_addr);
    bzero(&server_addr, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    inet_pton(AF_INET, ip, &server_addr.sin_addr);

    int res = bind(listen_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if(res == -1){
        printf("%d bind error\n",port);
        return 0;
    }
    res = listen(listen_fd,5);
    if ( res == -1 ) {
        printf("listen() error\n");
        return 0;
    }

    int client_fd = accept(listen_fd,(struct sockaddr*)&client_addr,&client_len);
    if( client_fd < 0 ) {
        printf("accept error\n");
        return 0;
    }
    printf("STDOUT_FILENO:%d\n",STDOUT_FILENO);
    close( STDOUT_FILENO );                          //关闭标准输出描述符
    int new_descriptor = dup(client_fd);             //将client_fd复制到new_descriptor
    printf("new_descriptor:%d\n",new_descriptor);    //输出new_descriptor值为 1（在客户端输出）
    if ( new_descriptor == 0 ) {
        printf(" dup() error\n");
        return 0;
    }
    char *say_hello_1 = "hello client_fd\n";
    char *say_hello_2 = "hello new_descriptor\n";
    char *say_hello_3 = "hello printf\n";
    write(client_fd,say_hello_1,strlen(say_hello_1));     //写数据到cilent_fd
    write(new_descriptor,say_hello_2,strlen(say_hello_2));//写数据到new_descriptor
    printf("%s\n",say_hello_3);                           //该输出将被输出到client_fd。
    close(client_fd);
    close(new_descriptor);
    close(listen_fd);
    return 0;
}