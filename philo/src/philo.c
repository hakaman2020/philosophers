/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philo.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: hman <marvin@codam.nl>                       +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/15 13:12:19 by hman          #+#    #+#                 */
/*   Updated: 2022/07/04 19:13:31 by hman          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// function the print the message dependant on the message type
void	print_message(t_sim_data *sim_data, int message_type, int philo_nbr)
{
	static const char	*messages[] = {"has taken a fork", "is eating",
		"is sleeping", "is thinking", "died"};
	unsigned int		time_diff;

	pthread_mutex_lock(&sim_data->print_mut);
	time_diff = time_passed(sim_data->start_time);
	if (!has_sim_ended(sim_data) || message_type == MSG_DIED)
		printf("%u %d %s\n", time_diff, philo_nbr, messages[message_type]);
	pthread_mutex_unlock(&sim_data->print_mut);
}

//	function that checks if the simulation has ended
//	if end_sim value = 1 the simulation has ended and it has not ended if it has
//	the value 0.
int	has_sim_ended(t_sim_data *sim_data)
{
	int	end_sim_value;

	pthread_mutex_lock(&sim_data->end_mut);
	end_sim_value = sim_data->end_sim;
	pthread_mutex_unlock(&sim_data->end_mut);
	return (end_sim_value);
}

//	function that will set the end_sim variable to 1 to signify simulation
//	should end
void	set_sim_ended(t_sim_data *sim_data)
{
	pthread_mutex_lock(&sim_data->end_mut);
	sim_data->end_sim = 1;
	pthread_mutex_unlock(&sim_data->end_mut);
}

//	function to monitor if any philosopher has died due to starvation
void	monitoring(t_sim_data *sim_data)
{
	int				i;
	struct timeval	cur_time;
	unsigned int	time_diff;

	while (!has_sim_ended(sim_data))
	{
		i = 0;
		while (i < sim_data->amnt_philos && !has_sim_ended(sim_data))
		{
			pthread_mutex_lock(&sim_data->last_eat_mut[i]);
			gettimeofday(&cur_time, NULL);
			time_diff = calc_time_diff_milli(sim_data->last_eat_arr[i],
					cur_time);
			pthread_mutex_unlock(&sim_data->last_eat_mut[i]);
			if (time_diff >= (unsigned int) sim_data->time_to_die)
			{
				set_sim_ended(sim_data);
				print_message(sim_data, MSG_DIED, i + 1);
			}
			i++;
		}
		usleep(1000);
	}
}

int	main(int argc, char *argv[])
{
	t_sim_data	sim_data;
	t_philo		*philos;
	pthread_t	*t_ids;
	int			i;

	if (process_input(argc, argv, &sim_data) == ERROR)
		return (1);
	if (setup_simulation(&sim_data) == ERROR)
		return (display_err_msg());
	if (setup_sim2(&sim_data, &philos) == ERROR)
		return (display_err_msg());
	if (start_threads(&sim_data, philos, &t_ids) == ERROR)
		return (display_err_msg());
	monitoring(&sim_data);
	i = 0;
	while (i < sim_data.amnt_philos)
	{
		pthread_join(t_ids[i], NULL);
		i++;
	}
	clean_sim_data(&sim_data, 6, 0);
	free(t_ids);
	free(philos);
	return (0);
}
