/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jimkwon <jimkwon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/29 12:20:54 by jimkwon           #+#    #+#             */
/*   Updated: 2021/06/29 16:03:06 by jimkwon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int				main(int argc, char **argv)
{
	t_philo_op	philo_op;
	t_philo		*philo;
	int			i;

	if (argc != 5 && argc != 6)
	{
		printf("argument error\n");
		exit(1);
	}
	init_philo_op(&philo_op, argc, argv);
	philo = (t_philo *)malloc(sizeof(t_philo) * philo_op.num);
	i = 0;
	while (i < philo_op.num)
	{
		init_philo(philo, i, philo_op);
		pthread_create(&(philo[i].thread), NULL,
		(void *)t_function, &(philo[i]));
		pthread_detach(philo[i].thread);
		i++;
	}
	find_die(philo);
	i = 0;
	while (i < philo_op.num)
		pthread_mutex_destroy(&(philo->op->fork[i++]));
	return (0);
}
