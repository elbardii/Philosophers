/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isel-bar <isel-bar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 15:32:34 by isel-bar          #+#    #+#             */
/*   Updated: 2025/08/29 18:44:54 by isel-bar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_death_flag_status(t_table_config *table_data)
{
	int	death_status_result;

	pthread_mutex_lock(&table_data->death_check_mutex);
	death_status_result = table_data->death_flag_detected;
	pthread_mutex_unlock(&table_data->death_check_mutex);
	return (death_status_result);
}

int	check_death_timing(t_philosopher_data *current_philosopher)
{
	long long	time_since_meal;

	time_since_meal = calculate_time_since_last_meal(current_philosopher);
	return (time_since_meal >= current_philosopher->shared_table_data->
		time_before_death);
}

long long	calculate_time_since_last_meal(
		t_philosopher_data *current_philosopher)
{
	long long	current_time;
	long long	time_difference_result;

	current_time = get_current_timestamp();
	pthread_mutex_lock(&current_philosopher->shared_table_data->
		meal_tracking_mutex);
	if (current_philosopher->last_meal_timestamp == 0)
		time_difference_result = calculate_time_elapsed(current_philosopher->
			shared_table_data->simulation_start_time);
	else
		time_difference_result = current_time - 
			current_philosopher->last_meal_timestamp;
	pthread_mutex_unlock(&current_philosopher->shared_table_data->
		meal_tracking_mutex);
	return (time_difference_result);
}

int	check_philosopher_death(t_philosopher_data *current_philosopher)
{
	int	death_flag_result;

	pthread_mutex_lock(&current_philosopher->shared_table_data->
		death_check_mutex);
	death_flag_result = current_philosopher->shared_table_data->
		death_flag_detected;
	pthread_mutex_unlock(&current_philosopher->shared_table_data->
		death_check_mutex);
	return (death_flag_result);
}

int	verify_all_philosophers_fed(t_table_config *table_data)
{
	int	philosopher_index;
	int	all_have_eaten;

	if (table_data->required_meal_count < 0)
		return (0);
	pthread_mutex_lock(&table_data->meal_tracking_mutex);
	philosopher_index = 0;
	all_have_eaten = 1;
	while (philosopher_index < table_data->number_of_diners)
	{
		if (table_data->philosophers_array[philosopher_index].
			total_meals_eaten < table_data->required_meal_count)
		{
			all_have_eaten = 0;
			break ;
		}
		philosopher_index++;
	}
	pthread_mutex_unlock(&table_data->meal_tracking_mutex);
	return (all_have_eaten);
}