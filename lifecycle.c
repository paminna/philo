#include "philo.h"

void	ft_eat(t_philo *philo)
{
	if (philo->my_num % 2)
	{
		pthread_mutex_lock(philo->left_fork);
		pthread_mutex_lock(philo->right_fork);
		print_message("has taken a fork",
			philo->table->birth, philo->my_num, philo);
	}
	else
	{
		pthread_mutex_lock(philo->right_fork);
		pthread_mutex_lock(philo->left_fork);
		print_message("has taken a fork",
			philo->table->birth, philo->my_num, philo);
	}
	print_message("is eating", philo->table->birth, philo->my_num, philo);
	pthread_mutex_lock(&philo->table->time);
	philo->last_lunch = get_time();
	pthread_mutex_unlock(&philo->table->time);
	philo->count_eat--;
	ft_wait(philo->table->time_to_eat);
	pthread_mutex_unlock(philo->right_fork);
	pthread_mutex_unlock(philo->left_fork);
}

void	ft_sleep(t_philo *philo)
{
	print_message("is sleeping", philo->table->birth, philo->my_num, philo);
	ft_wait(philo->table->time_to_sleep);
}

void	ft_wait(int time_to_wait)
{
	long	t_start;
	long	t_end;

	t_start = get_time();
	t_end = t_start + time_to_wait;
	while (t_start < t_end)
	{
		t_start = get_time();
		usleep(10);
	}
}

void	ft_think(t_philo *philo)
{
	print_message("is thinking", philo->table->birth, philo->my_num, philo);
}

void	*lifecycle(void *ph)
{
	t_philo		*philo;
	pthread_t	death;

	philo = ph;
	philo->last_lunch = get_time();
	philo->table->birth = get_time();
	pthread_create(&death, NULL, check_death, philo);
	while (!philo->table->is_dead && philo->count_eat != 0)
	{
		ft_eat(philo);
		ft_sleep(philo);
		ft_think(philo);
	}
	pthread_join(death, NULL);
	return (NULL);
}
