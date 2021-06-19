#ifndef PHILO_H
# define PHILO_H

typedef	struct 	s_input
{
	int			num_of_philo;
	int			time_to_die;
	int			time_to_eat;
	int			time_to_sleep;
	int			num_of_each_eats;
	int			all_time;
}				t_input;

typedef struct 	s_table
{
	int			left_fork;
	int			rigth_fork;
}				t_table;

typedef struct 	s_philo
{
	t_table		table;
}				t_philo;

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>

#endif