//Ввести два числа - левую и правую границу.
//Посчитать все делители каждого числа и найти сумму всех делителей в данном интервале.
//Вариант задачи без потоков.


#include <stdio.h>
#include <pthread.h>
#include <math.h>
#include <unistd.h>

long int numDivisors = 0;

int main(int argc, char* argv[]) {
  if (argc != 3)
  {
    printf("Введите левую и правую границу!");
  }
  for (long int i = atoi(argv[1]); i <= atoi(argv[2]); i++)
    {
      for (long int j = 1; j <= sqrt(i); j++)
        {
          if (!(i % j))
            {
              if (j != i/j)
                {
                  numDivisors += 2;
                }
                else
                {
                  numDivisors++;
                }
            }
        }
    }
  printf("%ld", numDivisors);
  return 0;
}
