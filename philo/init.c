/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashaheen <ashaheen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/25 17:38:19 by codespace         #+#    #+#             */
/*   Updated: 2025/11/01 20:04:51 by ashaheen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	init_fork_index(t_data *d)
{
	int	i;
	int	n;
	int	val;

	n = d->num_philo;
	i = 0;
	while (i < n)
	{
		if (i == 0 || i == n - 1)
			val = n;
		else if (i % 2 == 1)
			val = i + 1;
		else
			val = i + 2;
		if (val > n)
			val = ((val - 1) % n) + 1;
		//d->f_owner[i] = -1;
		d->forks_st[i] = val;
		i++;
	}
}

static int	init_global_mutexes(t_data *d)
{
	if (pthread_mutex_init(&d->print, NULL))
		return (1);
	if (pthread_mutex_init(&d->state, NULL))
	{
		pthread_mutex_destroy(&d->print);
		return (1);
	}
	if (pthread_mutex_init(&d->stop_mutex, NULL))
	{
		pthread_mutex_destroy(&d->state);
		pthread_mutex_destroy(&d->print);
		return (1);
	}
	return (0);
}

static int	init_fork_mutexes(t_data *d)
{
	int	i;

	i = 0;
	while (i < d->num_philo)
	{
		if (pthread_mutex_init(&d->forks[i], NULL))
		{
			while (--i >= 0)
				pthread_mutex_destroy(&d->forks[i]);
			return (1);
		}
		i++;
	}
	return (0);
}

static void	init_philos(t_data *d)
{
	int	i;
	int	n;

	i = 0;
	n = d->num_philo;
	while (i < n)
	{
		d->philo[i].id = i + 1;
		d->philo[i].meals_count = 0;
		d->philo[i].last_meal = 0;
		d->philo[i].l_fork = i;
		d->philo[i].r_fork = (i + 1) % n;
		d->philo[i].phdata = d;
		i++;
	}
}

int	init_data(t_data *d)
{
	d->start_time = 0;
	d->stop_sim = 0;
	d->philo = NULL;
	d->forks = NULL;
	d->forks_st = NULL;
	//d->f_owner = NULL;
	d->forks = malloc(d->num_philo * sizeof(pthread_mutex_t));
	if (!d->forks)
		return (malloc_failed(d));
	d->philo = malloc(d->num_philo * sizeof(t_philo));
	if (!d->philo)
		return (malloc_failed(d));
	d->forks_st = malloc(d->num_philo * sizeof(int));
	if (!d->forks_st)
		return (malloc_failed(d));
	// d->f_owner = malloc(d->num_philo * sizeof(int));
	// if (!d->f_owner)
	// 	return (malloc_failed(d));
	init_fork_index(d);
	if (init_global_mutexes(d))
		return (malloc_failed(d));
	if (init_fork_mutexes(d))
		return (malloc_failed(d));
	init_philos(d);
	return (0);
}
