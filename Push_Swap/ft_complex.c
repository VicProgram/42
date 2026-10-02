/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_complex.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vabad-ro <vabad-ro@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/20 14:14:00 by vabad-ro          #+#    #+#             */
/*   Updated: 2026/03/20 14:14:03 by vabad-ro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static void	index_reset(t_list **stack_a, int size)
{
	t_list	*curr;
	int		i;

	i = 0;
	while (i < size)
	{
		curr = *stack_a;
		while (curr)
		{
			if (curr->nb == find_min_index(*stack_a))
			{
				curr->index = i;
				break ;
			}
			curr = curr->next;
		}
		i++;
	}
}

void	assign_index(t_list **stack_a)
{
	t_list	*curr;

	curr = *stack_a;
	while (curr)
	{
		curr->index = -1;
		curr = curr->next;
	}
	index_reset(stack_a, stack_size(*stack_a));
}

int	ft_binmax(t_list *stack_a)
{
	int	index_max;
	int	bin_len;

	index_max = stack_size(stack_a) - 1;
	bin_len = 1;
	while (index_max > 0)
	{
		index_max /= 2;
		bin_len++;
	}
	return (bin_len);
}

static void	binary_pass(t_list **stack_a, t_list **stack_b,
				t_count **count_list, int i)
{
	int	size_a;
	int	size_b;

	size_a = stack_size(*stack_a);
	while (size_a > 0)
	{
		if (!(((*stack_a)->index >> i) & 1))
			pb_move(stack_a, stack_b, count_list);
		else
			ra_move(stack_a, count_list);
		size_a--;
	}
	size_b = stack_size(*stack_b);
	while (size_b > 0)
	{
		pa_move(stack_a, stack_b, count_list);
		size_b--;
	}
}

void	ft_complex(t_list **stack_a, t_count **count_list)
{
	t_list	*stack_b;
	int		i;

	stack_b = NULL;
	if (stack_size(*stack_a) <= 5)
	{
		tiny_sort(stack_a, count_list);
		return ;
	}
	assign_index(stack_a);
	i = 0;
	while (i < ft_binmax(*stack_a))
	{
		binary_pass(stack_a, &stack_b, count_list, i);
		i++;
	}
}
