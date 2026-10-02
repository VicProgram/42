/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils4.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vabad-ro <vabad-ro@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/20 14:16:03 by vabad-ro          #+#    #+#             */
/*   Updated: 2026/03/20 14:16:09 by vabad-ro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

int	find_min_index(t_list *stack_a)
{
	int		min;
	t_list	*temp;

	min = find_max(stack_a);
	temp = stack_a;
	while (temp)
	{
		if (temp->index == -1 && temp->nb < min)
			min = temp->nb;
		temp = temp->next;
	}
	return (min);
}

int	short_way_medium(t_list **stack, int i, int range)
{
	int		count;
	t_list	*temp;

	count = 0;
	temp = *stack;
	while (temp)
	{
		if (i <= temp->index && temp->index < range)
			return (count);
		count++;
		temp = temp->next;
	}
	return (-1);
}

int	short_way_simple_adj(t_list **stack)
{
	int		max;
	int		count;
	t_list	*temp;

	count = 0;
	temp = *stack;
	max = find_max(*stack);
	while (temp->nb != max)
	{
		count++;
		temp = temp->next;
	}
	return (count);
}

int	argv_isnumber(char *argv)
{
	int	i;

	i = 0;
	if (!argv[i])
		return (0);
	if (argv[i] == '-' || argv[i] == '+')
		i++;
	if (!argv[i])
		return (0);
	while (argv[i])
	{
		if (!ft_isdigit(argv[i]) && argv[i] != ' ')
			return (0);
		i++;
	}
	return (1);
}

int	not_equal_number(t_list *stack_a, int new_number)
{
	t_list	*current;

	current = stack_a;
	while (current)
	{
		if (current->nb == new_number)
			return (0);
		current = current->next;
	}
	return (1);
}
