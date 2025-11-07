/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashaheen <ashaheen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/25 17:38:35 by codespace         #+#    #+#             */
/*   Updated: 2025/11/01 20:00:08 by ashaheen         ###   ########.fr       */
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

int	take_two_if_allowed(t_philo *p)
{
	t_data	*d;
	int		f1;
	int		f2;

	d = p->phdata;
	get_lock_order(p->l_fork, p->r_fork, &f1, &f2);
	pthread_mutex_lock(&d->forks[f1]);
	pthread_mutex_lock(&d->forks[f2]);
	if (d->forks_st[p->l_fork] == p->id
		|| d->forks_st[p->r_fork] == p->id)
	{
		pthread_mutex_unlock(&d->forks[f2]);
		pthread_mutex_unlock(&d->forks[f1]);
		return (0);
	}
	log_state(p, "has taken a fork");
	log_state(p, "has taken a fork");
	return (1);
}

void	release_two(t_philo *p)
{
	t_data	*d;
	int		l;
	int		r;
	int		f1;
	int		f2;

	d = p->phdata;
	l = p->l_fork;
	r = p->r_fork;
	if (l < r)
	{
		f1 = l;
		f2 = r;
	}
	else
	{
		f1 = r;
		f2 = l;
	}
	pthread_mutex_unlock(&d->forks[f2]);
	pthread_mutex_unlock(&d->forks[f1]);
}

static int	ph_cycle(t_philo *p)
{
	t_data	*d;

	d = p->phdata;
	if (take_two_if_allowed(p))
	{
		set_last_meal(p, now_ms());
		log_state(p, "is eating");
		ms_sleep(d->time_to_eat, d);
		p->meals_count += 1;
		d->forks_st[p->l_fork] = p->id;
		d->forks_st[p->r_fork] = p->id;
		release_two(p);
		if (!get_stop(d))
		{
			log_state(p, "is sleeping");
			ms_sleep(d->time_to_sleep, d);
			log_state(p, "is thinking");
			if (d->time_to_eat > d->time_to_sleep)
				usleep((d->time_to_eat - d->time_to_sleep) * 500);
		}
		return (1);
	}
	usleep(200);
	return (0);
}

void	*philo_routine(void *arg)
{
	t_philo	*p;
	t_data	*d;
	int		done;

	p = (t_philo *)arg;
	d = p->phdata;
	done = 0;
	if (d->num_philo == 1)
	{
		pthread_mutex_lock(&d->forks[p->l_fork]);
		log_state(p, "has taken a fork");
		ms_sleep(d->time_to_die, d);
		pthread_mutex_unlock(&d->forks[p->l_fork]);
		done = 1;
	}
	if (!done && (p->id % 2 == 0))
		usleep(200);
	while (!done && !get_stop(d))
		ph_cycle(p);
	return (NULL);
}
