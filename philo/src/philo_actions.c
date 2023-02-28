/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philo_actions.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: hman <hman@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/06/15 13:35:57 by hman          #+#    #+#                 */
/*   Updated: 2022/07/01 17:57:38 by hman          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

//	function that will print out the thinking message
void	start_thinking(t_sim_data *sim_data, int philo_nbr)
{
	usleep(100);
	print_message(sim_data, MSG_THINKING, philo_nbr);
}

//	function that will update the last eat time and start the eating process
void	start_eating(t_sim_data *sim_data, int philo_nbr)
{
	struct timeval	cur_time;

	pthread_mutex_lock(&sim_data->last_eat_mut[philo_nbr - 1]);
	gettimeofday(&cur_time, NULL);
	sim_data->last_eat_arr[philo_nbr -1] = cur_time;
	pthread_mutex_unlock(&sim_data->last_eat_mut[philo_nbr - 1]);
	print_message(sim_data, MSG_EAT, philo_nbr);
	stricter_sleep_milli(sim_data->time_to_eat);
}

//	function that will unlock the locked fork mutexes
void	lay_down_fork(t_sim_data *sim_data, int fork_type, int philo_nbr)
{
	int	fork_index;

	if (philo_nbr != sim_data->amnt_philos && fork_type == RIGHT_FORK)
		fork_index = philo_nbr;
	else if (philo_nbr == sim_data->amnt_philos && fork_type == RIGHT_FORK)
		fork_index = 0;
	else
		fork_index = philo_nbr - 1;
	pthread_mutex_unlock(&sim_data->forks_mut[fork_index]);
}

//	this function will attempt to lock appropriate fork for the philosopher
void	take_fork(t_sim_data *sim_data, int fork_type, int philo_nbr)
{
	int	fork_index;

	if (philo_nbr != sim_data->amnt_philos && fork_type == RIGHT_FORK)
		fork_index = philo_nbr;
	else if (philo_nbr == sim_data->amnt_philos && fork_type == RIGHT_FORK)
		fork_index = 0;
	else
		fork_index = philo_nbr - 1;
	pthread_mutex_lock(&sim_data->forks_mut[fork_index]);
	print_message(sim_data, MSG_FORK, philo_nbr);
}

//	this function will be called after the philosopher has eaten
void	start_sleeping(t_sim_data *sim_data, int philo_nbr)
{
	print_message(sim_data, MSG_SLEEP, philo_nbr);
	stricter_sleep_milli(sim_data->time_to_sleep);
}
