/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rr_move.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vabad-ro <vabad-ro@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/20 14:15:26 by vabad-ro          #+#    #+#             */
/*   Updated: 2026/03/20 14:15:28 by vabad-ro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

void	rra_move(t_list **stack, t_count **count_list)
{
	t_list	*last;

	if (!stack || !*stack || !(*stack)->next)
		return ;
	last = ft_lstlast(*stack);
	last -> prev -> next = NULL;
	last -> next = *stack;
	last -> prev = NULL;
	(*stack)-> prev = last;
	*stack = last;
	(*count_list)->rra += 1;
	write(1, "rra\n", 4);
}

void	rrb_move(t_list **stack, t_count **count_list)
{
	t_list	*last;

	if (!stack || !*stack || !(*stack)->next)
		return ;
	last = ft_lstlast(*stack);
	last->prev->next = NULL;
	last->next = *stack;
	last->prev = NULL;
	(*stack)->prev = last;
	*stack = last;
	(*count_list)->rrb += 1;
	write(1, "rrb\n", 4);
}

void	rrr_move(t_list **stack_a, t_list **stack_b, t_count **count_list)
{
	rra_move(stack_a, count_list);
	rrb_move(stack_b, count_list);
	(*count_list)->rra -= 1;
	(*count_list)->rrb -= 1;
	(*count_list)->rrr += 1;
	write(1, "rrr\n", 4);
}
