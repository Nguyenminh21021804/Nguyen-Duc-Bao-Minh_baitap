#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

typedef struct {
    char mssv[20];
    char hoTen[50];
    char ngaySinh[20];
    float diemTB;
} SinhVien;

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <Port> <LogFile>\n", argv[0]);
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
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        int client = accept(server, (struct sockaddr *)&client_addr, &client_len);

        SinhVien sv;
        int ret = recv(client, &sv, sizeof(sv), 0);
        if (ret > 0) {
            char *client_ip = inet_ntoa(client_addr.sin_addr);
            time_t t = time(NULL);
            struct tm *tm_info = localtime(&t);
            char time_str[26];
            strftime(time_str, 26, "%Y-%m-%d %H:%M:%S", tm_info);

            // In ra man hinh
            printf("%s %s %s %s %s %.2f\n", client_ip, time_str, sv.mssv, sv.hoTen, sv.ngaySinh, sv.diemTB);

            // Ghi vao file log
            FILE *f = fopen(argv[2], "a");
            if (f) {
                fprintf(f, "%s %s %s %s %s %.2f\n", client_ip, time_str, sv.mssv, sv.hoTen, sv.ngaySinh, sv.diemTB);
                fclose(f);
            }
        }
        close(client);
    }
    return 0;
}