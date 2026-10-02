/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arg_control.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vabad-ro <vabad-ro@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/20 14:13:19 by vabad-ro          #+#    #+#             */
/*   Updated: 2026/03/20 14:13:23 by vabad-ro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static int	validate_num(t_list **stack_a, char *str, char **split)
{
	long	num;

	num = ft_atol(str);
	if (!not_equal_number(*stack_a, num))
	{
		write(2, "Error\n", 6);
		ft_free_split(split);
		ft_lstclear(stack_a);
		return (0);
	}
	if (num > INT_MAX || num < INT_MIN)
	{
		write(2, "Error\n", 6);
		ft_free_split(split);
		ft_lstclear(stack_a);
		return (0);
	}
	return (1);
}

static int	process_split(t_list **stack_a, char **split)
{
	int		j;
	t_list	*new_node;

	j = 0;
	while (split[j])
	{
		if (!validate_num(stack_a, split[j], split))
			return (0);
		new_node = ft_new_node(ft_atol(split[j]));
		if (!new_node)
			return (0);
		ft_lstadd_back(stack_a, new_node);
		j++;
	}
	ft_free_split(split);
	return (1);
}

static int	fill_stack(char **argv, int i, t_list **stack_a)
{
	char	**split;
	int		j;

	split = ft_split(argv[i], ' ');
	if (!split)
		return (0);
	j = 0;
	while (split[j])
	{
		if (!argv_isnumber(split[j]))
		{
			write(2, "Error\n", 6);
			ft_free_split(split);
			ft_lstclear(stack_a);
			return (0);
		}
		j++;
	}
	if (!process_split(stack_a, split))
		return (0);
	return (1);
}

static void	sort_stack(char **argv, t_list **stack_a, t_count **count_list)
{
	int	type;

	type = 0;
	(*count_list)->disorder = ft_compute_disorder(*stack_a);
	if (argv[1][0] == '-' && !ft_isdigit(argv[1][1]))
		type = ft_switch(argv[1], stack_a, count_list);
	else
		ft_adaptive(stack_a, count_list);
	if ((*count_list)->bench_active == 1)
		benchmark(*count_list, type);
}

void	arg_control(char **argv, t_count **count_list)
{
	int		i;
	t_list	*stack_a;

	i = 1;
	stack_a = NULL;
	if (argv[1][0] == '-' && !ft_isdigit(argv[1][1]))
		i++;
	while (argv[i])
	{
		if (!fill_stack(argv, i, &stack_a))
			return ;
		i++;
	}
	sort_stack(argv, &stack_a, count_list);
	ft_lstclear(&stack_a);
}
