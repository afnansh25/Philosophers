/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/25 17:38:35 by codespace         #+#    #+#             */
/*   Updated: 2025/10/27 17:31:34 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	get_lock_order(int l, int r, int *f1, int *f2)
{
	if (l < r)
	{
		*f1 = l;
		*f2 = r;
	}
	else
	{
		*f1 = r;
		*f2 = l;
	}
}

static int	take_two_if_allowed(t_philo *p)
{
	t_data			*d;
	int				l;
	int				r;
	int				f1;
	int				f2;

	d = p->phdata;
	l = p->l_fork;
	r = p->r_fork;
	get_lock_order(l, r, &f1, &f2);
	pthread_mutex_lock(&d->forks[f1]);
	pthread_mutex_lock(&d->forks[f2]);
	if (d->forks_st[l] == (char)p->id || d->forks_st[r] == (char)p->id)
	{
		pthread_mutex_unlock(&d->forks[f2]);
		pthread_mutex_unlock(&d->forks[f1]);
		return (0);
	}
	log_state(p, "has taken a fork");
	log_state(p, "has taken a fork");
	return (1);
}

static void	release_two(t_philo *p)
{
	t_data	*d;
	int		l;
	int		r;
	int		f1;
	int		f2;

	d = p->phdata;
	l = p->l_fork;
	r = p->r_fork;
	get_lock_order(l, r, &f1, &f2);
	pthread_mutex_unlock(&d->forks[f2]);
	pthread_mutex_unlock(&d->forks[f1]);
}

static void	step_once(t_philo *p)
{
	t_data	*d;

	d = p->phdata;
	if (take_two_if_allowed(p))
	{
		p->last_meal = now_ms();
		log_state(p, "is eating");
		ms_sleep(d->time_to_eat, d);
		p->meals_count = p->meals_count + 1;
		release_two(p);
		if (!get_stop(d))
		{
			log_state(p, "is sleeping");
			ms_sleep(d->time_to_sleep, d);
			log_state(p, "is thinking");
		}
	}
	else
		usleep(200);
}

void	*philo_routine(void *arg)
{
	t_philo	*p;
	t_data	*d;

	p = (t_philo *)arg;
	d = p->phdata;
	if (d->num_philo == 1)
	{
		pthread_mutex_lock(&d->forks[p->l_fork]);
		log_state(p, "has taken a fork");
		ms_sleep(d->time_to_die, d);
		pthread_mutex_unlock(&d->forks[p->l_fork]);
		return (NULL);
	}
	if (p->id % 2 == 0)
		usleep(200);
	while (!get_stop(d))
		step_once(p);
	return (NULL);
}

