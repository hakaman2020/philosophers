/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philo_func.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: hman <hman@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/06/27 15:41:02 by hman          #+#    #+#                 */
/*   Updated: 2022/06/29 14:50:04 by hman          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

//	function in which the philosopher will attempt to take the two forks
int	take_forks(t_philo *pd, int first_fork, int second_fork)
{
	if (!has_sim_ended(pd->sim_data))
	{
		take_fork(pd->sim_data, first_fork, pd->philo_nbr);
		if (pd->sim_data->amnt_philos == 1)
		{
			lay_down_fork(pd->sim_data, first_fork, pd->philo_nbr);
			return (-1);
		}
	}
	else
		return (-1);
	if (!has_sim_ended(pd->sim_data))
		take_fork(pd->sim_data, second_fork, pd->philo_nbr);
	else
	{
		lay_down_fork(pd->sim_data, first_fork, pd->philo_nbr);
		return (-1);
	}
	return (0);
}

//	function that initialize philosopher
void	init_philosopher(t_philo *pd, int *meals_eaten, int *first_fork,
			int *second_fork)
{
	struct timeval	cur_time;

	*meals_eaten = 0;
	start_thinking(pd->sim_data, pd->philo_nbr);
	pthread_mutex_lock(&pd->sim_data->last_eat_mut[pd->philo_nbr - 1]);
	gettimeofday(&cur_time, NULL);
	pd->sim_data->last_eat_arr[pd->philo_nbr -1] = cur_time;
	pthread_mutex_unlock(&pd->sim_data->last_eat_mut[pd->philo_nbr - 1]);
	*first_fork = LEFT_FORK;
	*second_fork = RIGHT_FORK;
	if (pd->philo_nbr % 2 == 0)
		stricter_sleep_milli(5);
}

//	function that will set the amount of meals a philosopher has eaten
void	set_meals_eaten(t_sim_data *sim_data, int *meals_eaten)
{
	(*meals_eaten)++;
	if (*meals_eaten == sim_data->min_meals)
	{
		pthread_mutex_lock(&sim_data->min_meal_mut);
		sim_data->min_meals_eaten++;
		if (sim_data->min_meals_eaten == sim_data->amnt_philos)
			set_sim_ended(sim_data);
		pthread_mutex_unlock(&sim_data->min_meal_mut);
	}
}

// this is the philosophers thread function that will be run.
void	*philo_func(void *data)
{
	t_philo			*pd;
	int				meals_eaten;
	int				first_fork;
	int				second_fork;

	pd = (t_philo *) data;
	init_philosopher(pd, &meals_eaten, &first_fork, &second_fork);
	while (!has_sim_ended(pd->sim_data))
	{
		if (take_forks(pd, first_fork, second_fork) == -1)
			break ;
		if (!has_sim_ended(pd->sim_data))
			start_eating(pd->sim_data, pd->philo_nbr);
		if (pd->sim_data->min_meals != -1)
			set_meals_eaten(pd->sim_data, &meals_eaten);
		lay_down_fork(pd->sim_data, LEFT_FORK, pd->philo_nbr);
		lay_down_fork(pd->sim_data, RIGHT_FORK, pd->philo_nbr);
		if (!has_sim_ended(pd->sim_data))
			start_sleeping(pd->sim_data, pd->philo_nbr);
		if (!has_sim_ended(pd->sim_data))
			start_thinking(pd->sim_data, pd->philo_nbr);
	}
	return (0);
}
