#ifndef PHILO_H
# define PHILO_H

typedef	struct 	s_input
{
	int			num_of_philo;
	int			time_to_die;
	int			time_to_eat;
	int			num_of_each_eats;
	int			all_time;
}				t_input;

#include <stdio.h>
#include <sys/time.h>

#endif