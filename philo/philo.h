/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jimchoi <jimchoi@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 16:43:57 by jimchoi           #+#    #+#             */
/*   Updated: 2024/07/09 16:54:54 by jimchoi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <sys/time.h>
# include <memory.h>

typedef enum e_flag
{
	FORK,
	EAT,
	SLEEP,
	THINK,
	DEAD,
}	t_flag;

typedef struct s_ph
{
	int				id;
	int				one;
	int				must_eat;
	int				*l_fork;
	int				*r_fork;
	double			time;
	double			last_eat_time;
	int				*alive;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;

	pthread_mutex_t	*l_mutex;
	pthread_mutex_t	*r_mutex;

	pthread_mutex_t	*start_mutex;
	pthread_mutex_t	*dead_mutex;
	pthread_mutex_t	*eat_mutex;
	pthread_mutex_t	*write_mutex;
	pthread_t		thread;

}	t_ph;

typedef struct s_data
{
	int				num;
	int				alive;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				must_eat;
	double			time;

	int				*forks;
	pthread_mutex_t	*fork_mutex;
	pthread_mutex_t	*eat_mutex;

	t_ph			*philos;

	pthread_mutex_t	start_mutex;
	pthread_mutex_t	dead_mutex;
	pthread_mutex_t	write_mutex;

}	t_data;

void		*philo(void *data);
void		*monitoring(void *arg);
int			make_thread(t_data *data);
int			main(int argc, char *argv[]);

long long	philo_atoi(const char *str);
double		get_time(void);
int			ph_isdigit(int c);
int			ph_strlen(char *s);
void		ph_usleep(int ms, t_ph *philo);
void		ph_write(t_ph *philo, t_flag flag);
void		free_thread(t_data *data);
int			ph_take(t_ph *philo);
void		ph_eat(t_ph *philo);
void		ph_sleep(t_ph *philo);
int			check_dead_philo(t_data *data);
int			check_eat_philo(t_data *data);
t_ph		*init_philos(t_data *data, int i);
int			init_data(int argc, char *argv[], t_data *data);
int			check_arg(int argc, char *argv[]);
void		init_mutex(t_data *data);

#endif