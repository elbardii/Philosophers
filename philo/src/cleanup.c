/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isel-bar <isel-bar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 15:25:09 by isel-bar          #+#    #+#             */
/*   Updated: 2025/08/29 18:44:54 by isel-bar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	free_simulation_resources(t_table_config *table_data)
{
	if (table_data->philosophers_array)
	{
		free(table_data->philosophers_array);
		table_data->philosophers_array = NULL;
	}
	if (table_data->fork_mutex_array)
	{
		cleanup_fork_resources(table_data->fork_mutex_array, 
			table_data->number_of_diners);
		table_data->fork_mutex_array = NULL;
	}
	cleanup_single_mutex(&table_data->print_output_mutex);
	cleanup_single_mutex(&table_data->death_check_mutex);
	cleanup_single_mutex(&table_data->start_synchronization_mutex);
	cleanup_single_mutex(&table_data->meal_tracking_mutex);
	return (0);
}

int	cleanup_single_mutex(pthread_mutex_t *mutex)
{
	int	destruction_result;

	destruction_result = pthread_mutex_destroy(mutex);
	if (destruction_result == 0)
		return (0);
	else
		return (1);
}

int	cleanup_fork_resources(t_dining_fork *forks, int count)
{
	int	fork_index;
	int	cleanup_success;

	fork_index = 0;
	cleanup_success = 1;
	while (fork_index < count)
	{
		if (cleanup_single_mutex(&forks[fork_index].fork_mutex) != 0)
			cleanup_success = 0;
		fork_index++;
	}
	free(forks);
	if (cleanup_success)
		return (0);
	else
		return (1);
}