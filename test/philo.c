#include "philo.h"



void	philo_eating(t_ph *philo)
{	
	pthread_mutex_lock(&philo->data->fork_mutex[philo->left]);
	if (philo->data->fork[philo->left] == -1)
		{
			pthread_mutex_unlock(&philo->data->fork_mutex[philo->left]);
			pthread_mutex_lock(&philo->data->fork_mutex[philo->left]);
			philo->data->fork[philo->left] = philo->id;
		}
	else
		pthread_mutex_unlock(&philo->data->fork_mutex[philo->left]);
	pthread_mutex_lock(&philo->data->fork_mutex[philo->right]);
		if (philo->data->fork[philo->right] == -1)
		{
			pthread_mutex_unlock(&philo->data->fork_mutex[philo->right]);
			pthread_mutex_lock(&philo->data->fork_mutex[philo->right]);
			philo->data->fork[philo->right] =  philo->id;
		}
	else
		pthread_mutex_unlock(&philo->data->fork_mutex[philo->right]);



		// DKJHFS
		if (philo->data->fork[philo->left] ==  philo->id && philo->data->fork[philo->right] ==  philo->id)
		{
			pthread_mutex_lock(&philo->data->write);
			printf("%.0f %d is eating\n", get_time_stamp(philo->data), philo->id);
			pthread_mutex_unlock(&philo->data->write);
			philo->eat_count++;
			philo->data->fork[philo->left] = -1;
			philo->data->fork[philo->right] = -1;
			philo->last_eat = get_time_stamp(philo->data);
			usleep(philo->data->eat_time * 1000);
			pthread_mutex_unlock(&philo->data->fork_mutex[philo->left]);
			pthread_mutex_unlock(&philo->data->fork_mutex[philo->right]);

		}

}

int is_alive(t_ph *philo)
{
    pthread_mutex_lock(&philo->data->dead_mutex);
        if (philo->data->is_dead)
        {
            pthread_mutex_unlock(&philo->data->dead_mutex);
            return (0);
        }
    pthread_mutex_unlock(&philo->data->dead_mutex);
        return (1);
}

void* philosopher(void* arg) {
    t_ph *philo;

    philo = *(t_ph**)arg;
	philo->last_eat = get_time_stamp(philo->data);
    pthread_mutex_lock(&philo->data->start);
    pthread_mutex_unlock(&philo->data->start);
	if (philo->id % 2 == 0) 
		usleep(1000);


	while(1)
	{
        if (!is_alive(philo))
            break ;
		else if (get_time_stamp(philo->data) - philo->last_eat > philo->data->life_time)
		{
			printf("%.0f %d died\n", get_time_stamp(philo->data), philo->id);
            pthread_mutex_lock(&philo->data->dead_mutex);
            philo->data->is_dead = 1;
            pthread_mutex_unlock(&philo->data->dead_mutex);
			break ;
		}
		if (philo->eat_count == philo->data->must_eat && philo->eat_count != 0)
			break  ;
        if (!is_alive(philo))
            break ;
		else
            philo_eating(philo);
        if (!is_alive(philo))
            break ;
        else
        {
			pthread_mutex_lock(&philo->data->write);
		printf("%.0f %d is sleeping\n", get_time_stamp(philo->data), philo->id);
			pthread_mutex_unlock(&philo->data->write);
            usleep(philo->data->sleep_time * 1000);
        }
        if (!is_alive(philo))
            break ;
        else
        {
			pthread_mutex_lock(&philo->data->write);

            printf("%.0f %d is thinking\n", get_time_stamp(philo->data), philo->id);
			pthread_mutex_unlock(&philo->data->write);
        }
	}

    return (arg);
}



void print_parsing(t_data *data)
{
    printf("num = %d\n", data->num);
    printf("life_time = %d\n", data->life_time);
    printf("eat_time = %d\n", data->eat_time);
    printf("sleep_time = %d\n", data->sleep_time);
    printf("must_eat = %d\n", data->must_eat);
    printf("must_eat = %f\n", data->start_time);

    for (int i = 0; i < data->num; i++)
    {
        printf("%d left = %d , right = %d\n", data->philo[i]->id, data->philo[i]->right, data->philo[i]->left);
        // printf("thread %ld eat_count = %d , eat_time = %d\n", data->philo[i]->thread, data->philo[i]->eat_count, data->philo[i]->eat_time);
    }
}


void *check_dead(void *arg)
{
    t_data *data = *(t_data **)arg;
    printf("############check_dead############\n isdead = %d\n", data->is_dead);

    // pthread_mutex_lock(&data->check_dead);
    // printf("############check_dead############\n");
    while(1)
    {
        pthread_mutex_lock(&data->dead_mutex);
        if (data->is_dead)
        {
            data->is_dead = 2;
            pthread_mutex_unlock(&data->dead_mutex);
            break ;

        }
        pthread_mutex_unlock(&data->dead_mutex);
        // usleep(100);
    }
    // pthread_mutex_unlock(&data->check_dead);
    printf("############check_dead_end############\n");

        return ((void *)1);
}


int main(int argc, char *argv[])
{
    t_ph *philo;
    t_data *data;

    data = calloc(1, sizeof(t_data));
    memset(data, 0, sizeof(t_data));
    if (argc != 5 && argc != 6)
        return (1);
    if (init_data(argv, argc, data))
        return (1);
// print_parsing(data);
    pthread_t check_thread;

    printf("############main############\n isdead = %d\n", data->is_dead);
    pthread_create(&check_thread, NULL, check_dead, &data);  // 이중 포인터 전달


    pthread_mutex_lock(&data->start);  // 시작 뮤텍스 획득

    for(int i = 0; i < data->num; i++) // 스레드 생생성
    {
        pthread_create(&data->philo[i]->thread, NULL, philosopher, &data->philo[i]);  // 이중 포인터 전달
    }
    pthread_mutex_unlock(&data->start);  // 시작 뮤텍스 해제



    // for(int i = 1; i <= data->num; i++)
    // usleep(10000);
    // pthread_mutex_lock(&data->check_dead);
    int status = 0;
    // pthread_detach(check_thread);
    pthread_join(check_thread, (void *)&status);
    printf("num = %d\n", status);
    for(int i = 0; i < data->num; i++)
    {
        // printf("1\n");
        // pthread_mutex_lock(&data->dead_mutex);
        // pthread_join(data->philo[i]->thread, NULL);
        pthread_detach(data->philo[i]->thread);
        // pthread_mutex_unlock(&data->dead_mutex);
    }



    // 메모리 해제

    printf("2\n");


    for (int i = 0; i < data->num; i++) {
        free(data->philo[i]);
    }
    for (int i = 0; i < data->num; i++) {
        pthread_mutex_destroy(&data->fork_mutex[i]);
    }

    free(data->philo);
    free(data->fork);
    free(data->fork_mutex);
    free(data);
    // printf("############main_end############\n isdead = %d\n", data->is_dead);

    // pthread_mutex_unlock(&data->check_dead);
    return 0;
}
