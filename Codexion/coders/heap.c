/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vabad-ro <vabad-ro@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/26 14:11:55 by vabad-ro          #+#    #+#             */
/*   Updated: 2026/10/26 17:28:42 by vabad-ro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	request_less(t_request a, t_request b)
{
	if (a.key < b.key)
		return (1);
	else if (a.key == b.key)
		if (a.seq < b.seq)
			return (1);
	return (0);
}

static void	sift_up(t_dongle *d, int i)
{
	int			padre;
	t_request	temp;

	while (i > 0)
	{
		padre = (i - 1) / 2;
		if (!request_less(d->heap[i], d->heap[padre]))
			break ;
		temp = d->heap[i];
		d->heap[i] = d->heap[padre];
		d->heap[padre] = temp;
		i = padre;
	}
}

static void	sift_down(t_dongle *dongle, int i)
{
	int			l_son;
	int			r_son;
	int			chosen;
	t_request	temp;

	l_son = (2 * i + 1);
	r_son = (2 * i + 2);
	while (l_son < dongle->heap_size)
	{
		chosen = l_son;
		if (r_son < dongle->heap_size
			&& request_less(dongle->heap[r_son], dongle->heap[l_son]))
			chosen = r_son;
		if (!request_less(dongle->heap[chosen], dongle->heap[i]))
			break ;
		temp = dongle->heap[i];
		dongle->heap[i] = dongle->heap[chosen];
		dongle->heap[chosen] = temp;
		i = chosen;
		l_son = (2 * i + 1);
		r_son = (2 * i + 2);
	}
}

void	heap_push(t_dongle *d, t_request r)
{
	if (d->heap_size >= d->heap_cap)
		return ;
	d->heap[d->heap_size] = r;
	d->heap_size++;
	sift_up(d, d->heap_size - 1);
}

t_request	heap_pop(t_dongle *d)
{
	t_request	top;

	top = d->heap[0];
	d->heap_size--;
	d->heap[0] = d->heap[d->heap_size];
	sift_down(d, 0);
	return (top);
}
