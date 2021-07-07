#ifndef PHILO_H
# define PHILO_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <pthread.h>

typedef	struct 			s_table
{
	int					num_of_philo;
	int					time_to_die;
	long				time_to_eat;
	int					time_to_sleep;
	int					num_of_each_eats;
	int					is_dead;
	long				birth;
	pthread_mutex_t		time;
	pthread_mutex_t		death;
	pthread_mutex_t		print;
	pthread_mutex_t		*forks;
}						t_table;

typedef struct 			s_philo
{
	pthread_mutex_t		*left_fork;
	pthread_mutex_t		*right_fork;
	int					count_eat;
	int					my_num;
	int					eat;
	long				last_lunch;
	int					*is_dead;
	t_table				*table;
}						t_philo;

void			ft_parse(int argc, char **argv, t_table *table);
int				check_long(int sign);
int				ft_atoi(const char *str);
int				ft_isdigit(int c);
void			ft_errors(char *str);
long			get_time(void);
void			ft_wait(int time_to_wait);
void			*lifecycle(void *a);
void			ft_start_philo();

#endif