/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isel-bar <isel-bar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 15:16:35 by isel-bar          #+#    #+#             */
/*   Updated: 2025/08/29 18:44:54 by isel-bar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	setup_dining_table(t_table_config *table_data, int argc, char **argv)
{
	if (argc != 5 && argc != 6)
		return (printf("%s\n", ERR_WRONG_ARG), 1);
	if (initialize_simulation_parameters(table_data, argc, argv))
		return (printf("%s\n", ERR_DATA_INIT), 1);
	if (initialize_philosophers_array(table_data) != 0)
		return (printf("%s\n", ERR_PHILO_INIT), 
			free_simulation_resources(table_data), 1);
	return (0);
}

int	main(int argc, char **argv)
{
	t_table_config	table_data;
	int				philosopher_index;

	if (setup_dining_table(&table_data, argc, argv) != 0)
		return (1);
	table_data.simulation_start_time = get_current_timestamp();
	pthread_mutex_lock(&table_data.start_synchronization_mutex);
	philosopher_index = 0;
	while (philosopher_index < table_data.number_of_diners)
	{
		table_data.philosophers_array[philosopher_index].last_meal_timestamp 
			= get_current_timestamp();
		if (pthread_create(&table_data.philosophers_array[philosopher_index].
			philosopher_thread, NULL, philosopher_routine,
			&table_data.philosophers_array[philosopher_index]) != 0)
			return (handle_thread_creation_error(&table_data, 
				ERR_CREATE_THREAD));
		philosopher_index++;
	}
	if (philosopher_index == table_data.number_of_diners)
	{
		if (pthread_create(&table_data.monitor_thread, NULL,
			monitor_simulation_routine, &table_data) != 0)
			return (handle_thread_creation_error(&table_data, 
				ERR_CREATE_MONITOR));
	}
	return (pthread_mutex_unlock(&table_data.start_synchronization_mutex), 
		handle_simulation_termination(&table_data),
		free_simulation_resources(&table_data), 0);
}