#include "philosophers.h"

void	parser(int argc, char **argv, t_table *table)
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
			table->num_of_times = 2147483647;
		table->error_flag = err;
	}
	else
		table->error_flag = 1;
}

void	philo_init(t_table *table, int i)
{
	table->philos[i].index = i + 1;
	table->philos[i].num_meals = 0;
	table->philos[i].flag = 1;
	table->philos[i].philo_is_full = 0;
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
