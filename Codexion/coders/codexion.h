/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vabad-ro <vabad-ro@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/16 13:52:54 by vabad-ro          #+#    #+#             */
/*   Updated: 2026/10/28 16:53:01 by vabad-ro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <sys/time.h>
# include <pthread.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>

typedef struct s_sim		t_sim;
typedef struct s_dongle		t_dongle;
typedef struct s_coder		t_coder;

typedef struct s_sim
{
	int					stop;
	int					n_coders;
	int					compiles_required;
	int					comp_time;
	int					debug_time;
	int					refactor_time;
	int					time_to_burnout;
	int					dongle_cooldown;
	long				start_time;
	char				*scheduler;
	pthread_t			thread;
	pthread_mutex_t		stop_mutex;
	pthread_mutex_t		log_mutex;
	t_coder				*coders;
	t_dongle			*dongles;
}	t_sim;

typedef struct s_request
{
	t_coder	*coder;
	long	key;
	long	seq;
}	t_request;

typedef struct s_dongle
{
	pthread_mutex_t	mutex;
	pthread_cond_t	cond;
	t_request		*heap;
	int				is_free;
	int				cooldown_ms;
	int				heap_size;
	int				heap_cap;
	long			cooldown_until;
	long			arrival;
}	t_dongle;

typedef struct s_coder
{
	int			coder_id;
	int			compile_times;
	long		last_compile;
	t_sim		*sim;
	t_dongle	*left;
	t_dongle	*right;
	pthread_t	thread;
}	t_coder;

int			check_number(char **args);
int			check_scheduler(char **args);
int			check_range(t_sim *sim);
int			parse_args(t_sim *sim, char **args);
int			init(t_sim *sim);
int			sim_stopped(t_sim *sim);
int			handle_stop_after_take(t_coder *coder, t_dongle *dongle,
				t_request *req);

long		get_time_ms(void);

void		wait_until_ms(pthread_cond_t *cond, pthread_mutex_t *mutex,
				long ms);
void		wait_for_dongle(t_coder *coder, t_dongle *dongle);
void		run_sim(t_sim *sim);
void		fill_sim(t_sim *sim, char **args);
void		cleanup(t_sim *sim);
void		*coder_loop(void *arg);
void		join_threads(t_sim *sim, int n);
void		take_dongle(t_coder *coder, t_dongle *dongle);
void		release_dongle(t_dongle *dongle);
void		heap_push(t_dongle *dongle, t_request request);
void		*monitor_loop(void *arg);
void		print_log(t_sim *sim, int id, const char *msg);
void		give_back_dongle(t_dongle *dongle, t_request req);
void		wake_all(t_sim *sim);
t_request	heap_pop(t_dongle *dongle);

#endif
