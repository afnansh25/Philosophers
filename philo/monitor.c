/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/25 17:38:49 by codespace         #+#    #+#             */
/*   Updated: 2025/10/27 17:24:10 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	get_stop(t_data *d)
{
	int	val;

	pthread_mutex_lock(&d->stop_mutex);
	val = d->stop_sim;
	pthread_mutex_unlock(&d->stop_mutex);
	return (val);
}

static void	set_stop(t_data *d)
{
	pthread_mutex_lock(&d->stop_mutex);
	d->stop_sim = 1;
	pthread_mutex_unlock(&d->stop_mutex);
}

static int	everyone_ate(t_data *d)
{
	int	i;
	int	done;

	if (d->eat_limit <= 0)
		return (0);
	i = 0;
	done = 0;
	while (i < d->num_philo)
	{
		if (d->philo[i].meals_count >= d->eat_limit)
			done++;
		i++;
	}
	return (done == d->num_philo);
}

static int	check_death(t_data *d, int i, long now)
{
	if (now - d->philo[i].last_meal < d->time_to_die)
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
	long	now;

	d = (t_data *)arg;
	while (!get_stop(d))
	{
		i = 0;
		while (i < d->num_philo)
		{
			now = now_ms();
			if (check_death(d, i, now))
				return (NULL);
			i++;
		}
		if (everyone_ate(d))
			return (set_stop(d), NULL);
		usleep(1000);
	}
	return (NULL);
}


