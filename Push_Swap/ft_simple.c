/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_simple.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vabad-ro <vabad-ro@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/20 14:14:31 by vabad-ro          #+#    #+#             */
/*   Updated: 2026/03/20 14:14:33 by vabad-ro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static int	short_way_simple(t_list **stack)
{
	int		min;
	int		count;
	t_list	*temp;

	count = 0;
	temp = *stack;
	min = find_min(*stack);
	while (temp->nb != min)
	{
		count++;
		temp = temp->next;
	}
	return (count);
}

static void	push_to_b(t_list **stack_a, t_list **stack_b,
			t_count **count_list, int size)
{
	int	count;

	count = short_way_simple(stack_a);
	if (count < (size / 2))
	{
		while (count > 0)
		{
			ra_move(stack_a, count_list);
			count--;
		}
	}
	else
	{
		count = size - count;
		while (count > 0)
		{
			rra_move(stack_a, count_list);
			count--;
		}
	}
	if (stack_size(*stack_a) > 2)
		pb_move(stack_a, stack_b, count_list);
}

void	ft_simple(t_list **stack_a, t_count **count_list)
{
	t_list	*stack_b;
	int		size;

	size = stack_size(*stack_a);
	stack_b = NULL;
	if (stack_size(*stack_a) <= 3)
	{
		tiny_sort(stack_a, count_list);
		return ;
	}
	while (size > 1)
	{
		push_to_b(stack_a, &stack_b, count_list, size);
		size--;
	}
	while (stack_b)
		pa_move(stack_a, &stack_b, count_list);
}
