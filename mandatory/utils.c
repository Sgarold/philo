#include "philosophers.h"

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

void	usleeper(long long time, struct timeval now)
{
	long long	cur_time;

	cur_time = get_time_ms(now);
	usleep((time - 15) * 1000);
	while (1)
	{
		usleep(50);
		if (get_time_ms(now) >= cur_time + time)
			break ;
	}
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

void	*death_checker(void *arg)
{
	int		i;
	int		flag;
	t_table	*table;

	table = (t_table *)arg;
	while (1)
	{
		i = 0;
		while (i < table->num_of_philo)
		{
			if (get_time_ms(table->timer) - \
			 table->philos[i].last_meal \
			> (long long)table->time_to_die)
				change_status(table, "died", i, 1);
			i++;
		}
		flag = 0;
		i = 0;
		while (i < table->num_of_philo)
			if (table->philos[i++].num_meals >= table->num_of_times)
				flag++;
		if (flag == table->num_of_philo)
			change_status(table, "philos are full", table->num_of_philo - 1, 1);
	}
}
