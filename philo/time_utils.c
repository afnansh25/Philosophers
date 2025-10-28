/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/25 17:38:39 by codespace         #+#    #+#             */
/*   Updated: 2025/10/27 17:29:31 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	now_ms(void)
{
    struct timeval	tv;
	long			ms;

	gettimeofday(&tv, NULL);
	ms = tv.tv_sec * 1000L + tv.tv_usec / 1000L;
	return (ms);
}

long	since_ms(long start)
{
	return (now_ms() - start);
}

void	ms_sleep(long ms, t_data *d)
{
	long	end;

	end = now_ms() + ms;
	while (now_ms() < end)
	{
		pthread_mutex_lock(&d->stop_mutex);
		if (d->stop_sim)
		{
			pthread_mutex_unlock(&d->stop_mutex);
			return ;
		}
		pthread_mutex_unlock(&d->stop_mutex);
		usleep(200); /* 0.2 ms chunks: responsive */
	}
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