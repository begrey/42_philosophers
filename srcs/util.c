/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jimkwon <jimkwon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/29 12:22:31 by jimkwon           #+#    #+#             */
/*   Updated: 2021/06/29 17:08:58 by jimkwon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int				ft_atoi(const char *str)
{
	long long	minus;
	long long	total;

	minus = 1;
	total = 0;
	while (*str == ' ' || (*str >= 9 && *str <= 13))
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			minus = -1;
		str++;
	}
	if (*str != '\0' && (*str >= '0' && *str <= '9'))
		total += *str++ - '0';
	while (*str != '\0' && (*str >= '0' && *str <= '9'))
	{
		if (minus == 1 && total > 2147483647)
			return (-1);
		if (minus == -1 && total > 2147483648)
			return (0);
		total *= 10;
		total += *str++ - '0';
	}
	return ((int)(total * minus));
}

long			current_mili_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void			ft_sleep(long time)
{
	long	obj_time;

	obj_time = time / 1000 + current_mili_time();
	while (obj_time > current_mili_time())
		usleep(100);
}

long			get_time(t_philo *philo)
{
	long		f_time;
	long		c_time;

	gettimeofday(&(philo->mytime), NULL);
	c_time = philo->mytime.tv_sec *
	(double)1000 + philo->mytime.tv_usec / (double)1000;
	f_time = philo->op->start_time.tv_sec *
	(double)1000 + philo->op->start_time.tv_usec / (double)1000;
	return (c_time - f_time);
}

void			print_status(t_philo *philo, int p_num, int status)
{
	pthread_mutex_lock(&(philo->op->print));
	if (status == 0)
		printf("%ld : %d has taken a fork\n", get_time(philo), p_num + 1);
	else if (status == 1)
	{
		philo->die = get_time(philo) + (philo->op->time_to_die / 1000);
		printf("%ld : %d is eating\n", get_time(philo), p_num + 1);
	}
	else if (status == 2)
		printf("%ld : %d is sleeping\n", get_time(philo), p_num + 1);
	else if (status == 3)
		printf("%ld : %d is thinking\n", get_time(philo), p_num + 1);
	else
	{
		printf("%ld : %d died\n", get_time(philo), p_num + 1);
		philo->op->dead_flag = 1;
	}
	pthread_mutex_unlock(&(philo->op->print));
}
