/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_thread.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jimchoi <jimchoi@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 14:39:23 by jimchoi           #+#    #+#             */
/*   Updated: 2024/07/09 16:54:23 by jimchoi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ph_take(t_ph *philo)
{
	pthread_mutex_lock(philo->r_mutex);
	if (philo->one != 1)
		pthread_mutex_lock(philo->l_mutex);
	if (*philo->r_fork == -1)
	{
		ph_write(philo, FORK);
		*philo->r_fork = philo->id;
	}
	if (*philo->l_fork == -1)
	{
		ph_write(philo, FORK);
		*philo->l_fork = philo->id;
	}
	pthread_mutex_unlock(philo->r_mutex);
	if (philo->one == 1)
		return (1);
	pthread_mutex_unlock(philo->l_mutex);
	return (0);
}

void	ph_eat(t_ph *philo)
{
	pthread_mutex_lock(philo->r_mutex);
	pthread_mutex_lock(philo->l_mutex);
	if (*philo->l_fork == philo->id && *philo->r_fork == philo->id)
	{
		ph_write(philo, EAT);
		pthread_mutex_lock(philo->eat_mutex);
		philo->last_eat_time = get_time();
		pthread_mutex_unlock(philo->eat_mutex);
		ph_usleep(philo->time_to_eat, philo);
		pthread_mutex_lock(philo->eat_mutex);
		if (philo->must_eat > 0)
			philo->must_eat--;
		pthread_mutex_unlock(philo->eat_mutex);
		*philo->l_fork = -1;
		*philo->r_fork = -1;
	}
	pthread_mutex_unlock(philo->r_mutex);
	pthread_mutex_unlock(philo->l_mutex);
}

int	check_dead_philo(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num)
	{
		pthread_mutex_lock(data->philos[i].eat_mutex);
		if (data->philos[i].last_eat_time != 0 && \
		get_time() - data->philos[i].last_eat_time > data->time_to_die)
		{
			pthread_mutex_lock(&data->dead_mutex);
			data->alive = 0;
			pthread_mutex_unlock(&data->dead_mutex);
			ph_write(&data->philos[i], DEAD);
			pthread_mutex_unlock(data->philos[i].eat_mutex);
			return (1);
		}
		pthread_mutex_unlock(data->philos[i].eat_mutex);
		i++;
		usleep(400);
	}
	return (0);
}

int	check_eat_philo(t_data *data)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (i < data->num)
	{
		pthread_mutex_lock(data->philos[i].eat_mutex);
		if (data->philos[i].must_eat == 0)
			count++;
		pthread_mutex_unlock(data->philos[i].eat_mutex);
		i++;
		usleep(400);
	}
	if (count == data->num)
	{
		pthread_mutex_lock(&data->dead_mutex);
		data->alive = 0;
		pthread_mutex_unlock(&data->dead_mutex);
		return (1);
	}
	return (0);
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
		printf("%.0f %d is sleeping\n", get_time() - philo->time, \
		philo->id + 1);
	else if (flag == THINK)
		printf("%.0f %d is thinking\n", get_time() - philo->time, \
		philo->id + 1);
	else if (flag == DEAD)
		printf("%.0f %d died\n", get_time() - philo->time, philo->id + 1);
	pthread_mutex_unlock(philo->write_mutex);
}
