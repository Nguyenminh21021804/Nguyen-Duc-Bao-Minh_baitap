#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

typedef struct {
    char mssv[20];
    char hoTen[50];
    char ngaySinh[20];
    float diemTB;
} SinhVien;

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <IP Address> <Port>\n", argv[0]);
        return 1;
    }

    int client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(argv[1]);
    addr.sin_port = htons(atoi(argv[2]));

    if (connect(client, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("Connect failed");
        return 1;
    }

    SinhVien sv;
    printf("MSSV: "); fgets(sv.mssv, sizeof(sv.mssv), stdin); sv.mssv[strcspn(sv.mssv, "\n")] = 0;
    printf("Ho ten: "); fgets(sv.hoTen, sizeof(sv.hoTen), stdin); sv.hoTen[strcspn(sv.hoTen, "\n")] = 0;
    printf("Ngay sinh: "); fgets(sv.ngaySinh, sizeof(sv.ngaySinh), stdin); sv.ngaySinh[strcspn(sv.ngaySinh, "\n")] = 0;
    printf("Diem TB: "); scanf("%f", &sv.diemTB);

    send(client, &sv, sizeof(sv), 0);
    
    close(client);
    return 0;
}