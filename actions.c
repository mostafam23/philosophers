/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-moha <mel-moha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 09:51:08 by mel-moha          #+#    #+#             */
/*   Updated: 2025/09/09 12:50:09 by mel-moha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	take_forks(t_philo *philo)
{
	pthread_mutex_t	*first_fork;
	pthread_mutex_t	*second_fork;

	if (philo->left_fork < philo->right_fork)
	{
		first_fork = philo->left_fork;
		second_fork = philo->right_fork;
	}
	else
	{
		first_fork = philo->right_fork;
		second_fork = philo->left_fork;
	}
	pthread_mutex_lock(first_fork);
	if (simulation_ended(philo->data))
		return (pthread_mutex_unlock(first_fork), 0);
	print_status(philo, "has taken a fork");
	pthread_mutex_lock(second_fork);
	if (simulation_ended(philo->data))
	{
		pthread_mutex_unlock(second_fork);
		pthread_mutex_unlock(first_fork);
		return (0);
	}
	return (print_status(philo, "has taken a fork"), 1);
}

void	eat(t_philo *philo)
{
	print_status(philo, "is eating");
	pthread_mutex_lock(&philo->data->data_mutex);
	philo->last_meal_time = get_current_time();
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->data->data_mutex);
	precise_usleep(philo->data->time_to_eat, philo->data);
}

void	put_forks(t_philo *philo)
{
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
}

void	sleep_and_think(t_philo *philo)
{
	print_status(philo, "is sleeping");
	precise_usleep(philo->data->time_to_sleep, philo->data);
	print_status(philo, "is thinking");
	if (philo->data->nb_philo % 2 == 1)
		usleep(1000);
}

int	create_philosopher_threads(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nb_philo)
	{
		if (pthread_create(&data->philos[i].thread, NULL,
				philosopher_routine, &data->philos[i]))
		{
			printf("Error: Thread creation failed\n");
			pthread_mutex_lock(&data->data_mutex);
			data->simulation_end = 1;
			pthread_mutex_unlock(&data->data_mutex);
			return (1);
		}
		i++;
	}
	return (0);
}
