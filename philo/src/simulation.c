/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isel-bar <isel-bar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 15:36:36 by isel-bar          #+#    #+#             */
/*   Updated: 2025/08/30 02:47:35 by isel-bar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	get_current_simulation_state(t_table_config *table_data)
{
	int	current_state;

	pthread_mutex_lock(&table_data->death_check_mutex);
	current_state = table_data->simulation_state;
	pthread_mutex_unlock(&table_data->death_check_mutex);
	return (current_state);
}

int	set_simulation_state(t_table_config *table_data, int state)
{
	pthread_mutex_lock(&table_data->death_check_mutex);
	table_data->simulation_state = state;
	if (state == SIM_STOPPED)
		table_data->death_flag_detected = 1;
	pthread_mutex_unlock(&table_data->death_check_mutex);
	return (0);
}

void	handle_simulation_termination(t_table_config *table_data)
{
	int	philosopher_index;

	pthread_join(table_data->monitor_thread, NULL);
	philosopher_index = 0;
	while (philosopher_index < table_data->number_of_diners)
	{
		pthread_join(table_data->philosophers_array[philosopher_index].
			philosopher_thread, NULL);
		philosopher_index++;
	}
}


void	handle_simulation_completion(t_table_config *table_data)
{
	pthread_mutex_lock(&table_data->print_output_mutex);
	printf("%lld All philosophers have eaten enough\n",
		calculate_time_elapsed(table_data->simulation_start_time));
	pthread_mutex_unlock(&table_data->print_output_mutex);
	set_simulation_state(table_data, SIM_COMPLETED);
}

int	attempt_fork_acquisition(t_philosopher_data *current_philosopher)
{
	int	acquisition_attempts;
	int	maximum_attempts;

	acquisition_attempts = 0;
	maximum_attempts = 100;
	while (!check_philosopher_death(current_philosopher)
		&& get_current_simulation_state(current_philosopher->
			shared_table_data) == SIM_RUNNING)
	{
		if (check_fork_availability(current_philosopher, 
			current_philosopher->left_fork_index) && 
			check_fork_availability(current_philosopher,
			current_philosopher->right_fork_index))
			return (1);
		if (++acquisition_attempts > maximum_attempts)
		{
			precise_sleep_duration(current_philosopher->shared_table_data->
				eating_duration / 10);
			acquisition_attempts = 0;
		}
		else
			precise_sleep_duration(1);
	}
	return (0);
}