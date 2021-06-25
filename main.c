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
	int				dead_flag;
	struct timeval	start_time;
	pthread_mutex_t *fork;
	pthread_mutex_t print;

}				t_philo_op;

typedef struct	s_philo
{
	pthread_t		thread;
	struct	timeval mytime;
	int				eat_count;
	int				eat_flag;
	int				p_num;
	int				fork_num;
	long			die; //죽을 시간
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

long get_time(t_philo *philo)
{
	long f_time;
	long c_time;

	gettimeofday(&(philo->mytime), NULL);
	c_time = philo->mytime.tv_sec * (double)1000 + philo->mytime.tv_usec / (double)1000;
	f_time = philo->op->start_time.tv_sec * (double)1000 + philo->op->start_time.tv_usec / (double)1000;
	return (c_time - f_time);
}

void print_status(t_philo *philo, long time, int p_num, int status)
{
	pthread_mutex_lock(&(philo->op->print));
	if (status == 0)
		printf("%ld : %d has taken a fork\n", time, p_num);
	else if (status == 1)
	{
		philo->die = time + (philo->op->time_to_die / 1000);
		printf("%ld : %d is eating\n", time, p_num);
	}
	else if (status == 2)
		printf("%ld : %d is sleeping\n", time, p_num);
	else if (status == 3)
		printf("%ld : %d is thinking\n", time, p_num);
	else
	{
		printf("%ld : %d died\n", time, p_num);
		philo->op->dead_flag = 1;
	}
	pthread_mutex_unlock(&(philo->op->print));
}

void pick_fork(t_philo *philo, int type)
{
	int fork_index;

	if (type == 0)
		fork_index = philo->p_num % philo->op->num;
	else
		fork_index = (philo->p_num + 1) % philo->op->num;
	pthread_mutex_lock(&(philo->op->fork[fork_index]));
	print_status(philo, get_time(philo), philo->p_num, 0);
	philo->fork_num++;
}

void *t_function(t_philo *philo)
{
	int fork_index;

	while (philo->op->dead_flag == 0)
	{
		pick_fork(philo, philo->p_num % 2);
		pick_fork(philo, (philo->p_num + 1) % 2);
		if (philo->fork_num == 2)
		{
			print_status(philo, get_time(philo), philo->p_num, 1);
			usleep(philo->op->time_to_eat);
			pthread_mutex_unlock(&(philo->op->fork[philo->p_num]));
			pthread_mutex_unlock(&(philo->op->fork[(philo->p_num + 1) % philo->op->num]));
			print_status(philo, get_time(philo), philo->p_num, 2);
			usleep(philo->op->time_to_sleep);
			print_status(philo, get_time(philo), philo->p_num, 3);
			philo->fork_num = 0;
			philo->eat_count++;
			if (philo->eat_count == philo->op->eat_num && philo->eat_flag == 0)
			{
				philo->op->finish_eat++;
				philo->eat_flag = 1;
				if (philo->op->finish_eat == philo->p_num)
					return (NULL);
			}
		}
	}
	return (NULL);
}

void			init_philo(t_philo_op *p, int argc, char **argv)
{
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
}

int				main(int argc, char **argv)
{
	t_philo_op	philo_op;
	t_philo		*philo;
	int			i;
    int			status;
	long		time;

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
		philo[i].eat_count = 0;
		philo[i].eat_flag = 0;
		philo[i].op = &philo_op;
		philo[i].die = philo[i].op->time_to_die;
		gettimeofday(&(philo->op->start_time), NULL);
		pthread_create(&(philo[i].thread), NULL, (void *)t_function, &(philo[i]));
		i++;
	}
	i = 0;
	while (1)
	{
		i %= philo->op->num;
		if ((time = get_time(philo)) >= philo[i].die)
		{
			printf("%ld %ld\n", time, philo[i].die);
			print_status(philo, time, i, 4);
			exit(0);
		}
		i++;
	}
	i = 0;
	//thread 대기
	while (i < philo_op.num)
    	pthread_join(philo[i++].thread, (void *)&status);
	return (0);
}
