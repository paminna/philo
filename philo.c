#include "philo.h"

void	ft_init(t_table *table)
{
	table->num_of_philo = 0;
	table->num_of_each_eats = 0;
	table->time_to_eat = 0;
	table->time_to_sleep = 0;
	table->time_to_die = 0;
}

void	ft_parse(int argc, char **argv, t_table *table)
{
	int i;
	int j;

	i = 1;
	j = 0;
	if (argc < 5 || argc > 6)
		ft_errors("wrong amount of arguments");
	while (argv[i])
	{
		if (i == 1 && ft_isdigit(argv[i][j]))
			table->num_of_philo = ft_atoi(argv[i]);
		else if (i == 2 && ft_isdigit(argv[i][j]))
			table->time_to_die = ft_atoi(argv[i]);
		else if (i == 3 && ft_isdigit(argv[i][j]))
			table->time_to_eat = ft_atoi(argv[i]);
		else if (i == 4 && ft_isdigit(argv[i][j]))
			table->time_to_sleep = ft_atoi(argv[i]);
		else if (i == 5 && ft_isdigit(argv[i][j]))
			table->num_of_each_eats = ft_atoi(argv[i]);
		else
			ft_errors("wrong args");
		i++;
	}
}


int	get_time(struct timeval *cur)
{
	gettimeofday(cur, NULL);
	return((int)((cur->tv_usec * 1000) - (cur->tv_sec / 1000)));
}

void	ft_wait(int time_to_wait)
{
	struct timeval start;
	int t_start;
	int t_end;

	t_start = 0;
	t_end = t_start + time_to_wait;
	while (t_start < t_end)
		t_start = t_start + get_time(&start);
}

void	ft_eat(t_philo *philo)
{

}

void	*lifecycle(void *ph)
{
	int i;
	t_philo *philo;

	philo = (t_philo*)ph;
	ft_eat(philo);
	// ft_wait();
	return NULL;
}
void	ft_init_philo(t_philo *philo)
{
	int i;

	i = 0;
	pthread_mutex_init(philo->table->forks, NULL);
	philo->table->forks = (pthread_mutex_t*)malloc(philo->table->num_of_philo + 1);
	if (philo->table->forks == NULL)
		ft_errors("malloc error");
	while (i != philo->table->num_of_philo)
		pthread_mutex_init(&philo->table->forks[i++], NULL);
	while (i != philo->table->num_of_philo)
	{
		philo[i].my_num = i;
		philo[i].eat = philo->table->time_to_eat;

		philo[i].left_fork = &philo[i].left_fork[i];
		if (i == philo->table->num_of_philo)
			i = 0;
		philo[i].right_fork = &philo[i].right_fork[i + 1];
		i++;
	}
}

void	ft_start_philo()
{
	int 		i;
	t_table 	table;
	t_philo		philo[table.num_of_philo];
	pthread_t	t_philo[table.num_of_philo];

	i = 0;
	ft_init_philo(philo);
	while (i <= table.num_of_philo)
	{
		pthread_create(&t_philo[i], NULL, lifecycle, &philo[i]);
		pthread_join(t_philo[i], NULL);
		i++;
	}
}

int	main(int argc, char **argv)
{
	t_table		table;
	int			i;

	ft_parse(argc, argv, &table);
	ft_start_philo();
}