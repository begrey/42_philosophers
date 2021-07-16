/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jimkwon <jimkwon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/29 12:23:14 by jimkwon           #+#    #+#             */
/*   Updated: 2021/06/29 18:54:54 by jimkwon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void		pick_fork(t_philo *philo, int type)
{
	int		fork_index;

	if (type == 0)
		fork_index = philo->p_num % philo->op->num;
	else
		fork_index = (philo->p_num + 1) % philo->op->num;
	pthread_mutex_lock(&(philo->op->fork[fork_index]));
	print_status(philo, philo->p_num, 0);
	philo->fork_num++;
}

int			finish_eat(t_philo *philo)
{
	print_status(philo, philo->p_num, 1);
	if (philo->eat_count == philo->op->eat_num)
	{
		philo->op->finish_eat++;
		if (philo->op->finish_eat == philo->p_num)
			return (1);
	}
	ft_sleep(philo->op->time_to_eat);
	pthread_mutex_unlock(&(philo->op->fork[philo->p_num]));
	pthread_mutex_unlock(&(philo->op->fork[(philo->p_num + 1)
	% philo->op->num]));
	print_status(philo, philo->p_num, 2);
	ft_sleep(philo->op->time_to_sleep);
	print_status(philo, philo->p_num, 3);
	philo->fork_num = 0;
	philo->eat_count++;
	return (0);
}

void		*t_function(t_philo *philo)
{
	while (philo->op->dead_flag == 0)
	{
		if (philo->p_num % 2 == 0)
			ft_sleep(1000);
		pick_fork(philo, 0);
		pick_fork(philo, 1);
		if (philo->fork_num == 2)
		{
			if ((finish_eat(philo)) == 1)
				philo->op->dead_flag = 1;
		}
	}
	return (NULL);
}

int			find_die(t_philo *philo)
{
	int		i;
	long	time;

	i = 0;
	while (1)
	{
		i %= philo->op->num;
		if ((time = get_time(philo)) >= philo[i].die)
		{
			philo->op->dead_flag = 1;
			ft_sleep(100);
			print_status(philo, i, 4);
			pthread_mutex_destroy(&(philo->op->print));
			return (1);
		}
		if (philo->op->dead_flag == 1)
			return (1);
		i++;
	}
}
