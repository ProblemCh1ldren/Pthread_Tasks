//Ввести два числа - левую и правую границу.
//Задача - Посчитать все делители каждого числа и найти сумму всех делителей в данном интервале.
//Вариант задачи с потоками, которые динамически выделаются с учетом количества ваших ядер.
//Интервалы приходят из другой программы filecreate


#include<stdio.h>
#include<pthread.h>
#include<math.h>
#include <unistd.h>

#define filename "intervals.txt"

//Сумма делителей
long int numDivisors = 0;
pthread_mutex_t mutex;

//Структура для потоковой функции
struct interval_t
{
  long int start;
  long int end;
};

//Потоковая функция
void* pthread_job(void* interval)
{
  struct interval_t* temp = (struct interval_t*) interval;
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
//Основная программа.
int main(int argc, char* argv[])
{
  FILE* fp;
  fp = fopen(filename , "r");
  pthread_t *t;
  struct interval_t* ThreadsStructs;
  int i = 0;
  //Количество ядер компьютера, по совместительству количество потоков.
  long int cores = sysconf(_SC_NPROCESSORS_CONF);
  pthread_mutex_init(&mutex, NULL);
  //Выделение памяти для количества потоков, ориентируется на количество ядер.
  t = (pthread_t *) malloc(cores * sizeof(pthread_t));
  ThreadsStructs = (struct interval_t *) malloc(cores * sizeof(struct interval_t));
  while (!feof(fp))
    {
      fscanf(fp, "%ld %ld", &ThreadsStructs[i].start, &ThreadsStructs[i].end);
      pthread_create(&t[i], NULL, pthread_job, &ThreadsStructs[i]);
      i++;
    }
  for(int j = 0; j <= i ; j++)
    {
      pthread_join(t[j], NULL);
    }
  pthread_mutex_destroy(&mutex); 
  printf("Количесто делителей чисел из интервала: %ld\n",numDivisors);
  printf("Количество ядер компьютера: %ld\n",cores);
  return 0;
  }
