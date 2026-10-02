/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vabad-ro <vabad-ro@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/20 14:15:59 by vabad-ro          #+#    #+#             */
/*   Updated: 2026/03/20 14:16:00 by vabad-ro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

int	stack_size(t_list *stack_a)
{
	int	size;

	size = 0;
	while (stack_a)
	{
		size++;
		stack_a = stack_a -> next;
	}
	return (size);
}

int	ft_sqrt(int nb)
{
	int	i;

	i = 1;
	if (nb < 0)
		return (0);
	while (i * i < nb)
		i++;
	return (i);
}

int	find_min(t_list *stack_a)
{
	int	min;

	min = stack_a->nb;
	while (stack_a)
	{
		if (stack_a->nb < min)
			min = stack_a->nb;
		stack_a = stack_a -> next;
	}
	return (min);
}

int	find_max(t_list *stack_a)
{
	int	max;

	max = stack_a->nb;
	while (stack_a)
	{
		if (stack_a->nb > max)
			max = stack_a->nb;
		stack_a = stack_a -> next;
	}
	return (max);
}

void	initialize_list(t_count **count_list)
{
	(*count_list)->sa = 0;
	(*count_list)->sb = 0;
	(*count_list)->ss = 0;
	(*count_list)->pa = 0;
	(*count_list)->pb = 0;
	(*count_list)->ra = 0;
	(*count_list)->rb = 0;
	(*count_list)->rr = 0;
	(*count_list)->rra = 0;
	(*count_list)->rrb = 0;
	(*count_list)->rrr = 0;
	(*count_list)->bench_active = 0;
}
