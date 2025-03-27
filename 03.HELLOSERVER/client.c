#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define SOCKET int
#define ISVALIDSOCKET(s) (s >= 0)

void die(char *);

void die(char *message)
{
    fprintf(stderr, "ERROR; %s\n", message);
    exit(1);
}

int main()
{
    SOCKET client = socket(AF_INET, SOCK_STREAM, 0);
    if (!ISVALIDSOCKET(client))
    {
        die("SOCKET ERROR IN CLIENT");
    }
    struct sockaddr_in host_address = {};
    host_address.sin_family = AF_INET;
    host_address.sin_port = htons(1234);
    host_address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    int con = connect(socket, (const struct sockaddr *)&host_address, sizeof(host_address));
    if (con)
    {
        die("Error in connection.. ");
    }
    char msg[] = "hello";
    write(client, msg, strlen(msg));

    char buffer[1024];
    ssize_t bytes_read = read(client, buffer, sizeof(buffer) - 1);
    if (bytes_read < 0)
    {
        die("read");
    }
    buffer[bytes_read] = '\0';
    printf("Server says:: %s \n", buffer);
    close(client);
}
