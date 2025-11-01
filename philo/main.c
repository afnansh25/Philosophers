/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashaheen <ashaheen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/25 17:38:13 by codespace         #+#    #+#             */
/*   Updated: 2025/11/01 19:32:07 by ashaheen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int ac, char **av)
{
	t_data	phdata;
	int		err;

	err = 0;
	if (ac != 5 && ac != 6)
	{
		printf("Usage: ./philo num_philo time_to_die "
			"time_to_eat time_to_sleep [eat_limit]\n");
		return (1);
	}
	if (parse_args(ac, av, &phdata))
		return (1);
	if (init_data(&phdata))
		return (1);
	if (start_routine(&phdata))
		err = 1;
	cleanup_all(&phdata);
	return (err);
}
