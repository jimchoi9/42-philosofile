#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>
#include <memory.h>

typedef struct s_data t_data;

typedef struct s_ph
{
    int id;
    int eat_count;
    int left;
    int right;
	double last_eat;
    pthread_t thread;
   t_data *data;
   int dead;

}t_ph;

typedef struct s_data
{
	// int check;
    int num;
    int life_time;
    int eat_time;
    int sleep_time;
    int eat_count;
    int must_eat;
    double start_time;
    int *fork;
	t_ph **philo;
        int is_dead;
    pthread_mutex_t write;
    // pthread_mutex_t dead;
    pthread_mutex_t start;
    pthread_mutex_t dead_mutex;
    pthread_mutex_t *fork_mutex;
	struct timeval time;
    // pthread_mutex_t check_dead;

}t_data;


long long	philo_atoi(const char *str);
double	get_time_stamp(t_data *data);
int init_data(char **argv , int argc, t_data *data);
