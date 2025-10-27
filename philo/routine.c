/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/25 17:38:35 by codespace         #+#    #+#             */
/*   Updated: 2025/10/27 16:51:29 by codespace        ###   ########.fr       */
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

