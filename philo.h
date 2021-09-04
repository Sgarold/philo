#ifndef PHILO_H
# define PHILO_H

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/time.h>

typedef struct s_philo
{
	int				index;
	long long		last_meal;
	int				num_meals;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	int				flag;
}					t_philo;

typedef struct s_table
{
	int					num_of_phlio;
	int					time_to_die;
	int					time_to_eat;
	int					time_to_sleep;
	int					num_of_times;
	int					error_flag;
	int					index;
	unsigned long long	start;
	t_philo				*philos;
	pthread_mutex_t		*typing;
	pthread_mutex_t		**forks;
	struct timeval		timer;
}			t_table;



#endif