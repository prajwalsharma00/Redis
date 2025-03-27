#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SOCKET int
#define ISVALIDSOCKET(s) (s >= 0)

void die(const char *);
void dosomething(SOCKET);
void msg(char *);

void msg(char *message)
{
    fprintf(stderr, "Error: %s\n", message);
    return;
}
void die(const char *message)
{
    fprintf(stderr, "Error: %s\n", message);
    exit(1);
}
void dosomething(SOCKET s)
{
    char recvbuffer[1024] = {};
    int bytes_read = read(s, recvbuffer - 1, sizeof(recvbuffer));
    if (bytes_read < 0)
    {
        msg("read() error \n");
        return;
    }
    else
    {
        recvbuffer[bytes_read] = '\0';
        printf("Recieved from client..  %s \n", recvbuffer);
    }

    char *buffer = "This is string literal that is store in code segment";
    write(s, buffer, sizeof(buffer));
    printf("send message to teh clinet.. \n");
}

int main()
{
    printf("Connecting to the socket.. \n");
    SOCKET server = socket(AF_INET, SOCK_STREAM, 0);
    // AF_INET IS for ipv4 if we need ipv^ addresss for our socket we use
    //  AF_INET6
    // sock-stream is for tcp
    int val = 1;
    setsockopt(server, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));
    // here we use sol_socket to meke sure that we are

    /*
    workign and making our socket chagnes , we cna also use tcp or ip



    */
    printf("Binding SOCKET with ip Address.. \n");
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = ntohs(1234);
    addr.sin_addr.s_addr = ntohl(0);
    int rv = bind(server, (const struct sockaddr *)&addr, sizeof(addr));
    if (rv)
    {
        die("bind()");
    }
    printf("Listening for conncetion,.. \n");
    rv = listen(server, SOMAXCONN);
    if (rv)
    {
        die("listen()");
    }
    while (1)
    {
        struct sockaddr_in client_address;
        memset(&client_address, 0, sizeof(client_address));
        socklen_t client_len = sizeof(client_address);
        SOCKET client = accept(server, &client_address, &client_len);
        if (!ISVALIDSOCKET(client))
        {
            die("client coulnot be accepted.. \n");
        }
        dosomething(client);
        close(client);
    }
}