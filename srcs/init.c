/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jimkwon <jimkwon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/29 12:23:40 by jimkwon           #+#    #+#             */
/*   Updated: 2021/06/29 16:02:30 by jimkwon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	init_philo_op(t_philo_op *p, int argc, char **argv)
{
	int	i;

	i = 0;
	p->num = ft_atoi(argv[1]);
	p->time_to_die = ft_atoi(argv[2]) * 1000;
	p->time_to_eat = ft_atoi(argv[3]) * 1000;
	p->time_to_sleep = ft_atoi(argv[4]) * 1000;
	if (argc == 6)
		p->eat_num = ft_atoi(argv[5]);
	else
		p->eat_num = -1;
	p->finish_eat = 0;
	p->dead_flag = 0;
	p->fork = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * p->num);
	pthread_mutex_init(&(p->print), NULL);
	while (i < p->num)
		pthread_mutex_init(&(p->fork[i++]), NULL);
}

void	init_philo(t_philo *philo, int i, t_philo_op op)
{
	philo[i].p_num = i;
	philo[i].fork_num = 0;
	philo[i].eat_count = 0;
	philo[i].op = &op;
	philo[i].die = philo[i].op->time_to_die / 1000;
	gettimeofday(&(philo[i].mytime), NULL);
	gettimeofday(&(philo->op->start_time), NULL);
}
