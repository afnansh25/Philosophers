/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 17:18:26 by codespace         #+#    #+#             */
/*   Updated: 2025/10/27 17:18:41 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	seed_last_meal(t_data *d)
{
	int	i;

	i = 0;
	while (i < d->num_philo)
	{
		d->philo[i].last_meal = d->start_time;
		i++;
	}
}

static int	create_philo_threads(t_data *d)
{
	int	i;

	i = 0;
	while (i < d->num_philo)
	{
		if (d->philo[i].id % 2 == 0)
			usleep(200);
		if (pthread_create(&d->philo[i].thrd, NULL,
				philo_routine, &d->philo[i]))
		{
			while (--i >= 0)
				pthread_join(d->philo[i].thrd, NULL);
			return (1);
		}
		i++;
	}
	return (0);
}

static int	create_monitor_thread(t_data *d)
{
	if (pthread_create(&d->monit, NULL, monitor_routine, d))
		return (1);
	return (0);
}

static void	join_all(t_data *d)
{
	int	i;

	pthread_join(d->monit, NULL);
	i = 0;
	while (i < d->num_philo)
	{
		pthread_join(d->philo[i].thrd, NULL);
		i++;
	}
}

int	start_routine(t_data *d)
{
	int	fail;

	d->start_time = now_ms();
	seed_last_meal(d);
	fail = create_philo_threads(d);
	if (fail)
		return (1);
	fail = create_monitor_thread(d);
	if (fail)
	{
		fail = 0;
		while (fail < d->num_philo)
		{
			pthread_join(d->philo[fail].thrd, NULL);
			fail++;
		}
		return (1);
	}
	join_all(d);
	return (0);
}

