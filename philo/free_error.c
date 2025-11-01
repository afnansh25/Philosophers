/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_error.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashaheen <ashaheen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/25 17:38:25 by codespace         #+#    #+#             */
/*   Updated: 2025/11/01 19:29:01 by ashaheen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	malloc_failed(t_data *d)
{
	cleanup_allocs(d);
	return (1);
}

void	cleanup_allocs(t_data *d)
{
	if (!d)
		return ;
	if (d->f_owner)
	{
		free(d->f_owner);
		d->f_owner = NULL;
	}
	if (d->forks_st)
	{
		free(d->forks_st);
		d->forks_st = NULL;
	}
	if (d->philo)
	{
		free(d->philo);
		d->philo = NULL;
	}
	if (d->forks)
	{
		free(d->forks);
		d->forks = NULL;
	}
}

void	cleanup_all(t_data *d)
{
	int	i;

	if (!d)
		return ;
	if (d->forks)
	{
		i = 0;
		while (i < d->num_philo)
		{
			pthread_mutex_destroy(&d->forks[i]);
			i++;
		}
	}
	pthread_mutex_destroy(&d->stop_mutex);
	pthread_mutex_destroy(&d->waiter);
	pthread_mutex_destroy(&d->state);
	pthread_mutex_destroy(&d->print);
	cleanup_allocs(d);
}
