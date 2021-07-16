/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jimkwon <jimkwon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/29 12:20:39 by jimkwon           #+#    #+#             */
/*   Updated: 2021/06/29 18:48:39 by jimkwon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <sys/time.h>

typedef struct	s_philo_op
{
	int				num;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				eat_num;
	int				finish_eat;
	int				dead_flag;
	struct timeval	start_time;
	pthread_mutex_t *fork;
	pthread_mutex_t print;
}				t_philo_op;

typedef struct	s_philo
{
	pthread_t		thread;
	struct timeval	mytime;
	int				eat_count;
	int				p_num;
	int				fork_num;
	long			die;
	t_philo_op		*op;
}				t_philo;

int				ft_atoi(const char *str);
void			ft_sleep(long time);
long			get_time(t_philo *philo);
void			print_status(t_philo *philo, int p_num, int status);
void			*t_function(t_philo *philo);
void			init_philo_op(t_philo_op *p, int argc, char **argv);
void			init_philo(t_philo *philo, int i, t_philo_op op);
int				find_die(t_philo *philo);
#endif
