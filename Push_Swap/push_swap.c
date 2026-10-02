/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   push_swap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vabad-ro <vabad-ro@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/20 14:14:56 by vabad-ro          #+#    #+#             */
/*   Updated: 2026/03/20 14:15:11 by vabad-ro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

void	push_swap(int argc, char **argv)
{
	t_count	*count_list;

	if (argc < 2)
		return ;
	count_list = malloc(sizeof(t_count));
	if (!count_list)
		return ;
	initialize_list(&count_list);
	if (ft_strncmp(argv[1], "--bench", ft_strlen("--bench")) == 0
		&& !argv[1][ft_strlen("--bench")])
	{
		if (argc < 3)
		{
			free(count_list);
			return ;
		}
		count_list -> bench_active = 1;
		argv ++;
	}
	arg_control(argv, &count_list);
	free(count_list);
	return ;
}

int	main(int argc, char **argv)
{
	if (argc < 2)
		return (0);
	if (argv[1][0] == '\0')
	{
		write(2, "Error\n", 6);
		return (1);
	}
	push_swap(argc, argv);
	return (0);
}
