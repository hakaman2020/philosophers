/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   validation.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: hman <marvin@codam.nl>                       +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/15 14:06:33 by hman          #+#    #+#                 */
/*   Updated: 2022/07/04 19:15:26 by hakaman       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

//	function will convert string into an integer
//	as long it is a positive non-zero number and not above INT_MAX
// 	it will return -1 otherwise.
//	RETURN	if conversion is successfull it will return an integer
//			else return -1
static int	convert_to_number(char *input)
{
	long	sum;

	sum = 0;
	while (*input == ' ')
		input++;
	while (*input)
	{
		if (*input < '0' || *input > '9')
			return (-1);
		sum = (sum * 10) + (*input - 48);
		input++;
	}
	if (sum > INT_MAX || sum == 0)
		return (-1);
	return ((int) sum);
}

//	fucntion will set the simulation data according to the arguments provided
//	RETURN	1 when the arguments valid
//			-1 if one of the arguments is not valid.
static int	setup_sim_data(int argc, char **argv, t_sim_data *sim_data)
{
	int	numbers[5];
	int	i;

	i = 0;
	while (i < 4)
	{
		numbers[i] = convert_to_number(argv[i + 1]);
		if (numbers[i] == -1)
			return (-1);
		i++;
	}	
	sim_data->amnt_philos = numbers[0];
	sim_data->time_to_die = numbers[1];
	sim_data->time_to_eat = numbers[2];
	sim_data->time_to_sleep = numbers[3];
	sim_data->min_meals = -1;
	if (argc == 6)
	{
		numbers[4] = convert_to_number(argv[5]);
		if (numbers[4] == -1)
			return (-1);
		sim_data->min_meals = numbers[4];
	}
	return (1);
}

/*
**	function will check if the input is valid
**	RETURN	-1 if the input is invalid
**			1 if the input is valid
*/			
int	process_input(int argc, char **argv, t_sim_data *sim_data)
{
	if (argc < 5 || argc > 6)
	{
		printf("Usage: ./philo [number of philosophers] [time to die in msec]");
		printf(" [time to eat in msec] [time to sleep in msec]");
		printf(" [opt: number of times a philosopher must eat]\n");
		return (-1);
	}
	if (setup_sim_data(argc, argv, sim_data) == -1)
	{
		write(2, "Error: Invalid input.\n", 22);
		return (-1);
	}
	return (1);
}
