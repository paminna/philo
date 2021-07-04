#include "philo.h"

void	ft_init(t_table *table)
{
	table->num_of_philo = 0;
	table->num_of_each_eats = 0;
	table->time_to_eat = 0;
	table->time_to_sleep = 0;
	table->time_to_die = 0;
	table->all_time = 0;
}

void	print_table(t_table *table)
{
	int i = 0;

	while (i < table->num_of_philo)
	{
		printf("[%p] ", &table->forks[i]);
		i++;
	}
	printf("\n");
}

void	print_philo(t_philo *philo)
{
	printf("%d fork L = [%p] ", philo->my_num, philo->left_fork);
	printf("fork R = [%p]\n", philo->right_fork);
}

void	give_forks(t_table *table, t_philo *philo)
{
	int i;

	i = 0;
	table->forks = (pthread_mutex_t*)malloc(table->num_of_philo * sizeof(pthread_mutex_t));
	if (table->forks == NULL)
		ft_errors("malloc error");
	// print_table(table);
	while (i < table->num_of_philo)
	{
		philo[i].right_fork = &table->forks[i];
		if (i == 0)
			philo[i].left_fork = &table->forks[table->num_of_philo - 1];
		philo[i].left_fork = &table->forks[i - 1];
		// print_philo(&philo[i]);
		i++;
	}
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

	philo = ph;
	ft_eat(philo);
	return NULL;
}
void	ft_init_philo(t_philo *philo, t_table *table)
{
	int i;
	int j;

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
		philo[i].my_num = i;
		philo[i].eat = table->time_to_eat;
		philo[i].left_fork = &philo[i].left_fork[j];
		if (j == table->num_of_philo)
			j = -1;
		philo[i].right_fork = &philo[i].right_fork[j + 1];
		i++;
		j++;
	}
	// i = 0;
	// while (i < table->num_of_philo)
	// 	print_philo(&philo[i++]);
}

void	ft_start_philo(t_table 	*table)
{
	int 		i;
	t_philo		philo[table->num_of_philo];
	pthread_t	t_philo[table->num_of_philo];

	i = 0;
	ft_init_philo(philo, table);
	printf("num of philo %d\n", table->num_of_philo);
	while (i <= table->num_of_philo)
	{
		pthread_create(&t_philo[i], NULL, lifecycle, &philo[i]);
		pthread_join(t_philo[i], NULL);
		i++;
	}
}

int	main(int argc, char **argv)
{
	t_table		table;

	ft_parse(argc, argv, &table);
	ft_start_philo(&table);
}