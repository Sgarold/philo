#include "bonus_philosophers.h"

void	init_philo(t_philo *philo, int i, t_table *table)
{
	philo->index = i;
	philo->num_meals = 0;
	philo->data = (void *)table;
}

int	ft_atoi_err(const char *s, int *err)
{
	int		sign;
	long	nb;

	nb = 0;
	sign = 1;
	while ((*s == 32) || (*s > 8 && *s < 14))
		s++;
	if (*s == 45 || *s == 43)
	{
		if (*s == 45)
			sign = -1;
		s++;
	}
	while (*s >= 48 && *s <= 57)
	{
		nb = nb * 10 + *s - 48;
		s++;
		if ((nb > 2147483648 && sign == -1) || (nb > 2147483647 && sign == 1))
			*err = 1;
	}
	if (*s && (*s < '0' || *s > '9'))
		*err = 1;
	return (sign * nb);
}

void	init_semaphores(t_table *table)
{
	sem_unlink("typing");
	sem_unlink("forks");
	table->typing = sem_open("typing", O_CREAT, S_IRWXU, 1);
	if (table->typing == (sem_t *) -1)
		exit_error("sem error");
	table->forks = sem_open("forks", O_CREAT, S_IRWXU, table->num_of_philo);
	if (table->forks == (sem_t *) -1)
		exit_error("sem error");
}

void	init_table(t_table *table)
{
	int	i;

	i = 0;
	if (table->error_flag)
	{
		printf("error args\n");
		exit(1);
	}
	init_semaphores(table);
	table->philos = malloc(table->num_of_philo * sizeof(t_philo));
	if (!table->philos)
		exit_error("malloc error");
	while (i++ != table->num_of_philo)
		init_philo(&table->philos[i - 1], i, table);
	gettimeofday(&table->timer, NULL);
}

void	parser(int argc, char **argv, t_table *t)
{
	int	err;

	err = 0;
	if (argc == 6 || argc == 5)
	{
		t->num_of_philo = ft_atoi_err(argv[1], &err);
		t->time_to_die = ft_atoi_err(argv[2], &err);
		t->time_to_eat = ft_atoi_err(argv[3], &err);
		t->time_to_sleep = ft_atoi_err(argv[4], &err);
		if (argc == 6)
			t->num_of_times = ft_atoi_err(argv[5], &err);
		else
			t->num_of_times = 2147483647;
		if (t->num_of_philo < 2 || t->time_to_die < 120 || t->time_to_eat < 60 \
		|| t->time_to_sleep < 60 || t->num_of_times < 0)
			err = 1;
		t->error_flag = err;
	}
	else
		t->error_flag = 1;
	init_table(t);
}
