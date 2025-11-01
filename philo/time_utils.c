/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashaheen <ashaheen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/25 17:38:39 by codespace         #+#    #+#             */
/*   Updated: 2025/11/01 19:55:21 by ashaheen         ###   ########.fr       */
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

int	get_stop(t_data *d)
{
	int	val;

	pthread_mutex_lock(&d->stop_mutex);
	val = d->stop_sim;
	pthread_mutex_unlock(&d->stop_mutex);
	return (val);
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
		usleep(200);
	}
}
