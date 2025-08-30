/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isel-bar <isel-bar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 15:17:45 by isel-bar          #+#    #+#             */
/*   Updated: 2025/08/29 18:44:54 by isel-bar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	convert_string_to_integer(const char *str)
{
	int	character_index;
	int	sign_multiplier;
	int	conversion_result;

	character_index = 0;
	sign_multiplier = 1;
	conversion_result = 0;
	while (str[character_index] == ' ' || (str[character_index] >= 9 
		&& str[character_index] <= 13))
		character_index++;
	if (str[character_index] == '-' || str[character_index] == '+')
	{
		if (str[character_index] == '-')
			sign_multiplier = -1;
		character_index++;
	}
	while (str[character_index] >= '0' && str[character_index] <= '9')
	{
		conversion_result = conversion_result * 10 + 
			(str[character_index] - '0');
		character_index++;
	}
	return (conversion_result * sign_multiplier);
}

long long	get_current_timestamp(void)
{
	struct timeval	time_value;

	if (gettimeofday(&time_value, NULL) != 0)
		return (-1);
	return ((time_value.tv_sec * 1000) + (time_value.tv_usec / 1000));
}

long long	calculate_time_elapsed(long long start_time)
{
	return (get_current_timestamp() - start_time);
}

void	precise_sleep_duration(long long time)
{
	long long	start_time;
	long long	elapsed_time;
	long long	remaining_time;

	start_time = get_current_timestamp();
	while (1)
	{
		elapsed_time = get_current_timestamp() - start_time;
		if (elapsed_time >= time)
			break ;
		remaining_time = time - elapsed_time;
		if (remaining_time > 50)
			usleep(500);
		else if (remaining_time > 10)
			usleep(200);
		else
			usleep(50);
	}
}

int	check_simulation_state(t_table_config *table_data, int target_state)
{
	int	current_state_result;

	pthread_mutex_lock(&table_data->death_check_mutex);
	current_state_result = (table_data->simulation_state == target_state);
	pthread_mutex_unlock(&table_data->death_check_mutex);
	return (current_state_result);
}