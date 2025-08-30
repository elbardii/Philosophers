/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isel-bar <isel-bar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 12:00:00 by isel-bar          #+#    #+#             */
/*   Updated: 2025/08/29 18:44:27 by isel-bar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

typedef enum e_action_state
{
	STATE_THINKING,
	STATE_EATING,
	STATE_SLEEPING,
	STATE_DEAD
}					t_action_state;

typedef enum e_fork_status
{
	FORK_AVAILABLE,
	FORK_IN_USE
}					t_fork_status;

typedef enum e_simulation_status
{
	SIM_RUNNING,
	SIM_STOPPED,
	SIM_ERROR,
	SIM_COMPLETED
}					t_simulation_status;

# define SUCCESS_CODE 0
# define ERROR_CODE 1
# define DEATH_CHECK_INTERVAL 1000
# define MIN_PHILOSOPHERS 1
# define MAX_PHILOSOPHERS 200
# define MILLISECONDS_TO_MICROSECONDS 1000

# define MSG_FORK "has taken a fork"
# define MSG_EAT "is eating"
# define MSG_SLEEP "is sleeping"
# define MSG_THINK "is thinking"
# define MSG_DIED "died"
# define MSG_COMPLETE "All philosophers have eaten enough meals"

# define RESOURCE_MUTEX 1
# define RESOURCE_MEMORY 2
# define RESOURCE_THREAD 3

# define ERR_CREATE_MONITOR "Error: failed to create monitor thread"
# define ERR_CREATE_THREAD "Error: failed to create thread"
# define ERR_WRONG_ARG "Error: wrong number of arguments"
# define ERR_DATA_INIT "Error: initialization failed"
# define ERR_PHILO_INIT "Error: philosopher initialization failed"

typedef struct s_dining_fork
{
	pthread_mutex_t	fork_mutex;
	int				fork_state;
	int				owner_philosopher_id;
}					t_dining_fork;

typedef struct s_philosopher_data
{
	int					unique_number;
	int					current_state;
	int					left_fork_index;
	int					right_fork_index;
	int					total_meals_eaten;
	long long			last_meal_timestamp;
	pthread_t			philosopher_thread;
	struct s_table_config	*shared_table_data;
}						t_philosopher_data;

typedef struct s_table_config
{
	int					number_of_diners;
	int					time_before_death;
	int					eating_duration;
	int					sleeping_duration;
	int					required_meal_count;
	int					death_flag_detected;
	int					simulation_state;
	int					single_philosopher_mode;
	long long			simulation_start_time;
	t_dining_fork		*fork_mutex_array;
	pthread_mutex_t		print_output_mutex;
	pthread_mutex_t		death_check_mutex;
	pthread_mutex_t		start_synchronization_mutex;
	pthread_mutex_t		meal_tracking_mutex;
	pthread_t			monitor_thread;
	t_philosopher_data	*philosophers_array;
}						t_table_config;

int					free_simulation_resources(t_table_config *table_data);
int					cleanup_single_mutex(pthread_mutex_t *mutex);
long long			get_current_timestamp(void);
int					convert_string_to_integer(const char *str);
long long			calculate_time_elapsed(long long start_time);
void				precise_sleep_duration(long long time);
int					check_simulation_state(t_table_config *table_data, 
						int target_state);
void				release_both_dining_forks(t_philosopher_data *philosopher, 
						int first_fork, int second_fork);
int					check_death_flag_status(t_table_config *table_data);
int					execute_eating_process(t_philosopher_data *philosopher);
int					attempt_both_forks_acquisition(
						t_philosopher_data *philosopher);
int					verify_all_philosophers_fed(t_table_config *table_data);
void				handle_simulation_completion(t_table_config *table_data);
void				begin_thinking_phase(t_philosopher_data *philosopher);
void				adjust_thinking_duration(t_philosopher_data *philosopher);
int					check_fork_availability(t_philosopher_data *philosopher, 
						int fork_index);
int					acquire_fork_safely(t_philosopher_data *philosopher, 
						int fork_index);
int					check_death_timing(t_philosopher_data *philosopher);
long long			calculate_time_since_last_meal(
						t_philosopher_data *philosopher);
void				handle_simulation_termination(t_table_config *table_data);
int					initialize_simulation_parameters(t_table_config *table_data,
						int argc, char **argv);
int					initialize_mutex_resources(t_table_config *table_data);
int					initialize_fork_mutexes(t_table_config *table_data);
int					initialize_philosophers_array(t_table_config *table_data);
void				*monitor_simulation_routine(void *arg);
int					handle_thread_creation_error(t_table_config *table_data, 
						char *err_msg);
int					get_current_simulation_state(t_table_config *table_data);
void				*philosopher_routine(void *arg);
void				execute_sleep_and_think_cycle(
						t_philosopher_data *philosopher);
int					attempt_fork_acquisition(t_philosopher_data *philosopher);
void				print_philosopher_state(t_philosopher_data *philosopher, 
						char *status);
int					check_philosopher_death(t_philosopher_data *philosopher);
int					set_simulation_state(t_table_config *table_data, int state);
void				enter_sleeping_state(t_philosopher_data *philosopher);
int					cleanup_fork_resources(t_dining_fork *forks, int count);
int					validate_command_arguments(int argc, char **argv);
int					parse_simulation_arguments(t_table_config *table_data, 
						int argc, char **argv);
#endif