
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>
#include "./libft_src/libft.h"
typedef struct s_data
{
    int num;
    int life_time;
    int eat_time;
    int eat_count;
    int sleep_time;
    int must_eat;
    double start_time;
    int *fork;
}t_data;


void init_data(t_data *data)
{
	data->fork = calloc(data->num+ 1, sizeof(int));
	 for (int i = 0; i < data->num; i++) {
		data->fork[i] = i;
		// data->fork_mutex = calloc(data->num, sizeof(pthread_mutex_t));
    }
}

int main()
{
	 t_data *data;

    data = calloc(1, sizeof(t_data));
	 data->num = 5;
	 init_data(data);
	 for(int i = 0; i < data->num; i++) {
		printf("%d\n", data->fork[i]);
	 }
}