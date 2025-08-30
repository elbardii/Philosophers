/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isel-bar <isel-bar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 15:21:27 by isel-bar          #+#    #+#             */
/*   Updated: 2025/08/29 18:44:54 by isel-bar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	initialize_simulation_parameters(t_table_config *table_data, int argc, 
		char **argv)
{
	if (validate_command_arguments(argc, argv))
		return (1);
	if (parse_simulation_arguments(table_data, argc, argv))
		return (1);
	table_data->death_flag_detected = 0;
	table_data->simulation_state = SIM_RUNNING;
	if (table_data->number_of_diners == 1)
		table_data->single_philosopher_mode = 1;
	else
		table_data->single_philosopher_mode = 0;
	if (initialize_mutex_resources(table_data) != 0)
		return (1);
	if (initialize_fork_mutexes(table_data) != 0)
	{
		cleanup_single_mutex(&table_data->print_output_mutex);
		cleanup_single_mutex(&table_data->death_check_mutex);
		cleanup_single_mutex(&table_data->start_synchronization_mutex);
		cleanup_single_mutex(&table_data->meal_tracking_mutex);
		return (1);
	}
	return (0);
}

int	initialize_mutex_resources(t_table_config *table_data)
{
	if (pthread_mutex_init(&table_data->print_output_mutex, NULL) != 0)
		return (1);
	if (pthread_mutex_init(&table_data->death_check_mutex, NULL) != 0)
		return (cleanup_single_mutex(&table_data->print_output_mutex), 1);
	if (pthread_mutex_init(&table_data->start_synchronization_mutex, 
		NULL) != 0)
	{
		cleanup_single_mutex(&table_data->print_output_mutex);
		cleanup_single_mutex(&table_data->death_check_mutex);
		return (1);
	}
	if (pthread_mutex_init(&table_data->meal_tracking_mutex, NULL) != 0)
	{
		cleanup_single_mutex(&table_data->print_output_mutex);
		cleanup_single_mutex(&table_data->death_check_mutex);
		cleanup_single_mutex(&table_data->start_synchronization_mutex);
		return (1);
	}
	return (0);
}

int	initialize_fork_mutexes(t_table_config *table_data)
{
	int	fork_index;

	table_data->fork_mutex_array = malloc(sizeof(t_dining_fork) 
		* table_data->number_of_diners);
	if (!table_data->fork_mutex_array)
		return (1);
	fork_index = 0;
	while (fork_index < table_data->number_of_diners)
	{
		if (pthread_mutex_init(&table_data->fork_mutex_array[fork_index].
			fork_mutex, NULL) != 0)
		{
			while (--fork_index >= 0)
				cleanup_single_mutex(&table_data->
					fork_mutex_array[fork_index].fork_mutex);
			free(table_data->fork_mutex_array);
			return (1);
		}
		table_data->fork_mutex_array[fork_index].fork_state = FORK_AVAILABLE;
		table_data->fork_mutex_array[fork_index].owner_philosopher_id = -1;
		fork_index++;
	}
	return (0);
}

int	initialize_philosophers_array(t_table_config *table_data)
{
	int	philosopher_index;

	table_data->philosophers_array = malloc(sizeof(t_philosopher_data) 
		* table_data->number_of_diners);
	if (!table_data->philosophers_array)
		return (1);
	philosopher_index = 0;
	while (philosopher_index < table_data->number_of_diners)
	{
		table_data->philosophers_array[philosopher_index].unique_number 
			= philosopher_index + 1;
		table_data->philosophers_array[philosopher_index].current_state 
			= STATE_THINKING;
		table_data->philosophers_array[philosopher_index].left_fork_index 
			= philosopher_index;
		table_data->philosophers_array[philosopher_index].right_fork_index 
			= (philosopher_index + 1) % table_data->number_of_diners;
		table_data->philosophers_array[philosopher_index].total_meals_eaten = 0;
		table_data->philosophers_array[philosopher_index].last_meal_timestamp 
			= 0;
		table_data->philosophers_array[philosopher_index].shared_table_data 
			= table_data;
		philosopher_index++;
	}
	return (0);
}

int	parse_simulation_arguments(t_table_config *table_data, int argc, 
		char **argv)
{
	table_data->number_of_diners = convert_string_to_integer(argv[1]);
	table_data->time_before_death = convert_string_to_integer(argv[2]);
	table_data->eating_duration = convert_string_to_integer(argv[3]);
	table_data->sleeping_duration = convert_string_to_integer(argv[4]);
	if (argc == 6)
		table_data->required_meal_count = convert_string_to_integer(argv[5]);
	else
		table_data->required_meal_count = -1;
	if (table_data->number_of_diners <= 0 || table_data->time_before_death <= 0
		|| table_data->eating_duration <= 0 
		|| table_data->sleeping_duration <= 0 || (argc == 6
			&& table_data->required_meal_count <= 0))
		return (1);
	return (0);
}