#include "philo.h"

int				check_long(int sign)
{
	if (sign == -1)
		return (0);
	return (-1);
}

int				ft_atoi(const char *str)
{
	int			i;
	long long	res;
	long long	tmp;
	int			sign;

	sign = 1;
	i = 0;
	res = 0;
	while (str[i] == 32 || (str[i] > 8 && str[i] < 14))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i++] == '-')
			sign = -1;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		tmp = res;
		res = res * 10 + str[i] - '0';
		if (tmp > res && i != 0)
			return (check_long(sign));
		i++;
	}
	res = res * sign;
	return ((int)res);
}

int	ft_isdigit(int c)
{
	if ((c >= '0' && c <= '9') || (c == ' '))
		return (1);
	return (0);
}

void	ft_errors(char *str)
{
	printf("%s\n", str);
	exit(0);
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

void	*lifecycle(void *args)
{}

void	wait()
{
	struct timeval start;
	gettimeofday(&start, NULL);
}
int	main(int argc, char **argv)
{
	t_table table;
	// t_philo philo;
	pthread_t *philo;

	int			i;

	ft_parse(argc, argv, &table);
	table.forks = (int*)malloc(table.num_of_philo + 1);
	if (table.forks == NULL)
		ft_errors("malloc error");
	philo = (pthread_t*)malloc(table.num_of_philo + 1);
	if (table.forks == NULL)
		ft_errors("malloc error");
	while (i <= table.num_of_philo)
	{
		pthread_create(&philo[i], NULL, lifecycle, NULL);
		pthread_join(philo[i], NULL);
		i++;
	}
}