/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isel-bar <isel-bar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 15:30:13 by isel-bar          #+#    #+#             */
/*   Updated: 2025/08/29 18:44:54 by isel-bar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	execute_eating_process(t_philosopher_data *current_philosopher)
{
	long long	current_time;

	if (!attempt_both_forks_acquisition(current_philosopher))
	{
		begin_thinking_phase(current_philosopher);
		return (0);
	}
	pthread_mutex_lock(&current_philosopher->shared_table_data->
		death_check_mutex);
	current_philosopher->current_state = STATE_EATING;
	pthread_mutex_unlock(&current_philosopher->shared_table_data->
		death_check_mutex);
	print_philosopher_state(current_philosopher, MSG_EAT);
	current_time = get_current_timestamp();
	pthread_mutex_lock(&current_philosopher->shared_table_data->
		meal_tracking_mutex);
	current_philosopher->last_meal_timestamp = current_time;
	pthread_mutex_unlock(&current_philosopher->shared_table_data->
		meal_tracking_mutex);
	precise_sleep_duration(current_philosopher->shared_table_data->
		eating_duration);
	pthread_mutex_lock(&current_philosopher->shared_table_data->
		meal_tracking_mutex);
	current_philosopher->total_meals_eaten++;
	pthread_mutex_unlock(&current_philosopher->shared_table_data->
		meal_tracking_mutex);
	release_both_dining_forks(current_philosopher, 0, 0);
	return (1);
}

void	enter_sleeping_state(t_philosopher_data *current_philosopher)
{
	if (check_philosopher_death(current_philosopher))
		return ;
	pthread_mutex_lock(&current_philosopher->shared_table_data->
		death_check_mutex);
	current_philosopher->current_state = STATE_SLEEPING;
	pthread_mutex_unlock(&current_philosopher->shared_table_data->
		death_check_mutex);
	print_philosopher_state(current_philosopher, MSG_SLEEP);
	precise_sleep_duration(current_philosopher->shared_table_data->
		sleeping_duration);
}

void	begin_thinking_phase(t_philosopher_data *current_philosopher)
{
	if (check_philosopher_death(current_philosopher))
		return ;
	pthread_mutex_lock(&current_philosopher->shared_table_data->
		death_check_mutex);
	current_philosopher->current_state = STATE_THINKING;
	pthread_mutex_unlock(&current_philosopher->shared_table_data->
		death_check_mutex);
	print_philosopher_state(current_philosopher, MSG_THINK);
	adjust_thinking_duration(current_philosopher);
}

void	adjust_thinking_duration(t_philosopher_data *current_philosopher)
{
	int	wait_time;

	if (current_philosopher->shared_table_data->number_of_diners % 2 == 1)
	{
		if (current_philosopher->shared_table_data->time_before_death <= 500)
			precise_sleep_duration(5);
		else
		{
			if (current_philosopher->unique_number % 2 == 0)
			{
				wait_time = current_philosopher->shared_table_data->
					eating_duration / 4;
				precise_sleep_duration(wait_time);
			}
		}
	}
	else
		precise_sleep_duration(5);
}

void	execute_sleep_and_think_cycle(t_philosopher_data *current_philosopher)
{
	enter_sleeping_state(current_philosopher);
	if (check_philosopher_death(current_philosopher))
		return ;
	begin_thinking_phase(current_philosopher);
	attempt_fork_acquisition(current_philosopher);
}