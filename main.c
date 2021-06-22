#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
 
typedef struct	s_philo
{
	int				num;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				eat_num;
	pthread_t		thread;
	pthread_mutex_t *fork;
}				t_philo;

// 뮤텍스 객체 선언
pthread_mutex_t fork1;

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

void *t_function(void *data)
{
    int i;
    char* thread_name = (char*)data;

    pthread_mutex_lock(&fork1);
    
	pthread_mutex_unlock(&fork1);

	printf("%s end\n", thread_name);
}

init_philo(t_philo *p, int argc, char **argv)
{
	p->num = argv[1];
	p->time_to_die = argv[2];
	p->time_to_eat = argv[3];
	p->time_to_sleep = argv[4];
	if (argc == 6)
		p->eat_num = argv[5];
	else
		p->eat_num = -1;
	p->fork = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * 5);
}

int main(int argc, char **argv)
{
	pthread_t	*philo;
	t_philo		p;
	int			i;

	i = 0;
	if (argc < 5)
	{
		printf("need more argument\n");
		exit(1);
	}
	init_philo(&p, argv, argc);
	//mutex_init
	while (i < p.num)
		pthread_mutex_init(&(p.fork[i++]), NULL);
	philo = (pthread_t *)malloc(sizeof(pthread_t) * 5);
	i = 0;
	//thread 생성
	while (i < p.num)
		pthread_create(&philo[i++], NULL, t_function, (void *)"philosopher");
    int status;

 
	for (int i = 0; i < 5; i++)
    	pthread_join(philo[i], (void *)&status);
}
