/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_parsing.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jimchoi <jimchoi@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 14:37:26 by jimchoi           #+#    #+#             */
/*   Updated: 2024/07/11 12:36:20 by jimchoi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

t_ph	*init_philos(t_data *data, int i)
{
	t_ph	*philos;

	philos = malloc(sizeof(t_ph) * data->num);
	if (philos == NULL)
		return (NULL);
	philos = memset(philos, 0, sizeof(t_ph) * data->num);
	while (++i < data->num)
	{
		philos[i].id = i;
		philos[i].must_eat = data->must_eat;
		philos[i].l_fork = &data->forks[i];
		philos[i].r_fork = &data->forks[(i + 1) % data->num];
		philos[i].l_mutex = &data->fork_mutex[i];
		philos[i].r_mutex = &data->fork_mutex[(i + 1) % data->num];
		philos[i].eat_mutex = &data->eat_mutex[i];
		philos[i].alive = &data->alive;
		philos[i].time = data->time;
		philos[i].time_to_die = data->time_to_die;
		philos[i].time_to_eat = data->time_to_eat;
		philos[i].time_to_sleep = data->time_to_sleep;
		philos[i].start_mutex = &data->start_mutex;
		philos[i].dead_mutex = &data->dead_mutex;
		philos[i].write_mutex = &data->write_mutex;
	}
	return (philos);
}

void	init_mutex(t_data *data)
{
	int	i;

	i = -1;
	pthread_mutex_init(&data->start_mutex, NULL);
	pthread_mutex_init(&data->dead_mutex, NULL);
	pthread_mutex_init(&data->write_mutex, NULL);
	while (++i < data->num)
	{
		data->forks[i] = -1;
		pthread_mutex_init(&data->fork_mutex[i], NULL);
		pthread_mutex_init(&data->eat_mutex[i], NULL);
	}
}

int	init_data(int argc, char *argv[], t_data *data)
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
	data->time = get_time();
	data->forks = malloc(sizeof(int) * data->num);
	data->fork_mutex = malloc(sizeof(pthread_mutex_t) * data->num);
	data->eat_mutex = malloc(sizeof(pthread_mutex_t) * data->num);
	if (data->eat_mutex == NULL || data->fork_mutex == NULL || \
	data->forks == NULL)
		return (0);
	init_mutex(data);
	data->philos = init_philos(data, -1);
	if (data->philos == NULL)
		return (0);
	return (1);
}

int	ph_strlen(char *s)
{
	int	len;

	len = 0;
	if (!s || s[len] == '\0')
		return (0);
	while (s[len] != '\0')
		len++;
	return (len);
}

int	check_arg(int argc, char *argv[])
{
	int			i;
	int			j;
	int			len;
	long long	num;

	i = 0;
	if (argc != 5 && argc != 6)
		return (printf("Error: invalid number of arguments\n"));
	while (++i < argc)
	{
		j = -1;
		len = ph_strlen(argv[i]);
		if (len == 0)
			return (printf("Error: invalid argument values\n"));
		while (++j < len)
		{
			if (!ph_isdigit(argv[i][j]))
				return (printf("Error: invalid argument values\n"));
		}
		num = philo_atoi(argv[i]);
		if (num == -1 || (i == 1 && num > 200))
			return (printf("Error: invalid argument values\n"));
	}
	return (0);
}
