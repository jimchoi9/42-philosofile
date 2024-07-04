/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_main.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jimchoi <jimchoi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 17:16:39 by jimchoi           #+#    #+#             */
/*   Updated: 2024/07/04 18:18:28 by jimchoi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "p_.h"

t_ph *init_philos(t_data *data)
{
	t_ph    *philos;
	int i = 0;
	philos = malloc(sizeof(t_ph) * data->num);
	philos = memset(philos, 0, sizeof(t_ph ) * data->num);
	while (i < data->num )
	{
		philos[i].id = i;
		philos[i].must_eat = data->must_eat;
		philos[i].l_fork = &data->forks[i];
		philos[i].r_fork = &data->forks[(i + 1) % data->num ];
		philos[i].l_mutex = &data->fork_mutex[i];
		philos[i].r_mutex = &data->fork_mutex[(i + 1) % data->num];
		philos[i].eat_mutex = &data->eat_mutex[i];
		philos[i].last_eat_time = 0;
		philos[i].alive = &data->alive;
		philos[i].start_time = data->start_time;
		philos[i].time_to_die = data->time_to_die;
		philos[i].time_to_eat = data->time_to_eat;
		philos[i].time_to_sleep = data->time_to_sleep;
		philos[i].start_mutex = &data->start_mutex;
		philos[i].dead_mutex = &data->dead_mutex;
		philos[i].write_mutex = &data->write_mutex;
		philos[i].start_time = data->start_time;
		i++;
	}
	return (philos);
}

int init_data(int argc, char *argv[], t_data *data)
{

	data->num = philo_atoi(argv[1]);
	data->time_to_die = philo_atoi(argv[2]);
	data->time_to_eat = philo_atoi(argv[3]);
	data->time_to_sleep = philo_atoi(argv[4]);
	if (argc == 6)
		data->must_eat = philo_atoi(argv[5]);
	else
		data->must_eat = -1;
	data->alive = 1;
	data->start_time = get_current_time();
	pthread_mutex_init(&data->start_mutex, NULL);
	pthread_mutex_init(&data->dead_mutex, NULL);
	pthread_mutex_init(&data->write_mutex, NULL);

	data->forks = malloc(sizeof(int) * data->num);
	data->fork_mutex = malloc(sizeof(pthread_mutex_t) * data->num);
	data->eat_mutex = malloc(sizeof(pthread_mutex_t) * data->num);
	for (int i = 0; i < data->num; i++)
	{
		data->forks[i] = -1;
		pthread_mutex_init(&data->fork_mutex[i], NULL);
		pthread_mutex_init(&data->eat_mutex[i], NULL);
	}

	data->philos = init_philos(data);
	return (0);
}

void *philo(void *data)
{
	t_ph *philo = data;

    pthread_mutex_lock(philo->start_mutex);
    pthread_mutex_unlock(philo->start_mutex);

	if (philo->id % 2 == 0)
		usleep(philo->time_to_eat * 500);
    while (1)
    {
		pthread_mutex_lock(philo->dead_mutex);
		if (*philo->alive == 0)
		{
			pthread_mutex_unlock(philo->dead_mutex);
            break;
		}
		pthread_mutex_unlock(philo->dead_mutex);
		if (ph_take(philo))
			continue ;
        ph_eat(philo);
		pthread_mutex_lock(philo->dead_mutex);
		if (*philo->alive == 0)
		{
			pthread_mutex_unlock(philo->dead_mutex);
            break;
		}
		pthread_mutex_unlock(philo->dead_mutex);
        ph_sleep(philo);
        ph_think(philo);
		usleep(300);
    }
    return (NULL);
}

void *monitoring(void *arg)
{
	t_data *data = (t_data *)arg;

	pthread_mutex_lock(&data->start_mutex);
	pthread_mutex_unlock(&data->start_mutex);
	while (1)
	{
		if(check_dead_philo(data))
			break ;
		usleep(300);
	}
			printf("monitoring done\n");

	return (NULL);
}
void make_thread(t_data *data)
{
	pthread_t monitor;

	pthread_mutex_lock(&data->start_mutex);
	if (pthread_create(&monitor, NULL, &monitoring, data))
	{
        printf("Error: unable to create thread\n");
        return;
    }
	for (int i = 0; i < data->num; i++)
	{
		if(pthread_create(&data->philos[i].thread, NULL, &philo, &data->philos[i]))
		{
            printf("Error: unable to create thread\n");
            return;
        }
	}
	pthread_mutex_unlock(&data->start_mutex);
		if(pthread_join(monitor, NULL))
		{
			printf("Error: unable to join thread\n");
			return;
		}
	printf("Success monitoring\n");
	printf("alive: %d\n", data->alive);
	for (int i = 0; i < data->num; i++)
	{

		if (pthread_join(data->philos[i].thread, NULL))
		{
            printf("Error: unable to join thread\n");
            return;
        }
	}
	
}
void check_leaks(void)
{
 system ("leaks philo");
}

int main(int argc, char *argv[])
{
	t_data *data;
    // atexit(check_leaks);

	data = malloc(sizeof(t_data));
	data = memset(data, 0, sizeof(t_data));
	if (argc != 5 && argc != 6)
	{
		printf("Error: wrong number of arguments\n");
		return (1);
	}
	if (init_data(argc, argv, data))
	{
		printf("Error: invalid arguments\n");
		return (1);
	}
	make_thread(data);
	free_thread(data);
	return (0);
}