/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-moha <mel-moha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 09:52:04 by mel-moha          #+#    #+#             */
/*   Updated: 2025/09/09 10:07:31 by mel-moha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long long	get_current_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	precise_usleep(int time_ms, t_data *data)
{
	long long	start_time;
	long long	current_time;
	long long	elapsed;

	start_time = get_current_time();
	while (1)
	{
		current_time = get_current_time();
		elapsed = current_time - start_time;
		if (elapsed >= time_ms)
			break ;
		if (simulation_ended(data))
			break ;
		if (time_ms - elapsed > 20)
			usleep(10000);
		else if (time_ms - elapsed > 5)
			usleep(1000);
		else
			usleep(500);
	}
}

void	print_status(t_philo *philo, char *status)
{
	long long	timestamp;

	pthread_mutex_lock(&philo->data->print_mutex);
	if (!simulation_ended(philo->data))
	{
		timestamp = get_current_time() - philo->data->start_time;
		printf("%lld %d %s\n", timestamp, philo->id, status);
	}
	pthread_mutex_unlock(&philo->data->print_mutex);
}

int	simulation_ended(t_data *data)
{
	int	ended;

	pthread_mutex_lock(&data->data_mutex);
	ended = data->simulation_end;
	pthread_mutex_unlock(&data->data_mutex);
	return (ended);
}

void	join_threads(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nb_philo)
	{
		pthread_join(data->philos[i].thread, NULL);
		i++;
	}
}
