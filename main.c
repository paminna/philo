#include "philo.h"

void	give_forks(t_table *table, t_philo *philo)
{
	int	i;

	i = 0;
	table->forks = (pthread_mutex_t *)malloc
		(table->num_of_philo * sizeof(pthread_mutex_t));
	if (table->forks == NULL)
		ft_errors("malloc error");
	while (i < table->num_of_philo)
	{
		philo[i].right_fork = &table->forks[i];
		if (i == 0)
			philo[i].left_fork = &table->forks[table->num_of_philo - 1];
		else
			philo[i].left_fork = &table->forks[i - 1];
		i++;
	}
}

void	ft_init_philo(t_philo *philo, t_table *table)
{
	int	i;
	int	j;

	j = 0;
	i = 0;
	give_forks(table, philo);
	while (i != table->num_of_philo)
	{
		pthread_mutex_init(&(table->forks[i]), NULL);
		i++;
	}
	i = 0;
	while (i != table->num_of_philo)
	{
		philo[i].my_num = i + 1;
		philo[i].is_dead = 0;
		philo[i].eat = table->time_to_eat;
		philo[i].count_eat = table->num_of_each_eats;
		philo[i].table = table;
		philo[i].is_dead = &table->is_dead;
		i++;
	}
}

void	ft_start_philo(t_table 	*table)
{
	int			i;
	t_philo		*philo;
	pthread_t	*th_philo;

	i = 0;
	th_philo = (pthread_t *)malloc(sizeof(pthread_t) * (table->num_of_philo));
	philo = (malloc(sizeof(t_philo) * table->num_of_philo));
	ft_init_philo(philo, table);
	while (i < table->num_of_philo)
	{
		pthread_create(&th_philo[i], NULL, lifecycle, &philo[i]);
		i++;
	}
	i = 0;
	while (i < table->num_of_philo)
	{
		pthread_join(th_philo[i], NULL);
		i++;
	}
	clear(table);
	free (philo);
	free(th_philo);
}

int	main(int argc, char **argv)
{
	t_table		table;

	ft_parse(argc, argv, &table);
	ft_start_philo(&table);
	return (0);
}
