/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jimchoi <jimchoi@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 13:13:57 by jimchoi           #+#    #+#             */
/*   Updated: 2024/06/29 03:51:45 by jimchoi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long long	philo_atoi(const char *str)
{
	long long	neg;
	long long	num;
	int			cnt;

	cnt = 0;
	num = 0;
	neg = 1;
	while (*str == '\n' || *str == '\t' || *str == '\v'
		|| *str == '\f' || *str == '\r' || *str == ' ')
		str++;
	while (*str == '-' || *str == '+')
	{
		if (*str == '-')
			neg = -1;
		cnt++;
		str++;
	}
	while ('0' <= *str && *str <= '9')
	{
		num = num * 10 + *str - '0';
		str++;
	}
	if (cnt > 1)
		num = 0;
	return (num * neg);
}

double	get_time_stamp(t_data *data)
{
	double now;

	gettimeofday(&(data->time), NULL);
	 now = (data->time.tv_sec * 1000 +  data->time.tv_usec / 1000);
	return (now - data->start_time);
}


int init_data(char **argv , int argc, t_data *data)
{
    data->num = philo_atoi(argv[1]);
    data->life_time = philo_atoi(argv[2]);
    data->eat_time = philo_atoi(argv[3]);
    data->sleep_time = philo_atoi(argv[4]);
    data->must_eat = 0; // check
    if (argc == 6)
        data->must_eat = philo_atoi(argv[5]);
    data->eat_count = 0;
	gettimeofday(&(data->time), NULL);
	data->start_time = data->time.tv_sec * 1000 + data->time.tv_usec / 1000;
    data->philo  = malloc(data->num * sizeof(t_ph *));
    memset(data->philo, 0, sizeof(t_ph *) * (data->num ));
	data->fork = malloc(data->num * sizeof(int));
    memset(data->fork, 1, sizeof(int) * (data->num ));
    data->is_dead = 0;
	
    
    pthread_mutex_init(&data->write, NULL);
    pthread_mutex_init(&data->start, NULL);
    pthread_mutex_init(&data->dead_mutex, NULL);
    // pthread_mutex_init(&data->check_dead, NULL);
    
    int i = 1;


	data->fork_mutex = malloc(data->num  * sizeof(pthread_mutex_t));
    memset(data->fork_mutex, 0, sizeof(pthread_mutex_t) * (data->num));


    for (i = 0; i < data->num; i++) {
		data->fork[i] = -1;
		// data->fork_mutex = calloc(data->num, sizeof(pthread_mutex_t));
        pthread_mutex_init(&data->fork_mutex[i], NULL);
    }

    for (i = 0; i < data->num; i++)
    {
        data->philo[i] = malloc( sizeof(t_ph));
        memset(data->philo[i], 0, sizeof(t_ph));
        data->philo[i]->id = i;
        data->philo[i]->right = i ;
        data->philo[i]->left = (i + 1) % data->num;
        data->philo[i]->data = data; // philo 구조체에 data 구조체 참조 설정
        data->philo[i]->eat_count = 0; // philo 구조체에 data 구조체 참조 설정
        data->philo[i]->dead = 0;
		data->philo[i]->last_eat = 0;
    }
    return 0;
}