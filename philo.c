#include "philo.h"

size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i] != '\0')
		i++;
	return (i);
}

void	ft_init(t_table *table)
{
	table->num_of_philo = 0;
	table->num_of_each_eats = 0;
	table->time_to_eat = 0;
	table->time_to_sleep = 0;
	table->time_to_die = 0;
	table->is_dead = 0;
	pthread_mutex_init(&table->time, NULL);
	pthread_mutex_init(&table->death, NULL);
	pthread_mutex_init(&table->print, NULL);
}

// void	print_table(t_table *table)
// {
// 	int i = 0;

// 	while (i < table->num_of_philo)
// 	{
// 		printf("[%p] ", &table->forks[i]);
// 		i++;
// 	}
// 	printf("\n");
// }

// void	print_philo(t_philo *philo)
// {
// 	printf("%d fork L = [%p] ", philo->my_num, philo->left_fork);
// 	printf("fork R = [%p]\n", philo->right_fork);
// }

void	give_forks(t_table *table, t_philo *philo)
{
	int i;

	i = 0;
	table->forks = (pthread_mutex_t*)malloc(table->num_of_philo * sizeof(pthread_mutex_t));
	if (table->forks == NULL)
		ft_errors("malloc error");
	while (i < table->num_of_philo)
	{
		philo[i].right_fork = &table->forks[(i + 1) % table->num_of_philo];
		philo[i].left_fork = &table->forks[(i - 1 + table->num_of_philo) % table->num_of_philo];
		i++;
	}
}

void	ft_parse(int argc, char **argv, t_table *table)
{
	int i;
	int j;

	i = 1;
	j = 0;
	ft_init(table);
	if (argc < 5 || argc > 6)
		ft_errors("wrong amount of arguments");
	while (argv[i])
	{
		if (i == 1 && ft_atoi(argv[i]) > 2)
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
	int i;

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
	struct timeval cur;

	gettimeofday(&cur, NULL);
	return((long)((cur.tv_sec * 1000) + (cur.tv_usec / 1000)));
}

void	ft_wait(int time_to_wait)
{
	time_t t_start;
	long t_end;

	t_start = get_time();
	t_end = t_start + time_to_wait;
	while (t_start < t_end)
	{
		t_start = get_time();
		usleep(10);
	}
}

void	print_message(char *str, long time, int num, t_philo *philo)
{
	pthread_mutex_lock(&philo->table->print);
	if (!philo->table->is_dead)
		printf("%ld %d %s\n", get_time() - time, num, str);
	pthread_mutex_unlock(&philo->table->print);
}

void	ft_sleep(t_philo *philo)
{
	print_message("is sleeping", philo->table->birth, philo->my_num, philo);
	pthread_mutex_lock(&philo->table->time);
	ft_wait(philo->table->time_to_sleep);
	pthread_mutex_unlock(&philo->table->time);
}

void	ft_eat(t_philo *philo)
{
	if (philo->my_num % 2 == 0)
	{
		pthread_mutex_lock(philo->left_fork);
		print_message("has taken a fork", philo->table->birth, philo->my_num, philo);
		pthread_mutex_lock(philo->right_fork);
		print_message("has taken a fork", philo->table->birth, philo->my_num, philo);
	}
	else
	{
		pthread_mutex_lock(philo->right_fork);
		print_message("has taken a fork", philo->table->birth, philo->my_num, philo);
		pthread_mutex_lock(philo->left_fork);
		print_message("has taken a fork", philo->table->birth, philo->my_num, philo);

	}
	print_message("is eating", philo->table->birth, philo->my_num, philo);
	pthread_mutex_lock(&philo->table->time);
	philo->last_lunch = get_time();
	ft_wait(philo->table->time_to_eat);
	pthread_mutex_unlock(&philo->table->time);
	pthread_mutex_unlock(philo->right_fork);
	pthread_mutex_unlock(philo->left_fork);
}

void	*check_death(void *ph)
{
	t_philo *philo;

	philo = ph;
	while (1)
	{
		pthread_mutex_lock(&philo->table->death);
		if (get_time() - philo->last_lunch > philo->table->time_to_die)
			break;
		pthread_mutex_unlock(&philo->table->death);
	}
	print_message("died", philo->table->birth, philo->my_num, philo);
	*philo->is_dead = 1;
	pthread_mutex_unlock(&philo->table->death);
	return(NULL);
}

void	ft_think(t_philo *philo)
{
	print_message("is thinking", philo->table->birth, philo->my_num, philo);
}

void	*lifecycle(void *ph)
{
	int i;
	t_philo *philo;
	pthread_t	death;

	philo = ph;
	philo->last_lunch = get_time();
	philo->table->birth = get_time();
	pthread_create(&death, NULL, check_death, philo);
	while (!philo->table->is_dead)
	{
		ft_eat(philo);
		ft_sleep(philo);
		ft_think(philo);
	}
	pthread_join(death, NULL);
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
		philo[i].is_dead = 0;
		philo[i].eat = table->time_to_eat;
		philo[i].table = table;
		philo[i].is_dead = &table->is_dead;
		i++;
	}
}

void	ft_start_philo(t_table 	*table)
{
	int 		i;
	t_philo		philo[table->num_of_philo];
	pthread_t	t_philo[table->num_of_philo];

	i = 0;
	ft_init_philo(philo, table);
	while (i < table->num_of_philo)
	{
		pthread_create(&t_philo[i], NULL, lifecycle, &philo[i]);
		i++;
	}
	i = 0;
	while (i < table->num_of_philo)
	{
		pthread_join(t_philo[i], NULL);
		i++;
	}
	clear(table);
}

int	main(int argc, char **argv)
{
	t_table		table;
	
	ft_parse(argc, argv, &table);
	ft_start_philo(&table);
	return (0);
}