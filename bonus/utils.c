#include "bonus_philosophers.h"

void	exit_error(char *str)
{
	printf("%s\n", str);
	exit(1);
}

long long	get_time_ms(struct timeval timer)
{
	struct timeval	now;

	gettimeofday(&now, NULL);
	now.tv_sec -= timer.tv_sec;
	now.tv_usec -= timer.tv_usec;
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
