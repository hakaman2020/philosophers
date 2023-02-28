/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   release_res.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: hman <hman@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/05/31 17:20:59 by hman          #+#    #+#                 */
/*   Updated: 2022/06/28 17:42:01 by hman          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

//	this function will free the initiated mutexes and free the malloced array
//	array is the to be free mutex array and i is the last active mutex in the 
//	array.
void	release_mutex_array(pthread_mutex_t *mutex_array, int i)
{
	int	j;

	j = 0;
	while (j < i)
	{
		pthread_mutex_destroy(&mutex_array[j]);
		j++;
	}
	free(mutex_array);
}

//	function will free the resources used by the sim_data structure
//	depending on the level it will resources till a certain "level"
//	level : 1	start sim mutex
//			2	end sim mutex
//			3	fork mutex array
//			4	last eaten int array
//			5	last eaten mutex array 
//			6	minimum meals mutex
//	the return number is being returned to save some lines in the
//	setup_simulation function.
int	clean_sim_data(t_sim_data *sim_data, int level, int return_number)
{
	if (level >= 1)
		pthread_mutex_destroy(&sim_data->print_mut);
	if (level >= 2)
		pthread_mutex_destroy(&sim_data->end_mut);
	if (level >= 3)
		release_mutex_array(sim_data->forks_mut, sim_data->amnt_philos);
	if (level >= 4)
		free(sim_data->last_eat_arr);
	if (level >= 5)
		release_mutex_array(sim_data->last_eat_mut, sim_data->amnt_philos);
	if (level >= 6)
		pthread_mutex_destroy(&sim_data->min_meal_mut);
	return (return_number);
}

// function to display error message if the simulation setup fails
int	display_err_msg(void)
{
	write(2, "Error: couldn't setup simulation.\n", 34);
	return (1);
}
