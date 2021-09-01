#include "philo.h"

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
		if ((nb > 2147483648 && sign == -1) || (nb > INT32_MAX && sign == 1))
			*err = 1;
	}
	if (*s && (*s < '0' || *s > '9'))
		*err = 1;
	return (sign * nb);
}

void	parser(int argc, char **argv, t_table *table) // numb_philo time_to_die time_to_eat time_to_sleep [num_of_times]
{
	int	i;
	int	err;

	i = 0;
	err = 0;
	if (argc == 6 || argc == 5)
	{
		table->num_of_phlio = ft_atoi_err(argv[1], &err);
		table->time_to_die = ft_atoi_err(argv[2], &err);
		table->time_to_eat = ft_atoi_err(argv[3], &err);
		table->time_to_sleep = ft_atoi_err(argv[4], &err);
		if (argc == 6)
			table->num_of_times = ft_atoi_err(argv[5], &err);
		else
			table->num_of_times = -1;
		table->error_flag = err;
	}
	else
		table->error_flag = 1;
}

void	philo_init(t_table *table, int i)
{
	table->philos[i].index = i + 1;
	table->philos[i].num_meals = 0;
	table->philos[i].right_fork = table->forks[i];
	if (i + 1 == table->num_of_phlio)
		table->philos[i].left_fork = table->forks[0];
	else
		table->philos[i].left_fork = table->forks[i + 1];
}	

void	table_init(t_table *table)
{
	int	i;

	i = 0;
	table->index = 0;
	table->forks = malloc(sizeof(pthread_mutex_t *) * table->num_of_phlio);
	while (i < table->num_of_phlio)
		table->forks[i++] = malloc(sizeof(pthread_mutex_t));
	i = 0;
	while (i < table->num_of_phlio)
		pthread_mutex_init(table->forks[i++], NULL);
	table->typing = malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(table->typing, NULL);
	table->philos = malloc(sizeof(t_philo) * table->num_of_phlio);
	i = 0;
	while (i < table->num_of_phlio)
		philo_init(table, i++);
	table->start = gettimeofday(&table->timer, NULL);
}

long long	get_time_ms(struct timeval start)
{
	struct timeval	now;

	gettimeofday(&now, NULL);
	now.tv_sec -= start.tv_sec;
	now.tv_usec -= start.tv_usec;
	if (now.tv_usec < 0)
	{
		now.tv_sec--;
		now.tv_usec += 1000000;
	}
	return ((now.tv_usec * 0.001) + (now.tv_sec * 1000));
}

void	change_status(t_table *table, char *str, int i, int flag)
{
	i++;
	pthread_mutex_lock(table->typing);
	printf("%lld %d %s\n", get_time_ms(table->timer), i, str);
	if (flag)
		exit(42);
	pthread_mutex_unlock(table->typing);
}

void	philo_eats_odd(t_table *table, int i)
{
	if ((table->num_of_phlio % 2))
		usleep(5);
	pthread_mutex_lock(table->philos[i].left_fork);
	change_status(table, "has taken a fork", i, 0);
	pthread_mutex_lock(table->philos[i].right_fork);
	change_status(table, "has taken a fork", i, 0);
	change_status(table, "is eating", i, 0);
	table->philos[i].last_meal = get_time_ms(table->timer);
	usleep(table->time_to_eat * 1000);
	pthread_mutex_unlock(table->philos[i].right_fork);
	pthread_mutex_unlock(table->philos[i].left_fork);
	change_status(table, "is sleeping", i, 0);
	usleep(table->time_to_sleep);
	change_status(table, "is thinking", i, 0);
}

void	philo_eats_even(t_table *table, int i)
{
	pthread_mutex_lock(table->philos[i].right_fork);
	change_status(table, "has taken a fork", i, 0);
	pthread_mutex_lock(table->philos[i].left_fork);
	change_status(table, "has taken a fork", i, 0);
	table->philos[i].last_meal = get_time_ms(table->timer);
	change_status(table, "is eating", i, 0);
	usleep(table->time_to_eat * 1000);
	pthread_mutex_unlock(table->philos[i].left_fork);
	pthread_mutex_unlock(table->philos[i].right_fork);
	change_status(table, "is sleeping", i, 0);
	usleep(table->time_to_sleep);
	change_status(table, "is thinking", i, 0);
}

void	*philo_funk(void *arg)
{
	t_table	*table;
	int		philo_index;

	table = (t_table *)arg;
	philo_index = table->index++;
	while (table->philos[philo_index].num_meals != table->num_of_times)
	{
		if (philo_index % 2)
			philo_eats_odd(table, philo_index);
		else
			philo_eats_even(table, philo_index);
		table->philos[philo_index].num_meals += 1;
	}
	return (NULL);
}

void	death_checker(t_table *table)
{
	int i;

	while(1)
	{
		i = 0;
		while(i < table->num_of_phlio)
		{
			if (get_time_ms(table->timer) - table->philos[i].last_meal > (long long)table->time_to_die)
				change_status(table, "died", i, 1);
			i++;
		}
	}
}

void	thread_creating(t_table *table)
{
	pthread_t	*threads;
	int			i;

	i = 0;
	threads = malloc(sizeof(pthread_t) * table->num_of_phlio);
	while (i < table->num_of_phlio)
		pthread_create(threads + i++, NULL, philo_funk, table);
	i = 0;
	death_checker(table);
	while (i < table->num_of_phlio)
		pthread_join(threads[i++], NULL);
}

int	main(int argc, char **argv)
{
	t_table	table;

	parser(argc, argv, &table);
	if (table.error_flag)
		return (0);
	table_init(&table);
	thread_creating(&table);

}