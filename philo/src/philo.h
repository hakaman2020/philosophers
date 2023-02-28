/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philo.h                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: hman <marvin@codam.nl>                       +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/15 16:06:57 by hman          #+#    #+#                 */
/*   Updated: 2022/07/01 17:48:58 by hman          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <limits.h>
# include <sys/time.h>

//----------------------------------------------
// Defines
//----------------------------------------------
//defines for readability
# define ERROR -1
# define LEFT_FORK 0
# define RIGHT_FORK 1
# define MSG_FORK 0
# define MSG_EAT 1
# define MSG_SLEEP 2
# define MSG_THINKING 3
# define MSG_DIED 4

//----------------------------------------------
// Data structures
//----------------------------------------------
//	datastructure that hold the simulation data
typedef struct s_sim_data
{
	int				amnt_philos;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				min_meals;
	int				min_meals_eaten;
	int				end_sim;
	struct timeval	*last_eat_arr;
	pthread_mutex_t	min_meal_mut;
	pthread_mutex_t	print_mut;
	pthread_mutex_t	end_mut;
	pthread_mutex_t	*forks_mut;
	pthread_mutex_t	*last_eat_mut;
	struct timeval	start_time;
}				t_sim_data;

//	data structure that represent a philosopher
typedef struct s_philo
{
	int			philo_nbr;
	t_sim_data	*sim_data;
}				t_philo;

//----------------------------------------------
// Functions
//----------------------------------------------
//	validation of input
int				process_input(int argc, char **argv, t_sim_data *sim_data);

//	initialize the simulation variables and mutexes
int				setup_simulation(t_sim_data *data);
int				setup_sim2(t_sim_data *sim_data, t_philo **philos);
int				start_threads(t_sim_data *sim_data, t_philo *philo,
					pthread_t **t_ids);
//	time functions
unsigned int	calc_time_diff_milli(struct timeval tv, struct timeval tv2);
long			calc_time_diff_usec(struct timeval tv, struct timeval tv2);
unsigned int	time_passed(struct timeval start);
void			stricter_sleep_milli(unsigned int ms);

//	release resources
void			release_mutex_array(pthread_mutex_t *array, int i);
int				clean_sim_data(t_sim_data *sim_data, int level,
					int return_number);
int				display_err_msg(void);

//	display status function
void			print_message(t_sim_data *sim_data, int message_type,
					int philo_nbr);

//	simulation functions
int				has_sim_ended(t_sim_data *sim_data);
void			set_sim_ended(t_sim_data *sim_data);

//	main philosopher thread function
void			*philo_func(void *data);

//	philosopher action functions
void			start_thinking(t_sim_data *sim_data, int philo_nbr);
void			start_eating(t_sim_data *sim_data, int philo_nbr);
void			lay_down_fork(t_sim_data *sim_data, int fork_type,
					int philo_nbr);
void			take_fork(t_sim_data *sim_data, int fork_type, int philo_nbr);
void			start_sleeping(t_sim_data *sim_data, int philo_nbr);
#endif
