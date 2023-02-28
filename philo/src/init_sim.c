/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   init_sim.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: hman <hman@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/06/09 17:31:31 by hman          #+#    #+#                 */
/*   Updated: 2022/07/08 16:15:50 by hman          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

//	function that will create fill the given array with mutexes
//	if something goes wrong it will release the build mutexes and also
//	free the malloced array.
//	RETURN	0 if it succesfully filled the array
//			-1 if something went wrong
void	fill_last_eat_arr(t_sim_data *sim_data)
{
	struct timeval	cur_time;
	int				i;

	gettimeofday(&cur_time, NULL);
	i = 0;
	while (i < sim_data->amnt_philos)
	{
		sim_data->last_eat_arr[i] = cur_time;
		i++;
	}
}

//	function that will fill the given array with mutexes
int	create_mutex_array(pthread_mutex_t *mutex_array, int amount)
{
	int	i;

	i = 0;
	while (i < amount)
	{
		if (pthread_mutex_init(&mutex_array[i], NULL) != 0)
		{
			release_mutex_array(mutex_array, i);
			return (-1);
		}
		i++;
	}
	return (0);
}

//	function that will setup most of the simulation data
//	initialize variables and create the necessary mutexes
int	setup_simulation(t_sim_data *data)
{
	if (pthread_mutex_init(&data->print_mut, NULL) != 0)
		return (-1);
	data->end_sim = 0;
	if (pthread_mutex_init(&data->end_mut, NULL) != 0)
		return (clean_sim_data(data, 1, -1));
	data->forks_mut = malloc(sizeof(pthread_mutex_t)
			* data->amnt_philos);
	if (data->forks_mut == NULL)
		return (clean_sim_data(data, 2, -1));
	if (create_mutex_array(data->forks_mut, data->amnt_philos) != 0)
		return (clean_sim_data(data, 2, -1));
	data->last_eat_arr = malloc(sizeof(struct timeval) * data->amnt_philos);
	if (data->last_eat_arr == NULL)
		return (clean_sim_data(data, 3, -1));
	fill_last_eat_arr(data);
	data->last_eat_mut = malloc(sizeof(pthread_mutex_t)
			* data->amnt_philos);
	if (data->last_eat_mut == NULL)
		return (clean_sim_data(data, 4, -1));
	if (create_mutex_array(data->last_eat_mut, data->amnt_philos) != 0)
		return (clean_sim_data(data, 4, -1));
	data->min_meals_eaten = 0;
	if (pthread_mutex_init(&data->min_meal_mut, NULL) != 0)
		return (clean_sim_data(data, 5, -1));
	return (0);
}

//	function that setups more elements for the simulation
int	setup_sim2(t_sim_data *sim_data, t_philo **philos)
{
	int	i;

	gettimeofday(&sim_data->start_time, NULL);
	*philos = malloc(sizeof (t_philo) * sim_data->amnt_philos);
	if (*philos == NULL)
	{
		clean_sim_data(sim_data, 6, 0);
		return (-1);
	}
	i = 0;
	while (i < sim_data->amnt_philos)
	{
		(*philos)[i].philo_nbr = i + 1;
		(*philos)[i].sim_data = sim_data;
		i++;
	}
	return (0);
}

//	function that starts the threads of philosophers
int	start_threads(t_sim_data *sim_data, t_philo *philos, pthread_t **t_ids)
{
	int	i;

	i = 0;
	*t_ids = malloc(sizeof (pthread_t) * sim_data->amnt_philos);
	if (*t_ids == NULL)
	{
		free(philos);
		clean_sim_data(sim_data, 6, 0);
		return (-1);
	}
	while (i < sim_data->amnt_philos)
	{
		pthread_create(&(*t_ids)[i], NULL, philo_func, (void *) &philos[i]);
		i++;
	}
	return (0);
}
