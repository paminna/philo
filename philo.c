#include "philo.h"

void	ft_parse(int argc, char **argv, t_table *table)
{
	int	i;

	i = 1;
	ft_init(table);
	if (argc < 5 || argc > 6)
		ft_errors("wrong amount of arguments");
	while (argv[i])
	{
		if (i == 1 && ft_atoi(argv[i]) >= 2)
			table->num_of_philo = ft_atoi(argv[i]);
		else if (i == 2 && ft_atoi(argv[i]) > 0)
			table->time_to_die = ft_atoi(argv[i]);
		else if (i == 3 && ft_atoi(argv[i]) > 0)
			table->time_to_eat = ft_atoi(argv[i]);
		else if (i == 4 && ft_atoi(argv[i]) > 0)
			table->time_to_sleep = ft_atoi(argv[i]);
		else if (i == 5 && ft_atoi(argv[i]))
			table->num_of_each_eats = ft_atoi(argv[i]);
		else
			ft_errors("wrong args");
		i++;
	}
}

void	clear(t_table *table)
{
	int	i;

	i = 0;
	pthread_mutex_destroy(&table->print);
	pthread_mutex_destroy(&table->death);
	pthread_mutex_destroy(&table->time);
	while (i < table->num_of_philo)
	{
		pthread_mutex_destroy(&table->forks[i]);
		i++;
	}
	free(table->forks);
}

long	get_time(void)
{
	struct timeval	cur;

	gettimeofday(&cur, NULL);
	return ((long)((cur.tv_sec * 1000) + (cur.tv_usec / 1000)));
}

void	print_message(char *str, long time, int num, t_philo *philo)
{
	pthread_mutex_lock(&philo->table->print);
	if (!philo->table->is_dead)
		printf("%ld %d %s\n", get_time() - time, num, str);
	pthread_mutex_unlock(&philo->table->print);
}

void	*check_death(void *ph)
{
	t_philo	*philo;

	philo = ph;
	while (1)
	{
		pthread_mutex_lock(&philo->table->death);
		if (get_time() - philo->last_lunch > philo->table->time_to_die)
			break ;
		pthread_mutex_unlock(&philo->table->death);
	}
	if (philo->count_eat != 0)
		print_message("died", philo->table->birth, philo->my_num, philo);
	else
		print_message("all feeded", philo->table->birth, philo->my_num, philo);
	*philo->is_dead = 1;
	pthread_mutex_unlock(&philo->table->death);
	return (NULL);
}
