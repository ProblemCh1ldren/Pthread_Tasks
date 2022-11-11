/* Программа считает сумму делителей всех чисел в интервалах, которые генерирует программа interval_generator
Ипользуются несколько потоков */

#include <pthread.h>
#include <math.h>
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

long long int numDivisors = 0; // общие данные для потоков  - переменная для подсчета суммы делителей
pthread_mutex_t mutex;

typedef struct interval {
    long int start;
    long int end;
} interval_t;

//Потоковая функция
void* pthread_job(void* interval)
{
  interval_t* temp = (struct interval_t*) interval;
  long int cores = sysconf(_SC_NPROCESSORS_CONF);
  //Цикл нахождения делителей с шагом в количество ядер.
  for (long int i = temp -> start; i <= temp -> end; i+= cores)
    {
      for (long int j = 1; j <= sqrt(i); j++)
       {
         if (!(i % j))
          {
            pthread_mutex_lock(&mutex);
            if (j != i/j)
              {
                numDivisors += 2;
              }
            else
              {
                numDivisors++;
              }
            pthread_mutex_unlock(&mutex);
          }
      }
    }
  return NULL;
 }

int main(int argc, char* argv[])
{
    //дескрипторы для пассивного(слушающего) и присоединенного сокетов
    int sockfd, newsockfd;
    int clilen, n;
    long int cores = sysconf(_SC_NPROCESSORS_CONF);
    //структуры для размещения полных адресов сервера и клиента
    struct sockaddr_in servaddr, cliaddr;
    //создаем TCP-сокет
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror(NULL);
        close(sockfd);
        exit(1);
    }
    //обнуляем всю структуру перед заполнением
    memset(&servaddr, 0, sizeof(servaddr));
    //заполняем структуру для адреса сервера
    servaddr.sin_family = AF_INET; //семейство
    servaddr.sin_port = htons(51000); //номер порта
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    //связываем созданный сокет с адресом (3 параметр - размер структуры в байтах)
    if (bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
        perror(NULL);
        close(sockfd);
        exit(1);
    }
    //переводим сокет в пассивное состояние
    // второй параметр - кол-во одновременных подключений
    if (listen(sockfd, 5) < 0) {
        perror(NULL);
        close(sockfd);
        exit(1);
    }
    // заносим в clilen максимальную длину ожидаемого адреса клиента
    clilen = sizeof(cliaddr);
    // принимаем соединение на сокете
    // при нормальном завершении в структуре будет адрес, а в clilen - фактическая длина
    if ((newsockfd = accept(sockfd, (struct sockaddr*)&cliaddr, &clilen)) < 0) {
        perror(NULL);
        close(sockfd);
        exit(1);
    }

    //выделение памяти для интервалов
    interval_t* intervals;
    intervals = (interval_t*)malloc(cores * sizeof(interval_t));

    //выделение памяти для  потоков
    pthread_t* p;
    p = (pthread_t*)malloc(cores * sizeof(pthread_t));

    //инициализация мьютекса
    pthread_mutex_init(&mutex, NULL);

    //считываем интервалы из сокета и создаем потоки
    for (int i = 0; i < cores; ++i) {
        n = read(newsockfd, &intervals[i], sizeof(intervals[i]));
        //если при чтении возникла ошибка - завершаем работу
        if (n < 0) {
            perror(NULL);
            close(sockfd);
            close(newsockfd);
            exit(1);
        }
        pthread_create(&p[i], NULL, pthread_job, &intervals[i]);
    }

    for (int i = 0; i < cores; ++i)
        pthread_join(p[i], NULL);

    pthread_mutex_destroy(&mutex);
    printf("Count of divisors = %lld\n", numDivisors);
  
    free(p);
    free(intervals);
    close(sockfd);
    close(newsockfd);
    return 0;
}
