/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isel-bar <isel-bar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 15:26:46 by isel-bar          #+#    #+#             */
/*   Updated: 2025/08/29 18:44:54 by isel-bar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	attempt_and_acquire_both_forks(
		t_philosopher_data *current_philosopher, int first_fork, 
		int second_fork);

int	check_fork_availability(t_philosopher_data *current_philosopher, 
		int fork_index)
{
	int	fork_available;

	pthread_mutex_lock(&current_philosopher->shared_table_data->
		fork_mutex_array[fork_index].fork_mutex);
	fork_available = (current_philosopher->shared_table_data->
		fork_mutex_array[fork_index].fork_state == FORK_AVAILABLE);
	pthread_mutex_unlock(&current_philosopher->shared_table_data->
		fork_mutex_array[fork_index].fork_mutex);
	return (fork_available);
}

int	acquire_fork_safely(t_philosopher_data *current_philosopher, 
		int fork_index)
{
	pthread_mutex_lock(&current_philosopher->shared_table_data->
		fork_mutex_array[fork_index].fork_mutex);
	if (current_philosopher->shared_table_data->
		fork_mutex_array[fork_index].fork_state == FORK_AVAILABLE)
	{
		current_philosopher->shared_table_data->
			fork_mutex_array[fork_index].fork_state = FORK_IN_USE;
		current_philosopher->shared_table_data->
			fork_mutex_array[fork_index].owner_philosopher_id = 
			current_philosopher->unique_number;
		pthread_mutex_unlock(&current_philosopher->shared_table_data->
			fork_mutex_array[fork_index].fork_mutex);
		print_philosopher_state(current_philosopher, MSG_FORK);
		return (1);
	}
	else if (current_philosopher->unique_number == current_philosopher->
		shared_table_data->number_of_diners && 
		calculate_time_since_last_meal(current_philosopher) > 
		(current_philosopher->shared_table_data->time_before_death * 3 / 4))
	{
		current_philosopher->shared_table_data->
			fork_mutex_array[fork_index].fork_state = FORK_IN_USE;
		current_philosopher->shared_table_data->
			fork_mutex_array[fork_index].owner_philosopher_id = 
			current_philosopher->unique_number;
		pthread_mutex_unlock(&current_philosopher->shared_table_data->
			fork_mutex_array[fork_index].fork_mutex);
		print_philosopher_state(current_philosopher, MSG_FORK);
		return (1);
	}
	pthread_mutex_unlock(&current_philosopher->shared_table_data->
		fork_mutex_array[fork_index].fork_mutex);
	return (0);
}

int	attempt_both_forks_acquisition(t_philosopher_data *current_philosopher)
{
	if (check_death_flag_status(current_philosopher->shared_table_data))
		return (0);
	if (current_philosopher->shared_table_data->single_philosopher_mode)
	{
		acquire_fork_safely(current_philosopher, 
			current_philosopher->left_fork_index);
		precise_sleep_duration(current_philosopher->shared_table_data->
			time_before_death + 1);
		return (0);
	}
	return (attempt_and_acquire_both_forks(current_philosopher, 0, 0));
}

static int	attempt_and_acquire_both_forks(
		t_philosopher_data *current_philosopher, int first_fork, 
		int second_fork)
{
	if (current_philosopher->unique_number == current_philosopher->
		shared_table_data->number_of_diners)
	{
		first_fork = current_philosopher->left_fork_index;
		second_fork = current_philosopher->right_fork_index;
	}
	else if (current_philosopher->unique_number % 2 == 0)
	{
		first_fork = current_philosopher->right_fork_index;
		second_fork = current_philosopher->left_fork_index;
	}
	else
	{
		first_fork = current_philosopher->left_fork_index;
		second_fork = current_philosopher->right_fork_index;
	}
	if (!acquire_fork_safely(current_philosopher, first_fork))
		return (0);
	if (!acquire_fork_safely(current_philosopher, second_fork))
	{
		pthread_mutex_lock(&current_philosopher->shared_table_data->
			fork_mutex_array[first_fork].fork_mutex);
		current_philosopher->shared_table_data->
			fork_mutex_array[first_fork].fork_state = FORK_AVAILABLE;
		current_philosopher->shared_table_data->
			fork_mutex_array[first_fork].owner_philosopher_id = -1;
		return (pthread_mutex_unlock(&current_philosopher->shared_table_data->
			fork_mutex_array[first_fork].fork_mutex), 0);
	}
	return (1);
}

void	release_both_dining_forks(t_philosopher_data *current_philosopher, 
		int first_fork, int second_fork)
{
	if (current_philosopher->unique_number == current_philosopher->
		shared_table_data->number_of_diners)
	{
		first_fork = current_philosopher->right_fork_index;
		second_fork = current_philosopher->left_fork_index;
	}
	else if (current_philosopher->unique_number % 2 == 0)
	{
		first_fork = current_philosopher->left_fork_index;
		second_fork = current_philosopher->right_fork_index;
	}
	else
	{
		first_fork = current_philosopher->right_fork_index;
		second_fork = current_philosopher->left_fork_index;
	}
	pthread_mutex_lock(&current_philosopher->shared_table_data->
		fork_mutex_array[first_fork].fork_mutex);
	current_philosopher->shared_table_data->
		fork_mutex_array[first_fork].fork_state = FORK_AVAILABLE;
	current_philosopher->shared_table_data->
		fork_mutex_array[first_fork].owner_philosopher_id = -1;
	pthread_mutex_unlock(&current_philosopher->shared_table_data->
		fork_mutex_array[first_fork].fork_mutex);
	pthread_mutex_lock(&current_philosopher->shared_table_data->
		fork_mutex_array[second_fork].fork_mutex);
	current_philosopher->shared_table_data->
		fork_mutex_array[second_fork].fork_state = FORK_AVAILABLE;
	current_philosopher->shared_table_data->
		fork_mutex_array[second_fork].owner_philosopher_id = -1;
	pthread_mutex_unlock(&current_philosopher->shared_table_data->
		fork_mutex_array[second_fork].fork_mutex);
}