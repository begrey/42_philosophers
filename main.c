#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>

typedef struct	s_philo_op
{
	int				num;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				eat_num;
	int				finish_eat;
	pthread_mutex_t *fork;
	pthread_mutex_t print;

}				t_philo_op;

typedef struct	s_philo
{
	pthread_t		thread;
	struct	timeval mytime;
	int				p_num;
	int				fork_num;
	t_philo_op		*op;
}				t_philo;


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

void print_status(t_philo *philo, long time, int p_num, int status)
{
	pthread_mutex_lock(&(philo->op->print));
	if (status == 0)
		printf("%ld : %d has taken a fork\n", time, p_num);
	else if (status == 1)
		printf("%ld : %d is eating\n", time, p_num);
	else if (status == 2)
		printf("%ld : %d is sleeping\n", time, p_num);
	else if (status == 3)
		printf("%ld : %d is thinking\n", time, p_num);
	else
		printf("%ld : %d died\n", time, p_num);
	pthread_mutex_unlock(&(philo->op->print));
}

void pick_left_fork(t_philo *philo)
{
	int fork_index;

	fork_index = philo->p_num % philo->op->num;
	pthread_mutex_lock(&(philo->op->fork[fork_index]));
    gettimeofday(&(philo->mytime), NULL);
	print_status(philo, philo->mytime.tv_usec * 1000, philo->p_num, 0);
	philo->fork_num++;
}

void pick_right_fork(t_philo *philo)
{
	int fork_index;

	fork_index = (philo->p_num + 1) % philo->op->num;
	pthread_mutex_lock(&(philo->op->fork[fork_index]));
	gettimeofday(&(philo->mytime), NULL);
	print_status(philo, philo->mytime.tv_usec * 1000, philo->p_num, 0);;
	philo->fork_num++;
}



void *t_function(t_philo *philo)
{
	int fork_index;

	while (philo->op->finish_eat <= 15)
	{
		if (philo->p_num % 2 == 0)
			pick_left_fork(philo);
		else
			pick_right_fork(philo);
		if (philo->p_num % 2 == 0)
			pick_right_fork(philo);
		else
			pick_left_fork(philo);
		if (philo->fork_num == 2)
		{
			gettimeofday(&(philo->mytime), NULL);
			//philo->eat_time = philo->mytime.tv_usec * 1000;
			usleep(philo->op->time_to_eat);
			pthread_mutex_unlock(&(philo->op->fork[philo->p_num]));
			pthread_mutex_unlock(&(philo->op->fork[(philo->p_num + 1) % philo->op->num]));
			//print_status(philo);
			gettimeofday(&(philo->mytime), NULL);
			usleep(philo->op->time_to_sleep);
			philo->fork_num = 0;
			philo->op->finish_eat++;
		}
		// while(1)
		// {
		// 	if (philo->op->finish_eat % 5 == 0)
		// 		break;
		// }
	}
	return (NULL);
}

void			init_philo(t_philo_op *p, int argc, char **argv)
{
	p->num = ft_atoi(argv[1]);
	p->time_to_die = ft_atoi(argv[2]);
	p->time_to_eat = ft_atoi(argv[3]);
	p->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		p->eat_num = ft_atoi(argv[5]);
	else
		p->eat_num = -1;
	p->finish_eat = 0;
	p->fork = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * p->num);
}

int				main(int argc, char **argv)
{
	t_philo_op	philo_op;
	t_philo		*philo;
	int			i;
    int			status;

	i = 0;
	if (argc < 5)
	{
		printf("need more argument\n");
		exit(1);
	}

	//구조체 init
	init_philo(&philo_op, argc, argv);
	//mutex_init
	pthread_mutex_init(&(philo_op.print), NULL);
	while (i < philo_op.num)
		pthread_mutex_init(&(philo_op.fork[i++]), NULL);
	philo = (t_philo *)malloc(sizeof(t_philo) * philo_op.num);
	i = 0;
	//thread 생성
	while (i < philo_op.num)
	{
		philo[i].p_num = i;//스레드 구분자를 어떻게 설정해줄까
		philo[i].fork_num = 0; //포크 쥔 개수 초기화
		philo[i].op = &philo_op;
		pthread_create(&(philo[i].thread), NULL, (void *)t_function, &(philo[i]));
		i++;
	}
	i = 0;
	//thread 대기
	while (i < philo_op.num)
    	pthread_join(philo[i++].thread, (void *)&status);
	return (0);
}
