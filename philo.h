/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-moha <mel-moha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 09:51:35 by mel-moha          #+#    #+#             */
/*   Updated: 2025/09/09 12:50:25 by mel-moha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <pthread.h>
# include <sys/time.h>
# include <limits.h>

typedef struct s_philo	t_philo;

typedef struct s_data
{
	int				nb_philo;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				nb_must_eat;
	int				simulation_end;
	long long		start_time;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	data_mutex;
	t_philo			*philos;
}	t_data;

typedef struct s_philo
{
	int				id;
	int				meals_eaten;
	long long		last_meal_time;
	pthread_t		thread;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	t_data			*data;
}	t_philo;

int			validate_input(int argc, char **argv);
int			init_data(t_data *data, int argc, char **argv);
void		init_philosophers(t_data *data);
int			init_mutexes(t_data *data);
void		start_simulation(t_data *data);
void		*philosopher_routine(void *arg);
void		*monitor_routine(void *arg);
int			take_forks(t_philo *philo);
void		eat(t_philo *philo);
void		put_forks(t_philo *philo);
void		sleep_and_think(t_philo *philo);
long long	get_current_time(void);
void		precise_usleep(int time_ms, t_data *data);
void		print_status(t_philo *philo, char *status);
int			simulation_ended(t_data *data);
void		cleanup(t_data *data);
void		join_threads(t_data *data);
int			ft_atoi(const char *str);
int			ft_isdigit(int c);
size_t		ft_strlen(const char *s);
int			create_philosopher_threads(t_data *data);
void		init_philosophers_time(t_data *data);
int			handle_death(t_data *data, int philo_id, long long current_time);

#endif