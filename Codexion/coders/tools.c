/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vabad-ro <vabad-ro@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/27 11:59:02 by vabad-ro          #+#    #+#             */
/*   Updated: 2026/10/28 16:56:39 by vabad-ro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	print_log(t_sim	*sim, int id, const char*msg)
{
	pthread_mutex_lock(&sim->log_mutex);
	printf("%ld %d %s\n", get_time_ms() - sim->start_time, id, msg);
	pthread_mutex_unlock(&sim->log_mutex);
}

void	give_back_dongle(t_dongle	*dongle, t_request	req)
{
	dongle->is_free = 1;
	heap_push(dongle, req);
	pthread_cond_broadcast(&dongle->cond);
	pthread_mutex_unlock(&dongle->mutex);
}

int	handle_stop_after_take(t_coder *coder, t_dongle *dongle, t_request *req)
{
	if (!sim_stopped(coder->sim))
		return (0);
	dongle->is_free = 1;
	heap_push(dongle, *req);
	pthread_cond_broadcast(&dongle->cond);
	pthread_mutex_unlock(&dongle->mutex);
	return (1);
}

void	wait_for_dongle(t_coder *coder, t_dongle *dongle)
{
	while (dongle->is_free == 0
		|| dongle->heap[0].coder != coder
		|| get_time_ms() < dongle->cooldown_until)
	{
		if (sim_stopped(coder->sim))
		{
		 	pthread_mutex_unlock(&dongle->mutex);
		 	return ;
		}
		if (dongle->heap[0].coder != coder || dongle->is_free == 0)
			pthread_cond_wait(&dongle->cond, &dongle->mutex);
		else
			wait_until_ms(&dongle->cond, &dongle->mutex,
				dongle->cooldown_until);
		if (sim_stopped(coder->sim))
		{
			pthread_mutex_unlock(&dongle->mutex);
			return ;
		}
	}
}

void	wake_all(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->n_coders)
	{
		pthread_mutex_lock(&sim->dongles[i].mutex);
		pthread_cond_broadcast(&sim->dongles[i].cond);
		pthread_mutex_unlock(&sim->dongles[i].mutex);
		i++;
	}
}
