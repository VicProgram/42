/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   s_mov.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vabad-ro <vabad-ro@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/20 14:15:31 by vabad-ro          #+#    #+#             */
/*   Updated: 2026/03/20 14:15:33 by vabad-ro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

void	sa_move(t_list **stack, t_count **count_list)
{
	t_list	*first;
	t_list	*second;

	if (!stack || !*stack || !(*stack)->next)
		return ;
	first = *stack;
	second = first->next;
	first->next = second->next;
	if (second->next)
		second->next->prev = first;
	second->next = first;
	second->prev = NULL;
	first->prev = second;
	*stack = second;
	(*count_list)->sa += 1;
	write(1, "sa\n", 3);
}

void	sb_move(t_list **stack, t_count **count_list)
{
	t_list	*first;
	t_list	*second;

	if (!stack || !*stack || !(*stack)->next)
		return ;
	first = *stack;
	second = first->next;
	first->next = second->next;
	if (second->next)
		second->next->prev = first;
	second->next = first;
	second->prev = NULL;
	first->prev = second;
	*stack = second;
	(*count_list)->sb += 1;
	write(1, "sb\n", 3);
}

void	ss_move(t_list **stack_a, t_list **stack_b, t_count **count_list)
{
	sa_move(stack_a, count_list);
	sb_move(stack_b, count_list);
	(*count_list)->sa -= 1;
	(*count_list)->sb -= 1;
	(*count_list)->ss += 1;
	write(1, "ss\n", 3);
}
