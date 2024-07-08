/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jimchoi <jimchoi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 14:52:03 by jimchoi           #+#    #+#             */
/*   Updated: 2024/07/08 16:25:14 by jimchoi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long long	philo_atoi(const char *str)
{
	long long	neg;
	long long	num;

	num = 0;
	neg = 1;
	while (*str == '\n' || *str == '\t' || *str == '\v'
		|| *str == '\f' || *str == '\r' || *str == ' ')
		str++;
	while ('0' <= *str && *str <= '9')
	{
		num = num * 10 + *str - '0';
		str++;
	}
	if (num > 2147483647)
		return (-1);
	return (num * neg);
}

double	get_time(void)
{
	double			now;
	struct timeval	time;

	gettimeofday(&time, NULL);
	now = (time.tv_sec * 1000 + time.tv_usec / 1000);
	return (now);
}

void	ph_usleep(int ms, t_ph *philo)
{
	double	start;
	int		is_alive;

	is_alive = 1;
	start = get_time();
	while (is_alive && (get_time() - start) < ms)
	{
		pthread_mutex_lock(philo->dead_mutex);
		is_alive = *philo->alive;
		pthread_mutex_unlock(philo->dead_mutex);
		usleep(200);
	}
}

void	ph_write(t_ph *philo, t_flag flag)
{
	int	is_alive;

	is_alive = 0;
	pthread_mutex_lock(philo->dead_mutex);
	is_alive = *philo->alive;
	pthread_mutex_unlock(philo->dead_mutex);
	if (!is_alive && flag != DEAD)
	{
		return ;
	}
	pthread_mutex_lock(philo->write_mutex);
	if (flag == FORK)
		printf("%.0f %d has taken a fork\n", \
		get_time() - philo->time, philo->id + 1);
	else if (flag == EAT)
		printf("%.0f %d is eating\n", get_time() - philo->time, philo->id + 1);
	else if (flag == SLEEP)
		printf("%.0f %d is sleeping\n", get_time() - philo->time, philo->id + 1);
	else if (flag == THINK)
		printf("%.0f %d is thinking\n", get_time() - philo->time, philo->id + 1);
	else if (flag == DEAD)
		printf("%.0f %d died\n", get_time() - philo->time, philo->id + 1);
	pthread_mutex_unlock(philo->write_mutex);
}

void	free_thread(t_data *data)
{
	int	i;

	i = 0;
	pthread_mutex_destroy(&data->start_mutex);
	pthread_mutex_destroy(&data->dead_mutex);
	pthread_mutex_destroy(&data->write_mutex);
	while (i < data->num)
	{
		pthread_mutex_destroy(&data->eat_mutex[i]);
		pthread_mutex_destroy(&data->fork_mutex[i]);
		i++;
	}
	free(data->philos);
	free(data->forks);
	free(data->fork_mutex);
	free(data->eat_mutex);
	free(data);
}
