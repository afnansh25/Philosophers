/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashaheen <ashaheen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/01 19:41:51 by ashaheen          #+#    #+#             */
/*   Updated: 2025/11/01 20:00:15 by ashaheen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	set_last_meal(t_philo *p, long t)
{
	pthread_mutex_lock(&p->phdata->state);
	p->last_meal = t;
	pthread_mutex_unlock(&p->phdata->state);
}

static int	join_threads(t_data *d, int code)
{
	int	i;

	i = 0;
	while (i < d->num_philo)
	{
		pthread_join(d->philo[i].thrd, NULL);
		i++;
	}
	return (code);
}

void	log_state(t_philo *p, const char *msg)
{
	t_data	*d;

	d = p->phdata;
	pthread_mutex_lock(&d->print);
	if (!get_stop(d))
		printf("%ld %d %s\n", since_ms(d->start_time), p->id, msg);
	pthread_mutex_unlock(&d->print);
}

int	start_routine(t_data *d)
{
	int	i;

	d->start_time = now_ms();
	i = 0;
	while (i < d->num_philo)
		d->philo[i++].last_meal = d->start_time;
	i = 0;
	while (i < d->num_philo)
	{
		if (pthread_create(&d->philo[i].thrd, NULL,
				philo_routine, &d->philo[i]))
			return (join_threads(d, 1));
		i++;
	}
	if (pthread_create(&d->monit, NULL, monitor_routine, d))
		return (join_threads(d, 1));
	pthread_join(d->monit, NULL);
	return (join_threads(d, 0));
}
