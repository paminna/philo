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
	// print_table(table);
	while (i < table->num_of_philo)
	{
		philo[i].right_fork = &table->forks[i];
		if (i == 0)
			philo[i].left_fork = &table->forks[table->num_of_philo - 1];
		else
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

void	clear(t_philo *philo)
{
	int i;

	i = 0;
	printf("died");
	while (i < philo->table->num_of_philo)
	{
		pthread_mutex_destroy(&philo->table->forks[i]);
		i++;
	}
	free(philo->table->forks);
	pthread_mutex_destroy(&philo->table->print);
	pthread_mutex_destroy(&philo->table->death);
	pthread_mutex_destroy(&philo->table->time);
}

long	get_time(void)
{
	struct timeval cur;

	gettimeofday(&cur, NULL);
	return((long)((cur.tv_sec * 1000) + (cur.tv_usec / 1000)));
}

void	ft_wait(int time_to_wait)
{
	long t_start;
	long t_end;

	// usleep(time_to_wait * 1000);
	t_start = get_time();
	t_end = t_start + time_to_wait;
	while (t_start < t_end)
	{
		t_start += get_time() - t_start;
		usleep(10);
	}
}

void	print_message(char *str, long time, int num, t_philo *philo)
{
	pthread_mutex_lock(&philo->table->print);
	printf("%ld %d %s\n", time, num, str);
	pthread_mutex_unlock(&philo->table->print);
}

void	ft_sleep(t_philo *philo)
{
	// pthread_mutex_lock(&philo->table->print);
	// printf("%ld %d is sleeping\n", get_time() - philo->table->birth, philo->my_num);
	// pthread_mutex_unlock(&philo->table->print);
	print_message("is sleeping", get_time() - philo->table->birth, philo->my_num, philo);
	pthread_mutex_lock(&philo->table->print);
	ft_wait(philo->table->time_to_sleep);
	pthread_mutex_unlock(&philo->table->print);
}

void	ft_eat(t_philo *philo)
{
	if (philo->my_num % 2)
	{
		pthread_mutex_lock(philo->left_fork);
		// pthread_mutex_lock(&philo->table->print);
		// printf("%ld %d philo has taken fork\n", get_time() - philo->table->birth, philo->my_num);
		// pthread_mutex_unlock(&philo->table->print);
		print_message("has taken a fork", get_time() - philo->table->birth, philo->my_num, philo);
		pthread_mutex_lock(philo->right_fork);
		// pthread_mutex_lock(&philo->table->print);
		// printf("%ld %d philo has taken fork\n", get_time() - philo->table->birth, philo->my_num);
		// pthread_mutex_unlock(&philo->table->print);
		print_message("has taken a fork", get_time() - philo->table->birth, philo->my_num, philo);
	}
	else
	{
		pthread_mutex_lock(philo->right_fork);
		// pthread_mutex_lock(&philo->table->print);
		// printf("%ld %d philo has taken fork\n", get_time() - philo->table->birth, philo->my_num);
		// pthread_mutex_unlock(&philo->table->print);
		print_message("has taken a fork", get_time() - philo->table->birth, philo->my_num, philo);
		pthread_mutex_lock(philo->left_fork);
		// pthread_mutex_lock(&philo->table->print);
		// printf("%ld %d philo has taken fork\n", get_time() - philo->table->birth, philo->my_num);
		// pthread_mutex_unlock(&philo->table->print);
		print_message("has taken a fork", get_time() - philo->table->birth, philo->my_num, philo);

	}
	print_message("is eating", get_time() - philo->table->birth, philo->my_num, philo);
	pthread_mutex_lock(&philo->table->time);
	ft_wait(philo->table->time_to_eat);
	pthread_mutex_unlock(&philo->table->time);
	pthread_mutex_lock(&philo->table->time);
	philo->last_lunch = get_time();
	pthread_mutex_unlock(&philo->table->time);
	if (philo->my_num % 2)
	{
		pthread_mutex_unlock(philo->left_fork);
		// pthread_mutex_lock(&philo->table->print);
		// printf("%ld %d philo put fork\n", get_time() - philo->table->birth, philo->my_num);
		// pthread_mutex_unlock(&philo->table->print);
		print_message("put fork", get_time() - philo->table->birth, philo->my_num, philo);
		pthread_mutex_unlock(philo->right_fork);
		// pthread_mutex_lock(&philo->table->print);
		// printf("%ld %d philo put fork\n", get_time() - philo->table->birth, philo->my_num);
		// pthread_mutex_unlock(&philo->table->print);
		print_message("put fork", get_time() - philo->table->birth, philo->my_num, philo);
	}
	else
	{
		pthread_mutex_unlock(philo->right_fork);
		// pthread_mutex_lock(&philo->table->print);
		// printf("%ld %d philo put fork\n", get_time() - philo->table->birth, philo->my_num);
		// pthread_mutex_unlock(&philo->table->print);
		print_message("put fork", get_time() - philo->table->birth, philo->my_num, philo);
		pthread_mutex_unlock(philo->left_fork);
		// pthread_mutex_lock(&philo->table->print);
		// printf("%ld %d philo put fork\n", get_time() - philo->table->birth, philo->my_num);
		// pthread_mutex_unlock(&philo->table->print);
		print_message("put fork", get_time() - philo->table->birth, philo->my_num, philo);
	}
}

void	*check_death(void *ph)
{
	t_philo *philo;

	philo = ph;
	while (1)
	{
		pthread_mutex_lock(&philo->table->time);
		if (get_time() - philo->last_lunch > philo->table->time_to_die)
		{
			philo->is_dead = 1;
			printf("time1 %ld\n time2 %d\n", get_time() - philo->last_lunch , philo->table->time_to_die);
			printf("%d philo died\n", philo->my_num);
			break;
		}
		// pthread_mutex_lock(&philo->table->print); 
		// pthread_mutex_unlock(&philo->table->print);
	}
	return(NULL);
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
	while (philo->is_dead == 0)
	{
		ft_eat(philo);
		ft_sleep(philo);
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
		philo[i].left_fork = &philo[i].left_fork[j];
		philo[i].table = table;
		if (j == table->num_of_philo)
			j = -1;
		philo[i].right_fork = &philo[i].right_fork[j + 1];
		i++;
		j++;
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
	clear(philo);
}

int	main(int argc, char **argv)
{
	t_table		table;
	
	ft_parse(argc, argv, &table);
	ft_start_philo(&table);
}