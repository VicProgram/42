/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_adaptive.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vabad-ro <vabad-ro@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/20 14:13:37 by vabad-ro          #+#    #+#             */
/*   Updated: 2026/03/20 14:13:42 by vabad-ro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

float	ft_compute_disorder(t_list *stack)
{
	t_list	*temp_i;
	t_list	*temp_j;
	float	mistakes;
	float	total_pairs;

	if (!stack || !stack->next)
		return (0);
	mistakes = 0;
	total_pairs = 0;
	temp_i = stack;
	while (temp_i != NULL)
	{
		temp_j = temp_i->next;
		while (temp_j != NULL)
		{
			total_pairs += 1;
			if (temp_i->nb > temp_j->nb)
				mistakes += 1;
			temp_j = temp_j->next;
		}
		temp_i = temp_i->next;
	}
	return (mistakes / total_pairs);
}

void	ft_adaptive(t_list **stack_a, t_count **count_list)
{
	if (stack_size(*stack_a) <= 3)
	{
		tiny_sort(stack_a, count_list);
		return ;
	}
	if (0.0 < (*count_list)->disorder && (*count_list)->disorder < 0.2)
		ft_simple(stack_a, count_list);
	else if (0.2 <= (*count_list)->disorder && (*count_list)->disorder <= 0.5)
		ft_medium(stack_a, count_list);
	else if (0.5 < (*count_list)->disorder && (*count_list)->disorder <= 1.0)
		ft_complex(stack_a, count_list);
	else if ((*count_list)->disorder == 0.0)
		return ;
	else
	{
		write (2, "Error\n", 6);
		ft_lstclear(stack_a);
	}
}
