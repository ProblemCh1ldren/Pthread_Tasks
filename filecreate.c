//Создание интервалов для программы "Tasks_With_Pthreads"
//Шаг интервала равен количеству ядер.

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>

#define filename "intervals.txt"

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        printf("Введите левую и правую границу интервала\n");
        return -1;
    }
    FILE* fp;
    fp = fopen(filename, "w+");
    for (int i = 0; i < cores; i++)
    {
        if (i <= atoi(argv[2]) - 1)
        {
            fprintf(fp, "%d %d\n", atoi(argv[1]) + i, atoi(argv[2]));
        }
    }
    //fprintf(fp, "%d = Шаг в интервале и количество ядер.", cores);
    fclose(fp);
}
