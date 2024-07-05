#include "p_.h"

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
double	get_current_time()
{
	double now;
	struct timeval time;

	gettimeofday(&time, NULL);
	 now = (time.tv_sec * 1000 +  time.tv_usec / 1000);
	return (now );
}

int	check_dead_philo(t_data *data)
{
	int i;
	int count = 0;;

	i=0;
	while (i < data->num)
	{
		pthread_mutex_lock(data->philos[i].eat_mutex);
		if (data->philos[i].last_eat_time != 0 && get_current_time() - data->philos[i].last_eat_time > data->time_to_die)
		{
			pthread_mutex_lock(data->philos[i].dead_mutex);
			data->alive = 0;
			pthread_mutex_unlock(data->philos[i].dead_mutex);
			ph_write(&data->philos[i], DEAD);
			return (1);
		}
		pthread_mutex_unlock(data->philos[i].eat_mutex);
		i++;
		usleep(200);
	}
	pthread_mutex_unlock(&data->dead_mutex);
	return (0);
}
int	check_eat_philo(t_data *data)
{
	int i;
	int count = 0;;

	i=0;
	while (i < data->num)
	{
		pthread_mutex_lock(data->philos[i].eat_mutex);
		if (data->philos[i].must_eat == 0)
			count++;
		pthread_mutex_unlock(data->philos[i].eat_mutex);
		if (count == data->num)
		{
			pthread_mutex_lock(&data->dead_mutex);
			data->alive = 0;
			pthread_mutex_unlock(&data->dead_mutex);
			pthread_mutex_unlock(data->philos[i].eat_mutex);
			return (1);
		}
		i++;
		usleep(200);
	}
	pthread_mutex_unlock(&data->dead_mutex);
	return (0);
}

void	ft_usleep(int ms, t_ph *philo)
{
	double	start;

	start = get_current_time();
	while ((get_current_time() - start) < ms)
	{
		pthread_mutex_lock(philo->dead_mutex);
		if (philo->alive == 0)
		{
			pthread_mutex_unlock(philo->dead_mutex);
			return ;
		}
		pthread_mutex_unlock(philo->dead_mutex);
		usleep(200);
	}
	return ;
}

int ph_take(t_ph *philo)
{
	pthread_mutex_lock(philo->r_mutex);
	if (*philo->r_fork == -1)
	{
		ph_write(philo, FORK);
		*philo->r_fork = philo->id;
	}
	else
	{
		pthread_mutex_unlock(philo->r_mutex);
		return (1);
	}
	pthread_mutex_unlock(philo->r_mutex);
	pthread_mutex_lock(philo->l_mutex);
	if (*philo->l_fork == -1)
	{
		ph_write(philo, FORK);
		*philo->l_fork = philo->id;
	}
	else 
	{
		pthread_mutex_unlock(philo->l_mutex);
		return (1);
	}
	pthread_mutex_unlock(philo->l_mutex);
	return (0);
}

void ph_eat(t_ph *philo)
{

	pthread_mutex_lock(philo->r_mutex);
	pthread_mutex_lock(philo->l_mutex);
	if (*philo->l_fork == philo->id && *philo->r_fork == philo->id)
	{
		ph_write(philo, EAT);
		pthread_mutex_lock(philo->eat_mutex);
		philo->last_eat_time = get_current_time();
		pthread_mutex_unlock(philo->eat_mutex);
		pthread_mutex_lock(philo->eat_mutex);
		if (philo->must_eat > 0)
			philo->must_eat--;
		pthread_mutex_unlock(philo->eat_mutex);
		ft_usleep(philo->time_to_eat, philo);
		*philo->l_fork = -1;
		*philo->r_fork = -1;
	}
	pthread_mutex_unlock(philo->r_mutex);
	pthread_mutex_unlock(philo->l_mutex);
}

void ph_sleep(t_ph *philo)
{
	ph_write(philo, SLEEP);
	usleep(philo->time_to_sleep * 1000);
}

void ph_think(t_ph *philo)
{
	ph_write(philo, THINK);
}

void free_thread(t_data *data)
{
		pthread_mutex_destroy(&data->start_mutex);
		pthread_mutex_destroy(&data->dead_mutex);
		pthread_mutex_destroy(&data->write_mutex);

	for (int i = 0; i < data->num; i++)
	{
		pthread_mutex_destroy(&data->eat_mutex[i]);
		pthread_mutex_destroy(&data->fork_mutex[i]);

	}
	free(data->philos);
	free(data->forks);
	free(data->fork_mutex);
	free(data->eat_mutex);
	free(data);
}

void    ph_write(t_ph *philo, t_flag flag)
{
	pthread_mutex_lock(philo->dead_mutex);
	if (!*philo->alive)
	{
		pthread_mutex_unlock(philo->dead_mutex);
		return ;
	}
	pthread_mutex_unlock(philo->dead_mutex);
	pthread_mutex_lock(philo->write_mutex);
	if (flag == FORK)
		printf("%.0f %d has taken a fork\n", get_current_time() - philo->start_time, philo->id + 1);
	else if (flag == EAT)
		printf("%.0f %d is eating\n", get_current_time() - philo->start_time, philo->id + 1);
	else if (flag == SLEEP)
	printf("%.0f %d is sleeping\n", get_current_time() - philo->start_time, philo->id + 1);
	else if (flag == THINK)
		printf("%.0f %d is thinking\n", get_current_time() - philo->start_time, philo->id + 1);
	else if (flag == DEAD)
		printf("%.0f %d died\n", get_current_time() - philo->start_time, philo->id + 1);
	pthread_mutex_unlock(philo->write_mutex);
}
