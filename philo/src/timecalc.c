/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   timecalc.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: hakaman <hakaman@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/06/14 11:36:11 by hakaman       #+#    #+#                 */
/*   Updated: 2022/07/01 18:14:59 by hman          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

//	function that calculates the difference in milliseconds
unsigned int	calc_time_diff_milli(struct timeval tv, struct timeval tv2)
{
	long	time_diff;

	time_diff = calc_time_diff_usec(tv, tv2) / 1000;
	return ((unsigned int) time_diff);
}

// function that calculates the difference in microseconds
long	calc_time_diff_usec(struct timeval tv, struct timeval tv2)
{
	long	time_diff;

	time_diff = ((tv2.tv_sec - tv.tv_sec) * 1000000)
		+ (tv2.tv_usec - tv.tv_usec);
	return (time_diff);
}

//	since usleep is not guaranteed to sleep the given value, this function
//	attempts to get closer to the given value. 
//	note usleep(100) for some reason is too low and make the the function
//	sleep a shorter time then intended.
void	stricter_sleep_milli(unsigned int ms)
{
	struct timeval	start_time;
	struct timeval	cur_time;

	gettimeofday(&start_time, NULL);
	cur_time = start_time;
	while (calc_time_diff_usec(start_time, cur_time) < ms * 1000)
	{
		usleep(200);
		gettimeofday(&cur_time, NULL);
	}
}

// calculate the time difference between the start time and current time
unsigned int	time_passed(struct timeval start)
{
	struct timeval	cur_time;
	unsigned int	time_diff;

	gettimeofday(&cur_time, NULL);
	time_diff = calc_time_diff_milli(start, cur_time);
	return (time_diff);
}
