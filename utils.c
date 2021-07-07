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

void	ft_init(t_table *table)
{
	table->num_of_philo = 0;
	table->num_of_each_eats = -1;
	table->time_to_eat = 0;
	table->time_to_sleep = 0;
	table->time_to_die = 0;
	table->is_dead = 0;
	pthread_mutex_init(&table->time, NULL);
	pthread_mutex_init(&table->death, NULL);
	pthread_mutex_init(&table->print, NULL);
}
