/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_medium.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vabad-ro <vabad-ro@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/20 14:14:07 by vabad-ro          #+#    #+#             */
/*   Updated: 2026/03/20 14:14:11 by vabad-ro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

void	rotate(t_list **stack_a, t_count **count_list,
				int count, int size)
{
	if (count > size / 2)
	{
		count = size - count;
		while (count-- > 0)
			rra_move(stack_a, count_list);
	}
	else
	{
		while (count-- > 0)
			ra_move(stack_a, count_list);
	}
}

void	ft_bucket(int chunk, t_list **stack_a, t_list **stack_b,
		t_count **count_list)
{
	int	size;
	int	count;
	int	range;

	size = stack_size(*stack_a);
	range = size;
	while (size > 0)
	{
		range -= chunk;
		count = short_way_medium(stack_a, range, range + chunk);
		while (count != -1)
		{
			count = short_way_medium(stack_a, range, range + chunk);
			rotate(stack_a, count_list, count, size);
			pb_move(stack_a, stack_b, count_list);
		}
		size = stack_size(*stack_a);
	}
}

static void	ft_simple_adj(t_list **stack_a, t_list **stack_b,
		t_count **count_list)
{
	int	size;
	int	count;

	size = stack_size(*stack_b);
	while (size > 0)
	{
		count = short_way_simple_adj(stack_b);
		if (count <= (size / 2))
		{
			while (count-- > 0)
				rb_move(stack_b, count_list);
		}
		else
		{
			count = size - count;
			while (count-- > 0)
				rrb_move(stack_b, count_list);
		}
		pa_move(stack_a, stack_b, count_list);
		size--;
	}
}

void	ft_medium(t_list **stack_a, t_count **count_list)
{
	t_list	*stack_b;
	int		chunk;

	chunk = ft_sqrt(stack_size(*stack_a));
	stack_b = NULL;
	if (stack_size(*stack_a) <= 5)
	{
		tiny_sort(stack_a, count_list);
		return ;
	}
	assign_index(stack_a);
	ft_bucket(chunk, stack_a, &stack_b, count_list);
	ft_simple_adj(stack_a, &stack_b, count_list);
}
