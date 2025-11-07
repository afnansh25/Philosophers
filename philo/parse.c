/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashaheen <ashaheen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/25 17:38:30 by codespace         #+#    #+#             */
/*   Updated: 2025/11/01 19:36:14 by ashaheen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static long	ft_atoi_long(const char *s)
{
	long	val;
	int		i;

	if (!s)
		return (0);
	val = 0;
	i = 0;
	if (s[i] == '+')
		i++;
	while (s[i] >= '0' && s[i] <= '9')
	{
		val = val * 10 + (s[i] - '0');
		if (val > INT_MAX || val <= 0)
			return (-1);
		i++;
	}
	return (val);
}

static int	is_num(const char *s)
{
	int	i;

	i = 0;
	if (!s || !s[0])
		return (0);
	if (s[i] == '+')
		i++;
	while (s[i])
	{
		if (s[i] < '0' || s[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

static void	assign_args(int ac, char **av, t_data *d)
{
	d->num_philo = (int)ft_atoi_long(av[1]);
	d->time_to_die = (int)ft_atoi_long(av[2]);
	d->time_to_eat = (int)ft_atoi_long(av[3]);
	d->time_to_sleep = (int)ft_atoi_long(av[4]);
	d->eat_limit = -1;
	if (ac == 6)
		d->eat_limit = (int)ft_atoi_long(av[5]);
}

static int	arg_err(const char *msg, int idx)
{
	if (idx > 0)
		printf("%s %d\n", msg, idx);
	else
		printf("%s\n", msg);
	return (1);
}

int	parse_args(int ac, char **av, t_data *data)
{
	int		i;
	long	val;

	if (!data)
		return (arg_err("Error: null data pointer", 0));
	i = 1;
	while (i < ac)
	{
		if (!is_num(av[i]))
			return (arg_err("Error: argument not a valid number -> index", i));
		val = ft_atoi_long(av[i]);
		if (val <= 0 || val > INT_MAX)
			return (arg_err("Error: argument out of range -> index", i));
		i++;
	}
	assign_args(ac, av, data);
	if (data->num_philo > 200)
		return (arg_err("Error: num_philo too big-> value", data->num_philo));
	return (0);
}
