//Ввести два числа - левую и правую границу.
//Задача - Посчитать все делители каждого числа и найти сумму всех делителей в данном интервале.
//Вариант задачи с потоками, которые динамически выделаются с учетом количества ваших ядер.


#include<stdio.h>
#include<pthread.h>
#include<math.h>
#include <unistd.h>

//Сумма делителей
long long int numDivisors = 0;
pthread_mutex_t mutex;

//Структура для потоковой функции
typedef struct
{
    long int start;
    long int end;
    int flag;//Шаг
} interval_t;

//Потоковая функция
void* pthread_job(void* interval)
{
    interval_t* temp = (interval_t*) interval;
    long int cores = sysconf(_SC_NPROCESSORS_CONF);
    //Цикл нахождения делителей с шагом в количество ядер.
    for (long int i = (temp -> start + temp -> flag); i <= temp -> end; i+= cores)
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
//Основная программа.
int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        printf("Введите левую и правую границу интервала\n");
        return -1;
    }
    pthread_t *t;
    interval_t* ThreadsStructs;
    //Количество ядер компьютера, по совместительству количество потоков.
    long int cores = sysconf(_SC_NPROCESSORS_CONF);
    pthread_mutex_init(&mutex, NULL);
    //Выделение памяти для количества потоков, ориентируется на количество ядер.
    t = (pthread_t *) malloc(cores * sizeof(pthread_t));
    ThreadsStructs = (interval_t *) malloc(cores * sizeof(interval_t));
    for (int i=0; i < cores; i++)
    {
        ThreadsStructs[i].start = atoi(argv[1]);//atoi-из строки в число.
        ThreadsStructs[i].end = atoi(argv[2]);
        ThreadsStructs[i].flag = i;
        pthread_create(&t[i],NULL,pthread_job,&ThreadsStructs[i]);
    }
    for(int i=0; i < cores ; i++)
    {
        pthread_join(t[i],NULL);
    }
    pthread_mutex_destroy(&mutex);
    free(t);
    free(ThreadsStructs);
    printf("Количесто делителей чисел из интервала: %lld\n",numDivisors);
    printf("Количество ядер компьютера: %ld\n",cores);
    return 0;
}
