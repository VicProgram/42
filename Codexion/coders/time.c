/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vabad-ro <vabad-ro@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/22 14:14:33 by vabad-ro          #+#    #+#             */
/*   Updated: 2026/10/23 11:39:08 by vabad-ro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000L + tv.tv_usec / 1000L);
}

static void	ms_to_timespec(long ms, struct timespec *ts)
{
	ts->tv_sec = ms / 1000L;
	ts->tv_nsec = (ms % 1000L) * 1000000L;
}

void	wait_until_ms(pthread_cond_t *cond, pthread_mutex_t *mutex, long ms)
{
	struct timespec	ts;

	ms_to_timespec(ms, &ts);
	pthread_cond_timedwait(cond, mutex, &ts);
}
