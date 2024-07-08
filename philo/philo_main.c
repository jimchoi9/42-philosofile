/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jimchoi <jimchoi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 17:16:39 by jimchoi           #+#    #+#             */
/*   Updated: 2024/07/08 17:16:21 by jimchoi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*philo(void *data)
{
	t_ph	*philo;

	philo = data;
	pthread_mutex_lock(philo->start_mutex);
	pthread_mutex_unlock(philo->start_mutex);
	pthread_mutex_lock(philo->eat_mutex);
	philo->last_eat_time = get_time();
	pthread_mutex_unlock(philo->eat_mutex);
	if (philo->id % 2 == 0 && philo->one != 1)
		ph_usleep(philo->time_to_eat, philo);
	while (1)
	{
		pthread_mutex_lock(philo->dead_mutex);
		if (*philo->alive == 0)
			return (pthread_mutex_unlock(philo->dead_mutex), NULL);
		pthread_mutex_unlock(philo->dead_mutex);
		if (ph_take(philo))
			continue ;
		ph_eat(philo);
		ph_sleep(philo);
		ph_write(philo, THINK);
		usleep(300);
	}
	return (NULL);
}

void	*monitoring(void *arg)
{
	t_data	*data;

	data = (t_data *)arg;
	pthread_mutex_lock(&data->start_mutex);
	pthread_mutex_unlock(&data->start_mutex);
	while (1)
	{
		if (check_dead_philo(data) || check_eat_philo(data))
			break ;
		usleep(400);
	}
	return (NULL);
}

int	make_thread(t_data *data)
{
	pthread_t	monitor;
	int			i;

	i = -1;
	pthread_mutex_lock(&data->start_mutex);
	if (pthread_create(&monitor, NULL, &monitoring, data))
		return (printf("Error: unable to create thread\n"));
	while (++i < data->num)
	{
		if (pthread_create(&data->philos[i].thread, NULL, &philo, \
		&data->philos[i]))
			return (printf("Error: unable to create thread\n"));
	}
	pthread_mutex_unlock(&data->start_mutex);
	if (pthread_join(monitor, NULL) != 0)
		return (printf("Error: unable to join thread\n"));
	i = -1;
	while (++i < data->num)
	{
		if (pthread_join(data->philos[i].thread, NULL))
			return (printf("Error: unable to join thread\n"));
	}
	return (0);
}

int	main(int argc, char *argv[])
{
	t_data	*data;
	int		empty;

	empty = 0;
	if (check_arg(argc, argv))
		return (1);
	data = malloc(sizeof(t_data));
	data = memset(data, 0, sizeof(t_data));
	pthread_mutex_init(&data->start_mutex, NULL);
	pthread_mutex_init(&data->dead_mutex, NULL);
	pthread_mutex_init(&data->write_mutex, NULL);
	if (init_data(argc, argv, data, -1))
	{
		printf("Error: invalid arguments\n");
		return (1);
	}
	if (data->num == 1)
	{
		data->philos[0].one = 1;
		data->philos[0].l_fork = &empty;
	}
	make_thread(data);
	free_thread(data);
	return (0);
}
