/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isel-bar <isel-bar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 15:24:22 by isel-bar          #+#    #+#             */
/*   Updated: 2025/08/29 18:44:54 by isel-bar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	validate_command_arguments(int argc, char **argv)
{
	int	argument_index;
	int	character_index;

	argument_index = 1;
	while (argument_index < argc)
	{
		character_index = 0;
		while (argv[argument_index][character_index])
		{
			if (argv[argument_index][character_index] < '0' || 
				argv[argument_index][character_index] > '9')
				return (1);
			character_index++;
		}
		argument_index++;
	}
	return (0);
}