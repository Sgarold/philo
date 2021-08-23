#include "philo.h"

typedef struct jopa
{
	int index;
	int time_to_die;
	int time_to_eat;
	int time_to_sleep;

	pthread_mutex_t left_fork;
	pthread_mutex_t right_fork;
}			t_p;

void init_philo1(t_p *p, pthread_mutex_t w, pthread_mutex_t e)
{
	p->index = 0;
	p->time_to_die = 700;
	p->time_to_eat = 100;
	p->time_to_sleep = 100;
	p->left_fork = e;
	p->right_fork = w;
}
void init_philo2(t_p *p, pthread_mutex_t w, pthread_mutex_t e)
{
	p->index = 1;
	p->time_to_die = 700;
	p->time_to_eat = 100;
	p->time_to_sleep = 100;
	p->left_fork = w;
	p->right_fork = e;
}

void *phonk(t_p *ph)
{

	while(1)
	{
		if (ph->index % 2)
		{
			pthread_mutex_lock(&ph->right_fork);
			pthread_mutex_lock(&ph->left_fork);
		}
		else
		{
			pthread_mutex_lock(&ph->left_fork);
			pthread_mutex_lock(&ph->right_fork);
		}
		printf("thread %d doing smth\n", ph->index + 1 );
		usleep(100000);
		if (ph->index % 2)
		{
			pthread_mutex_unlock(&ph->left_fork);
			pthread_mutex_unlock(&ph->right_fork);
		}
		else
		{
			pthread_mutex_unlock(&ph->right_fork);
			pthread_mutex_unlock(&ph->left_fork);
		}
	}
	return (void *) 0;
}

int main()
{
	pthread_t upper_philo;
	pthread_t lower_philo;
	pthread_mutex_t west_fork;
	pthread_mutex_t east_fork;
	t_p	north;
	t_p south;

	pthread_mutex_init(&west_fork, NULL);
	pthread_mutex_init(&east_fork, NULL);

	init_philo1(&north, west_fork, east_fork);
	init_philo2(&south, west_fork, east_fork);

	pthread_create(&upper_philo, NULL, phonk, &north);
	pthread_create(&lower_philo, NULL, phonk, &south);
	pthread_join(upper_philo, NULL);
	pthread_join(lower_philo, NULL);

}