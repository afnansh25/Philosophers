/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/25 17:38:13 by codespace         #+#    #+#             */
/*   Updated: 2025/10/26 07:54:29 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int main(int ac, char **av)
{
    t_data  phdata;
    
    if (ac != 5 && ac != 6)
    {
		printf("Usage: ./philo num_philo time_to_die ");
		printf("time_to_eat time_to_sleep [eat_limit]\n");
        return (1);
    }
    if (parse_args(ac, av, &phdata))
        return (1);
    if (init_data(&phdata))
        return (1);
    // if (start_routine(&phdata))
    // {
    //     cleanup_all(&phdata);
    //     return (1);
    // }
    cleanup_all(&phdata);
    return (0);
}
