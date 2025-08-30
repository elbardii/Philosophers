/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_routine.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isel-bar <isel-bar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 14:55:58 by isel-bar          #+#    #+#             */
/*   Updated: 2025/08/30 02:48:24 by isel-bar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	*handle_single_philosopher_case(t_table_config *table_data)
{
	precise_sleep_duration(table_data->time_before_death + 1);
	pthread_mutex_lock(&table_data->death_check_mutex);
	table_data->death_flag_detected = 1;
	table_data->philosophers_array[0].current_state = STATE_DEAD;
	pthread_mutex_unlock(&table_data->death_check_mutex);
	print_philosopher_state(&table_data->philosophers_array[0], MSG_DIED);
	set_simulation_state(table_data, SIM_STOPPED);
	return (NULL);
}

static int	check_philosopher_death_condition(t_table_config *table_data, 
		int philosopher_index)
{
	long long	last_meal_time;
	long long	elapsed_time;

	pthread_mutex_lock(&table_data->meal_tracking_mutex);
	last_meal_time = table_data->philosophers_array[philosopher_index].
		last_meal_timestamp;
	pthread_mutex_unlock(&table_data->meal_tracking_mutex);
	elapsed_time = calculate_time_elapsed(last_meal_time);
	return (elapsed_time > table_data->time_before_death);
}

static void	*handle_philosopher_death_event(t_table_config *table_data, 
		int philosopher_index)
{
	pthread_mutex_lock(&table_data->death_check_mutex);
	if (!table_data->death_flag_detected)
	{
		table_data->philosophers_array[philosopher_index].current_state = 
			STATE_DEAD;
		table_data->death_flag_detected = 1;
		pthread_mutex_unlock(&table_data->death_check_mutex);
		print_philosopher_state(&table_data->
			philosophers_array[philosopher_index], MSG_DIED);
		set_simulation_state(table_data, SIM_STOPPED);
		return (NULL);
	}
	pthread_mutex_unlock(&table_data->death_check_mutex);
	return (NULL);
}

static void	*execute_monitoring_loop(t_table_config *table_data)
{
	int	philosopher_index;

	philosopher_index = 0;
	while (check_simulation_state(table_data, SIM_RUNNING))
	{
		if (philosopher_index < table_data->number_of_diners)
		{
			if (check_philosopher_death_condition(table_data, 
				philosopher_index))
				return (handle_philosopher_death_event(table_data, 
					philosopher_index), NULL);
			philosopher_index++;
		}
		else
		{
			philosopher_index = 0;
			if (verify_all_philosophers_fed(table_data) && 
				check_simulation_state(table_data, SIM_RUNNING))
				return (handle_simulation_completion(table_data), NULL);
			precise_sleep_duration(1);
		}
	}
	return (NULL);
}

void	*monitor_simulation_routine(void *arg)
{
	t_table_config	*table_data;

	table_data = (t_table_config *)arg;
	pthread_mutex_lock(&table_data->start_synchronization_mutex);
	pthread_mutex_unlock(&table_data->start_synchronization_mutex);
	if (table_data->single_philosopher_mode)
		return (handle_single_philosopher_case(table_data), NULL);
	return (execute_monitoring_loop(table_data), NULL);
}