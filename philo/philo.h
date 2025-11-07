/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashaheen <ashaheen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/25 17:18:34 by codespace         #+#    #+#             */
/*   Updated: 2025/11/01 20:19:13 by ashaheen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <limits.h>
# include <pthread.h>
# include <stdint.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_data	t_data;

typedef struct s_philo
{
	pthread_t			thrd;
	int					id;
	int					meals_count;
	long				last_meal;
	int					is_thinking;
	int					l_fork;
	int					r_fork;
	t_data				*phdata;
}						t_philo;

typedef struct s_data
{
	int					num_philo;
	int					time_to_die;
	int					time_to_eat;
	int					time_to_sleep;
	int					eat_limit;
	long				start_time;
	int					stop_sim;
	int					*forks_st;
	int					*f_owner;
	pthread_mutex_t		*forks;
	pthread_mutex_t		print;
	pthread_mutex_t		state;
	pthread_mutex_t		waiter;
	pthread_mutex_t		stop_mutex;
	pthread_t			monit;
	t_philo				*philo;
}						t_data;

// free
int						malloc_failed(t_data *d);
void					cleanup_allocs(t_data *d);
void					cleanup_all(t_data *d);

// parse
int						parse_args(int ac, char **av, t_data *data);

// init
int						init_data(t_data *d);

/* start */
int						start_routine(t_data *d);
void					log_state(t_philo *p, const char *msg);

/* routine */
void					set_last_meal(t_philo *p, long t);
int						take_two_if_allowed(t_philo *p);
void					release_two(t_philo *p);
void					*philo_routine(void *arg);

// monitor
void					*monitor_routine(void *arg);
int						get_stop(t_data *d);
long					get_last_meal(t_philo *p);

// time_utils
long					now_ms(void);
long					since_ms(long start);
void					ms_sleep(long ms, t_data *d);

#endif