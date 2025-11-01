/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-moha <mel-moha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 09:51:58 by mel-moha          #+#    #+#             */
/*   Updated: 2025/09/09 12:51:00 by mel-moha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	start_simulation(t_data *data)
{
	pthread_t	monitor;

	init_philosophers_time(data);
	if (create_philosopher_threads(data))
		return ;
	if (pthread_create(&monitor, NULL, monitor_routine, data))
	{
		printf("Error: Monitor thread creation failed\n");
		pthread_mutex_lock(&data->data_mutex);
		data->simulation_end = 1;
		pthread_mutex_unlock(&data->data_mutex);
		join_threads(data);
		return ;
	}
	pthread_join(monitor, NULL);
	join_threads(data);
	usleep(1000);
}

void	*philosopher_routine(void *arg)
{
	t_philo	*philo;
	int		forks_acquired;

	philo = (t_philo *)arg;
	if (philo->id % 2 == 0)
		usleep(philo->data->time_to_eat * 500);
	while (!simulation_ended(philo->data))
	{
		forks_acquired = take_forks(philo);
		if (simulation_ended(philo->data))
			break ;
		if (forks_acquired)
		{
			eat(philo);
			put_forks(philo);
			if (simulation_ended(philo->data))
				break ;
			sleep_and_think(philo);
		}
		else
			break ;
	}
	return (NULL);
}

static int	check_death(t_data *data)
{
	int			i;
	long long	time_since_meal;
	long long	current_time;
	long long	last_meal;

	i = 0;
	current_time = get_current_time();
	while (i < data->nb_philo)
	{
		pthread_mutex_lock(&data->data_mutex);
		last_meal = data->philos[i].last_meal_time;
		pthread_mutex_unlock(&data->data_mutex);
		time_since_meal = current_time - last_meal;
		if (time_since_meal >= (long long)data->time_to_die)
			return (handle_death(data, data->philos[i].id, current_time));
		i++;
	}
	return (0);
}

static int	check_all_ate(t_data *data)
{
	int	i;
	int	finished_eating;

	if (data->nb_must_eat == -1)
		return (0);
	finished_eating = 0;
	pthread_mutex_lock(&data->data_mutex);
	i = 0;
	while (i < data->nb_philo)
	{
		if (data->philos[i].meals_eaten >= data->nb_must_eat)
			finished_eating++;
		i++;
	}
	if (finished_eating == data->nb_philo)
	{
		data->simulation_end = 1;
		pthread_mutex_unlock(&data->data_mutex);
		return (1);
	}
	pthread_mutex_unlock(&data->data_mutex);
	return (0);
}

void	*monitor_routine(void *arg)
{
	t_data	*data;

	data = (t_data *)arg;
	usleep(10000);
	while (!simulation_ended(data))
	{
		if (check_death(data) || check_all_ate(data))
			break ;
		usleep(1000);
	}
	return (NULL);
}
