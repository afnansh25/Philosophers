/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/25 17:38:35 by codespace         #+#    #+#             */
/*   Updated: 2025/10/27 17:08:30 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	log_state(t_philo *p, const char *msg)
{
	t_data	*d;

	d = p->phdata;
	pthread_mutex_lock(&d->print);
	if (!get_stop(d))
		printf("%ld %d %s\n", since_ms(d->start_time), p->id, msg);
	pthread_mutex_unlock(&d->print);
}

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

int	start_routine(t_data *d)
{
	int	i;

	d->start_time = now_ms();
	i = 0;
	while (i < d->num_philo)
	{
		d->philo[i].last_meal = d->start_time;
		i++;
	}
	i = 0;
	while (i < d->num_philo)
	{
		if (d->philo[i].id % 2 == 0)
			usleep(200);
		if (pthread_create(&d->philo[i].thrd, NULL, philo_routine, &d->philo[i]))
		{
			while (--i >= 0)
				pthread_join(d->philo[i].thrd, NULL);
			return (1);
		}
		i++;
	}
	if (pthread_create(&d->monit, NULL, monitor_routine, d))
	{
		i = 0;
		while (i < d->num_philo)
		{
			pthread_join(d->philo[i].thrd, NULL);
			i++;
		}
		return (1);
	}
	pthread_join(d->monit, NULL);
	i = 0;
	while (i < d->num_philo)
	{
		pthread_join(d->philo[i].thrd, NULL);
		i++;
	}
	return (0);
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
	{
		if (take_two_if_allowed(p))
		{
			p->last_meal = now_ms();
			log_state(p, "is eating");
			ms_sleep(d->time_to_eat, d);
			p->meals_count += 1;
			d->forks_st[p->l_fork] = (char)p->id;
			d->forks_st[p->r_fork] = (char)p->id;
			release_two(p);
			if (get_stop(d))
				break ;
			log_state(p, "is sleeping");
			ms_sleep(d->time_to_sleep, d);
			log_state(p, "is thinking");
		}
		else
		{
			usleep(200);
		}
	}
	return (NULL);
}
