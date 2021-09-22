#include "philosophers.h"
void	philo_eats_odd(t_table *table, int i)
{
	pthread_mutex_lock(table->philos[i].left_fork);
	change_status(table, "has taken a fork", i, 0);
	pthread_mutex_lock(table->philos[i].right_fork);
	change_status(table, "has taken a fork", i, 0);
	change_status(table, "is eating", i, 0);
	table->philos[i].last_meal = get_time_ms(table->timer);
	usleeper(table->time_to_eat, table->timer);
	pthread_mutex_unlock(table->philos[i].right_fork);
	pthread_mutex_unlock(table->philos[i].left_fork);
	change_status(table, "is sleeping", i, 0);
	usleeper(table->time_to_sleep, table->timer);
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
	usleeper(table->time_to_eat, table->timer);
	pthread_mutex_unlock(table->philos[i].left_fork);
	pthread_mutex_unlock(table->philos[i].right_fork);
	change_status(table, "is sleeping", i, 0);
	usleeper(table->time_to_sleep, table->timer);
	change_status(table, "is thinking", i, 0);
}

void	*philo_funk(void *arg)
{
	t_table		*table;
	int			i;

	table = (t_table *)arg;
	i = table->index++;
	if (!(table->num_of_philo % 2) && i % 2 && table->philos[i].flag)
	{
		table->philos[i].flag = 0;
		usleep(100);
		if (i + 1 == table->num_of_philo)
			usleep(200);
	}
	while (1)
	{
		if (i % 2)
			philo_eats_odd(table, i);
		else
			philo_eats_even(table, i);
		table->philos[i].num_meals += 1;
		if (table->philos[i].num_meals == table->num_of_times)
			table->philos[i].philo_is_full = 1;
	}
	return (NULL);
}
