#include "philo.h"

void *my_funk()
{
	int *res;
	int val;
	val = (rand() % 6) + 1;
	res = malloc(sizeof(int));
	*res = val;  
	return((void *)res);
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
		if ((nb > 2147483648 && sign == -1) || (nb > INT32_MAX && sign == 1))
			*err = 1;
	}
	if (*s && (*s < '0' || *s > '9'))
		*err = 1;
	return (sign * nb);
}

void	parser(int argc, char **argv, t_table *table) // numb_philo time_to_die time_to_eat time_to_sleep [num_of_times]
{
	int i;
	int err;

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

int main(int argc, char **argv)
{
	t_table table;


	parser(argc, argv, &table);
	if (table.error_flag)
		return(0);
	printf("%d, %d, %d, %d, %d, %d\n", table.num_of_phlio, table.time_to_die, table.time_to_eat, table.time_to_sleep, table.num_of_times, table.error_flag);
	// int *res;
	// pthread_t t1[val];
	// int i;

	// for (int i = 0; i < val; i++)
	// 	pthread_create(&t1[i], NULL, my_funk, NULL);
	// for (int i = 0; i < val; i++)
	// {
	// 	pthread_join(t1[i], (void **) &res);
	// 	printf("dice is equal %d\n", *res);
	// 	free(res);
	// }
	// while(1);
	// return(0);
}
		// printf("%d, %d\n", table->num_of_phlio, ft_atoi_err(argv[1], &err));