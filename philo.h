#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <pthread.h>

typedef struct s_table
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

typedef struct s_philo
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

void	give_forks(t_table *table, t_philo *philo);
void	ft_parse(int argc, char **argv, t_table *table);
void	clear(t_table *table);
long	get_time(void);
void	ft_wait(int time_to_wait);
void	print_message(char *str, long time, int num, t_philo *philo);
void	*check_death(void *ph);
void	ft_init_philo(t_philo *philo, t_table *table);
void	ft_start_philo(t_table 	*table);
int		check_long(int sign);
int		ft_atoi(const char *str);
void	ft_errors(char *str);
void	ft_init(t_table *table);
void	ft_eat(t_philo *philo);
void	ft_sleep(t_philo *philo);
void	ft_wait(int time_to_wait);
void	ft_think(t_philo *philo);
void	*lifecycle(void *ph);

#endif