#include "bonus_philosophers.h"

void	print_what_philo_do(unsigned long time, int index, char *str,
	 t_table *table)
{
	sem_wait(table->typing);
	printf("%lu %d %s", time, index, str);
	sem_post(table->typing);
}

void	change_status(t_table *table, char *str, int i)
{
	sem_wait(table->typing);
	printf("%lld %d %s\n", get_time_ms(table->timer), i, str);
	sem_post(table->typing);
}

void	philo_life(t_philo **p, t_table *table)
{
	sem_wait(table->forks);
	sem_wait(table->forks);
	change_status(table, "take a fork", (*p)->index);
	change_status(table, "take a fork", (*p)->index);
	(*p)->last_meal = get_time_ms(table->timer);
	change_status(table, "is eating", (*p)->index);
	usleeper((long long)table->time_to_eat, table->timer);
	sem_post(table->forks);
	sem_post(table->forks);
	(*p)->num_meals++;
	change_status(table, "is sleeping", (*p)->index);
	usleeper((long long)table->time_to_sleep, table->timer);
	change_status(table, "is thinking", (*p)->index);
}

void	*life(void *arg)
{
	t_philo			*p;
	unsigned long	time;
	t_table			*table;

	p = (t_philo *)arg;
	table = (t_table *)p->data;
	time = 0;
	while (1)
		philo_life(&p, table);
	return (NULL);
}
