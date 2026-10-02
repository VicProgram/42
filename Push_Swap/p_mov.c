/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_mov.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vabad-ro <vabad-ro@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/20 14:14:52 by vabad-ro          #+#    #+#             */
/*   Updated: 2026/03/20 14:14:53 by vabad-ro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

void	pa_move(t_list **stack_a, t_list **stack_b, t_count **count_list)
{
	t_list	*temp;

	if (!stack_b || !*stack_b)
		return ;
	temp = *stack_b;
	*stack_b = temp->next;
	if (*stack_b)
		(*stack_b)->prev = NULL;
	temp->next = *stack_a;
	if (*stack_a)
		(*stack_a)->prev = temp;
	*stack_a = temp;
	(*stack_a)->prev = NULL;
	(*count_list)->pa += 1;
	write(1, "pa\n", 3);
}

void	pb_move(t_list **stack_a, t_list **stack_b, t_count **count_list)
{
	t_list	*temp;

	if (!stack_a || !*stack_a)
		return ;
	temp = *stack_a;
	*stack_a = temp->next;
	if (*stack_a)
		(*stack_a)->prev = NULL;
	temp->next = *stack_b;
	if (*stack_b)
		(*stack_b)->prev = temp;
	*stack_b = temp;
	(*stack_b)->prev = NULL;
	(*count_list)->pb += 1;
	write(1, "pb\n", 3);
}
