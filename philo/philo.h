/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/25 17:18:34 by codespace         #+#    #+#             */
/*   Updated: 2025/10/26 10:15:57 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <limits.h>
# include <stdint.h>
# include <sys/time.h>

typedef struct s_data	t_data;

typedef	struct	s_philo
{
	pthread_t		thrd;
	int				id;
	int		 		meals_count;
	int         	last_meal;
	int				is_thinking;
	// pthread_mutex_t	*l_fork;
	// pthread_mutex_t	*r_fork;
	int				l_fork;
	int				r_fork;
	t_data		*phdata;
}t_philo;

typedef struct	s_data
{	
	int     		num_philo;
	int     		time_to_die;
	int     		time_to_eat;
	int     		time_to_sleep;
	int     		eat_limit;
	long			start_time;
	int				stop_sim;
	char			*forks_st;
	int				*f_owner;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print;
	pthread_mutex_t state;
	pthread_mutex_t waiter;
	pthread_mutex_t stop_mutex;
	pthread_t 		monit;
	t_philo 		*philo;
}t_data;


//free
int		malloc_failed(t_data *d);
void	cleanup_allocs(t_data *d);
void	cleanup_all(t_data *d);

//parse
int		parse_args(int ac, char **av, t_data *data);

//init
int		init_data(t_data *d);

//routine
int		start_routine(t_data *d);
void	*philo_routine(void *arg);
void	*monitor_routine(void *arg);

//monitor
int	get_stop(t_data *d);

//time_utils
long	now_ms(void);
long	since_ms(long start);
void	ms_sleep(long ms, t_data *d);


#endif