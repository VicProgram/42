/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_switch.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vabad-ro <vabad-ro@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/20 14:14:43 by vabad-ro          #+#    #+#             */
/*   Updated: 2026/03/20 14:14:45 by vabad-ro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static int	type_algorithm(char *argv)
{
	if (ft_strncmp(argv, "--simple", ft_strlen("--simple")) == 0
		&& !argv[ft_strlen("--simple")])
		return (SIMPLE);
	else if (ft_strncmp(argv, "--medium", ft_strlen("--medium")) == 0
		&& !argv[ft_strlen("--medium")])
		return (MEDIUM);
	else if (ft_strncmp(argv, "--complex", ft_strlen("--complex")) == 0
		&& !argv[ft_strlen("--complex")])
		return (COMPLEX);
	else if (ft_strncmp(argv, "--adaptive", ft_strlen("--adaptive")) == 0
		&& !argv[ft_strlen("--adaptive")])
		return (ADAPTIVE);
	else
		return (0);
}

int	ft_switch(char *strat, t_list **stack_a, t_count **count_list)
{
	int	type;

	type = type_algorithm(strat);
	if (type == SIMPLE)
		ft_simple(stack_a, count_list);
	else if (type == MEDIUM)
		ft_medium(stack_a, count_list);
	else if (type == COMPLEX)
		ft_complex(stack_a, count_list);
	else if (type == ADAPTIVE)
		ft_adaptive(stack_a, count_list);
	else
		write(2, "Error\n", 6);
	return (type);
}
