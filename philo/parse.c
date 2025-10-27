/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/25 17:38:30 by codespace         #+#    #+#             */
/*   Updated: 2025/10/26 07:24:48 by codespace        ###   ########.fr       */
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

int	parse_args(int ac, char **av, t_data *data)
{
    int     i;
    long	value;

    if (!data)
    {
        printf("Error: null data pointer\n");
        return (1);
    }
    i = 1;
    while (i < ac)
    {
        if (!is_num(av[i]))
        {
            printf("Error: argument %d is not a valid number\n", i);
            return (1);
        }
        value = ft_atoi_long(av[i]);
        if (value <= 0 || value > INT_MAX)
        {
            printf("Error: argument %d out of range\n", i);
            return (1);
        }
		i++;
    }
    assign_args(ac, av, data);
	if (data->num_philo > 200)
	{
		printf("Error: num_philo too large\n");
		return (1);
	}
	return (0);
}
