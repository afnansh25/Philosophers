/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashaheen <ashaheen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/25 17:38:49 by codespace         #+#    #+#             */
/*   Updated: 2025/11/01 19:35:45 by ashaheen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	get_last_meal(t_philo *p)
{
	long	t;

	pthread_mutex_lock(&p->phdata->state);
	t = p->last_meal;
	pthread_mutex_unlock(&p->phdata->state);
	return (t);
}

void	set_stop(t_data *d)
{
	pthread_mutex_lock(&d->stop_mutex);
	d->stop_sim = 1;
	pthread_mutex_unlock(&d->stop_mutex);
}

int	everyone_ate(t_data *d)
{
	int	i;
	int	done;
	int	count;

	if (d->eat_limit <= 0)
		return (0);
	i = 0;
	done = 0;
	while (i < d->num_philo)
	{
		pthread_mutex_lock(&d->state);
		count = d->philo[i].meals_count;
		pthread_mutex_unlock(&d->state);
		if (count >= d->eat_limit)
			done++;
		i++;
	}
	return (done == d->num_philo);
}

static int	handle_death(t_data *d, int i)
{
	long	now;

	now = now_ms();
	if (now - get_last_meal(&d->philo[i]) <= d->time_to_die)
		return (0);
	pthread_mutex_lock(&d->print);
	if (!get_stop(d))
	{
		set_stop(d);
		printf("%ld %d died\n", since_ms(d->start_time), d->philo[i].id);
	}
	pthread_mutex_unlock(&d->print);
	return (1);
}

void	*monitor_routine(void *arg)
{
	t_data	*d;
	int		i;
	int		stop;

	d = (t_data *)arg;
	stop = 0;
	while (!get_stop(d) && !stop)
	{
		i = 0;
		while (i < d->num_philo && !stop)
		{
			if (handle_death(d, i))
				stop = 1;
			i++;
		}
		if (!stop && everyone_ate(d))
		{
			set_stop(d);
			stop = 1;
		}
		usleep(500);
	}
	return (NULL);
}
