/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vabad-ro <vabad-ro@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/20 19:04:39 by vabad-ro          #+#    #+#             */
/*   Updated: 2026/10/28 16:50:42 by vabad-ro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	take_dongle(t_coder *coder, t_dongle *dongle)
{
	t_request	req;

	pthread_mutex_lock(&dongle->mutex);
	req.coder = coder;
	req.seq = dongle->arrival++;
	if (strcmp(coder->sim->scheduler, "edf") == 0)
		req.key = coder->last_compile + coder->sim->time_to_burnout;
	else
		req.key = req.seq;
	heap_push(dongle, req);
	wait_for_dongle(coder, dongle);
	if (sim_stopped(coder->sim))
	{
		pthread_mutex_unlock(&dongle->mutex);
		return ;
	}
	heap_pop(dongle);
	if (handle_stop_after_take(coder, dongle, &req))
	{
		pthread_mutex_unlock(&dongle->mutex);
		return ;
	}
	dongle->is_free = 0;
	print_log(coder->sim, coder->coder_id, "has taken a dongle");
	pthread_mutex_unlock(&dongle->mutex);
}

static void	acquire_dongles(t_coder *coder)
{
	t_dongle	*first;
	t_dongle	*second;

	first = coder->left;
	second = coder->right;
	take_dongle(coder, first);
	if (first != second)
		take_dongle(coder, second);
}

void	release_dongle(t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->mutex);
	dongle->cooldown_until = get_time_ms() + dongle->cooldown_ms;
	dongle->is_free = 1;
	pthread_cond_broadcast(&dongle->cond);
	pthread_mutex_unlock(&dongle->mutex);
}

static void	release_both(t_coder *c)
{
	release_dongle(c->left);
	if (c->left != c->right)
		release_dongle(c->right);
}

void	*coder_loop(void *arg)
{
	t_coder	*c;

	c = arg;
	while (!sim_stopped(c->sim) && c->compile_times < c->sim->compiles_required)
	{
		acquire_dongles(c);
		if (sim_stopped(c->sim))
		{
			release_both(c);
			return (NULL);
		}
		print_log(c->sim, c->coder_id, "is compiling");
		usleep(c->sim->comp_time * 1000);
		print_log(c->sim, c->coder_id, "is debugging");
		usleep(c->sim->debug_time * 1000);
		print_log(c->sim, c->coder_id, "is refactoring");
		usleep(c->sim->refactor_time * 1000);
		c->compile_times++;
		c->last_compile = get_time_ms();
		release_both(c);
	}
	return (NULL);
}
