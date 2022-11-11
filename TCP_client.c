//Клиент, который посылает интервал для решения задач в программу TCP_server.
//Задача - Посчитать все делители каждого числа и найти сумму всех делителей в данном интервале.
//Вариант задачи с потоками, которые динамически выделаются с учетом количества ваших ядер.


#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct interval
{
    long int start;
    long int end;
} interval_t;

int main(int argc, char* argv[])
{
    if (argc != 4)
    {
        printf("Неверное количество аргументов\nВведите IP-адрес и две границы интервала\n");
        return -1;
    }
    //структура для размещения адреса сервера
    struct sockaddr_in servaddr;
    int sockfd;
    int n;
    //создаем TCP-сокет
    if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror(NULL);
        exit(1);
    }
    //перед заполнением обнуляем всю структуру
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET; //семейство
    servaddr.sin_port = htons(51000); //хост

    //преобразует адрес в нужную форму и сохраняет в поле структуры
    if (inet_aton(argv[1], &servaddr.sin_addr) == 0)
    {
        printf("Неверный IP-адрес\n");
        close(sockfd);
        exit(1);
    }
    //устанавливаем соединение через созданный сокет с сокетом сервера, адрес которого в структуре
    if (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0)
    {
        perror(NULL);
        close(sockfd);
        exit(1);
    }

    long int cores = sysconf(_SC_NPROCESSORS_CONF);

    interval_t* PthreadStructs;
    PthreadStructs = (interval_t*)malloc(cores * sizeof(interval_t));

    // write(sockfd, &count_of_cores, sizeof(count_of_cores));
    //если левая граница = 1
    for (int i = 0; i < cores; i++)
    {
        PthreadStructs[i].start = atoi(argv[2]) + i;
        PthreadStructs[i].start = atoi(argv[3]);
        if ((n = write(sockfd, &PthreadStructs[i], sizeof(PthreadStructs[i]))) < 0)
        {
            perror(NULL);
            close(sockfd);
            exit(1);
        };
    }

    close(sockfd);
    free(PthreadStructs);
    return 0;
}
