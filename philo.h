#ifndef PHILO_H
# define PHILO_H

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>

typedef struct s_philo
{
	int index;
	pthread_mutex_t left_fork;
	pthread_mutex_t right_fork;
	int last_meal;
	int	num_meals;
}				t_philo;

typedef struct s_table
{
	int num_of_phlio;
	int time_to_die;
	int time_to_eat;
	int time_to_sleep;
	int num_of_times;
	int error_flag;
}			t_table;



#endif