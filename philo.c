#include "philo.h"

void	ft_errors(char *str)
{
	printf("%s\n", str);
}

void	ft_parse(int argc, char **argv, t_input *args)
{
	int i;

	i = 0;
	if (argc < 5 || argc > 6)
		ft_errors("wrong count of arguments");
	
}

int	main(int argc, char **argv)
{
	t_input args;
	ft_parse(argc, argv, &args);
}