#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <stdio.h>
# include <unistd.h>
# include <pthread.h>
# include <stdlib.h>
# include <sys/time.h>

typedef struct s_philo
{
	int				index;
	long long		last_meal;
	int				num_meals;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	int				flag;
	int				philo_is_full;
}					t_philo;

typedef struct s_table
{
	int					num_of_philo;
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

int			ft_atoi_err(const char *s, int *err);
void		parser(int argc, char **argv, t_table *table);
void		philo_init(t_table *table, int i);
void		table_init(t_table *table, int i);
long long	get_time_ms(struct timeval start);
void		usleeper(long long time, struct timeval now);
void		change_status(t_table *table, char *str, int i, int flag);
void		philo_eats_odd(t_table *table, int i);
void		philo_eats_even(t_table *table, int i);
void		*philo_funk(void *arg);
void		*death_checker(void *arg);
void		thread_creating(t_table *table);

#endif
