//Задача - Посчитать все делители каждого числа и найти сумму всех делителей в данном интервале.
//Интервалы берутся из файла, созданного программой "filecreate".
//Вариант задачи с потоками, которые динамически выделаются с учетом количества ваших ядер.


#include<stdio.h>
#include<pthread.h>
#include<math.h>
#include <unistd.h>

#define filename "intervals.txt"

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
  FILE* fp;
  fp = fopen(filename , "r");
  pthread_t *t;
  interval_t* ThreadsStructs;
  long int cores = sysconf(_SC_NPROCESSORS_CONF);
  int lines = 0;
  while(!feof(fp))
  {
    if (fgetc(fp) == '\n')
    ++lines;
  }
  pthread_mutex_init(&mutex, NULL);
  //Выделение памяти для количества потоков, ориентируется на количество ядер.
  t = (pthread_t *) malloc(lines * sizeof(pthread_t));
  ThreadsStructs = (interval_t *) malloc(lines * sizeof(interval_t));
  rewind(fp);
  for (int i = 0; i < lines; ++i)
    {
      fscanf(fp,"%ld %ld", &ThreadsStructs[i].start, &ThreadsStructs[i].end);
      pthread_create(&t[i], NULL, pthread_job, &ThreadsStructs[i]);
    }
  for(int j = 0; j < lines ; j++)
    {
      pthread_join(t[j], NULL);
    }
  pthread_mutex_destroy(&mutex); 
  printf("Количесто делителей чисел из интервала: %lld\n",numDivisors);
  printf("Количество ядер компьютера: %ld\n",cores);
  free(t);
  free(ThreadsStructs);
  fclose(fp);
  return 0;
}
