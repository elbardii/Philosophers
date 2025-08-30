/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isel-bar <isel-bar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 15:39:10 by isel-bar          #+#    #+#             */
/*   Updated: 2025/08/29 18:44:54 by isel-bar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	*execute_philosopher_simulation_loop(
		t_philosopher_data *current_philosopher);

void	*philosopher_routine(void *arg)
{
	t_philosopher_data	*current_philosopher;

	current_philosopher = (t_philosopher_data *)arg;
	pthread_mutex_lock(&current_philosopher->shared_table_data->
		start_synchronization_mutex);
	pthread_mutex_unlock(&current_philosopher->shared_table_data->
		start_synchronization_mutex);
	if (current_philosopher->shared_table_data->single_philosopher_mode)
		return (print_philosopher_state(current_philosopher, MSG_FORK), 
			precise_sleep_duration(current_philosopher->shared_table_data->
				time_before_death), NULL);
	if (current_philosopher->unique_number % 2 == 0)
	{
		precise_sleep_duration(current_philosopher->shared_table_data->
			eating_duration / 2);
		begin_thinking_phase(current_philosopher);
		attempt_fork_acquisition(current_philosopher);
	}
	return (execute_philosopher_simulation_loop(current_philosopher));
}

static void	*execute_philosopher_simulation_loop(
		t_philosopher_data *current_philosopher)
{
	while (get_current_simulation_state(current_philosopher->
		shared_table_data) == SIM_RUNNING)
	{
		if (execute_eating_process(current_philosopher))
		{
			if (get_current_simulation_state(current_philosopher->
				shared_table_data) != SIM_RUNNING)
				break ;
			execute_sleep_and_think_cycle(current_philosopher);
		}
		else
		{
			if (current_philosopher->unique_number == current_philosopher->
				shared_table_data->number_of_diners)
				precise_sleep_duration(current_philosopher->shared_table_data->
					eating_duration);
			else
				precise_sleep_duration(current_philosopher->shared_table_data->
					eating_duration / 2);
		}
	}
	return (NULL);
}

int	handle_thread_creation_error(t_table_config *table_data, char *err_msg)
{
	table_data->death_flag_detected = 1;
	set_simulation_state(table_data, SIM_ERROR);
	printf("%s\n", err_msg);
	return (pthread_mutex_unlock(&table_data->start_synchronization_mutex), 
		handle_simulation_termination(table_data),
		free_simulation_resources(table_data), 1);
}

void	print_philosopher_state(t_philosopher_data *current_philosopher, 
		char *status)
{
	long long	current_time;

	pthread_mutex_lock(&current_philosopher->shared_table_data->
		print_output_mutex);
	current_time = calculate_time_elapsed(current_philosopher->
		shared_table_data->simulation_start_time);
	printf("%lld %d %s\n", current_time, current_philosopher->unique_number, 
		status);
	pthread_mutex_unlock(&current_philosopher->shared_table_data->
		print_output_mutex);
}