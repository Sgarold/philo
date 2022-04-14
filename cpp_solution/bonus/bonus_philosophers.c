#include "bonus_philosophers.h"

int	death_checker(t_philo *p, int *flag, t_table *table)
{
	unsigned long	time;

	time = get_time_ms(table->timer);
	if (p->num_meals < table->num_of_times)
		*flag = 0;
	if (time > p->last_meal && time - p->last_meal
		> (unsigned long)table->time_to_die)
	{
		sem_wait(table->typing);
		printf("%lu %d %s", time, p->index, "died\n");
		return (0);
	}
	return (1);
}

void	*checker(void *arg)
{
	int			i;
	int			flag;
	t_philo		*p;
	t_table		*table;

	p = (t_philo *)arg;
	table = (t_table *)p->data;
	i = -1;
	while (1)
	{
		flag = 1;
		if (death_checker(p, &flag, table) == 0)
			exit(0);
		if (table->num_of_times != -1 && flag == 1)
			exit(1);
		usleep(100);
	}
	return (NULL);
}

void	child(int i, t_table *table)
{
	pthread_t	death;

	table->philos[i].last_meal = get_time_ms(table->timer);
	pthread_create(&table->philos[i].thread, NULL, life,
		(void *)&table->philos[i]);
	pthread_detach(table->philos[i].thread);
	pthread_create(&death, NULL, checker, (void *)&table->philos[i]);
	pthread_join(death, NULL);
}

void	waiter(t_table *table)
{
	int	i;
	int	return_val;

	i = -1;
	while (++i < table->num_of_philo)
	{
		waitpid(-1, &return_val, 0);
		if (!WEXITSTATUS(return_val))
		{
			i = 0;
			while (i < table->num_of_philo)
				kill(table->philos[i++].pid, SIGKILL);
		}
	}
}

int	main(int argc, char **argv)
{
	int		i;
	t_table	table;

	parser(argc, argv, &table);
	i = -1;
	while (++i < table.num_of_philo)
	{
		table.philos[i].pid = fork();
		if (table.philos[i].pid == -1)
			exit_error("process init error");
		else if (table.philos[i].pid == 0)
			child(i, &table);
	}
	waiter(&table);
	return (0);
}
