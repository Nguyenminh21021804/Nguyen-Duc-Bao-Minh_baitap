#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Usage: %s <Port> <HelloFile> <LogFile>\n", argv[0]);
        return 1;
    }

    int server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(atoi(argv[1]));

    bind(server, (struct sockaddr *)&addr, sizeof(addr));
    listen(server, 5);

    while (1) {
        int client = accept(server, NULL, NULL);

        // 1. Đọc file và gửi câu chào
        FILE *f_hello = fopen(argv[2], "r");
        if (f_hello) {
            char hello_msg[1024];
            while (fgets(hello_msg, sizeof(hello_msg), f_hello)) {
                send(client, hello_msg, strlen(hello_msg), 0);
            }
            fclose(f_hello);
        }

        // 2. Nhận dữ liệu từ client và ghi vào file log
        FILE *f_log = fopen(argv[3], "a");
        if (f_log) {
            char buffer[1024];
            int ret = recv(client, buffer, sizeof(buffer) - 1, 0);
            if (ret > 0) {
                buffer[ret] = 0;
                fprintf(f_log, "%s\n", buffer);
            }
            fclose(f_log);
        }
        close(client);
    }
    close(server);
    return 0;
}