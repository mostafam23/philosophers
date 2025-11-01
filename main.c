/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-moha <mel-moha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 09:51:24 by mel-moha          #+#    #+#             */
/*   Updated: 2025/09/09 12:50:18 by mel-moha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	handle_death(t_data *data, int philo_id, long long current_time)
{
	pthread_mutex_lock(&data->data_mutex);
	if (!data->simulation_end)
	{
		data->simulation_end = 1;
		pthread_mutex_unlock(&data->data_mutex);
		pthread_mutex_lock(&data->print_mutex);
		printf("%lld %d died\n", current_time - data->start_time, philo_id);
		pthread_mutex_unlock(&data->print_mutex);
	}
	else
	{
		pthread_mutex_unlock(&data->data_mutex);
	}
	return (1);
}

static void	cleanup_single_philo(t_data *data)
{
	if (data->forks)
		free(data->forks);
	if (data->philos)
		free(data->philos);
}

static int	handle_single_philo(t_data *data)
{
	printf("0 1 has taken a fork\n");
	usleep(data->time_to_die * 1000);
	printf("%d 1 died\n", data->time_to_die);
	cleanup_single_philo(data);
	return (0);
}

int	main(int argc, char **argv)
{
	t_data	data;

	if (argc != 5 && argc != 6)
	{
		printf("Usage: %s nb_philo time_to_die time_to_eat \
			time_to_sleep [nb_must_eat]\n", argv[0]);
		return (1);
	}
	if (!validate_input(argc, argv))
		return (printf("Error: Invalid arguments\n"), 1);
	if (init_data(&data, argc, argv))
		return (printf("Error: Initialization failed\n"), 1);
	if (data.nb_philo == 1)
		return (handle_single_philo(&data));
	if (init_mutexes(&data))
	{
		printf("Error: Mutex initialization failed\n");
		cleanup(&data);
		return (1);
	}
	init_philosophers(&data);
	start_simulation(&data);
	cleanup(&data);
	return (0);
}
