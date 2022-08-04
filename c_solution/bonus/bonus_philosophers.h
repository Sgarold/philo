#ifndef BONUS_PHILOSOPHERS_H
# define BONUS_PHILOSOPHERS_H

# include <stdio.h>
# include <fcntl.h>
# include <signal.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/wait.h>
# include <sys/time.h>
# include <semaphore.h>

typedef struct s_philo
{
	int					index;
	unsigned long		last_meal;
	int					num_meals;
	pthread_t			thread;
	pid_t				pid;
	void				*data;
}					t_philo;

typedef struct s_table
{
	int				num_of_philo;
	int				time_to_die;
	int				time_to_sleep;
	int				time_to_eat;
	int				num_of_times;
	int				error_flag;
	long long		start;
	sem_t			*forks;
	sem_t			*typing;
	t_philo			*philos;
	struct timeval	timer;
}				t_table;

long long	get_time_ms(struct timeval start);
void		exit_error(char *str);
void		*life(void *arg);
void		parser(int argc, char **argv, t_table *t);
void		usleeper(long long time, struct timeval now);

#endif