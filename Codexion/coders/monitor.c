/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vabad-ro <vabad-ro@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/26 18:22:31 by vabad-ro          #+#    #+#             */
/*   Updated: 2026/10/28 16:53:03 by vabad-ro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	all_done(t_sim *sim)
{
	int		i;

	i = 0;
	while (i < sim->n_coders)
	{
		if (sim->coders[i].compile_times < sim->compiles_required)
			return (0);
		i++;
	}
	return (1);
}

static void	call_stop(t_sim *sim)
{
	pthread_mutex_lock(&sim->stop_mutex);
	sim->stop = 1;
	pthread_mutex_unlock(&sim->stop_mutex);
}

static int	check_burnout(t_sim *sim)
{
	int		i;
	long	now;

	i = 0;
	now = get_time_ms();
	while (i < sim->n_coders)
	{
		if (sim->coders[i].compile_times < sim->compiles_required
			&& now >= sim->coders[i].last_compile + sim->time_to_burnout)
		{
			pthread_mutex_lock(&sim->log_mutex);
			printf(
				"%ld %d burned out\n", get_time_ms() - sim->start_time,
				sim->coders[i].coder_id);
			pthread_mutex_unlock(&sim->log_mutex);
			return (1);
		}
		i++;
	}
	return (0);
}

int	sim_stopped(t_sim *sim)
{
	int	stopped;

	pthread_mutex_lock(&sim->stop_mutex);
	stopped = sim->stop;
	pthread_mutex_unlock(&sim->stop_mutex);
	return (stopped);
}

void	*monitor_loop(void *arg)
{
	t_sim	*sim;

	sim = arg;
	while (1)
	{
		if (all_done(sim))
		{
			call_stop(sim);
			wake_all(sim);
			break ;
		}
		if (check_burnout(sim))
		{
			call_stop(sim);
			wake_all(sim);
			break ;
		}
		usleep(1000);
	}
	return (NULL);
}
