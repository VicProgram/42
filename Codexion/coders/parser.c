/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   parser.c                                          :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: vabad-ro <vabad-ro@student.42madrid.com>  #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/10/19 20:56:39 by vabad-ro         #+#    #+#              */
/*   Updated: 2026/10/20 16:43:01 by vabad-ro        ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	check_number(char **args)
{
	int	i;
	int	j;

	i = 1;
	j = 0;
	while (i < 8)
	{
		while (args[i][j] != '\0')
		{
			if (args[i][j] >= '0' && args[i][j] <= '9')
				j++;
			else
				return (0);
		}
		if (j == 0)
			return (0);
		i++;
		j = 0;
	}
	return (1);
}

int	check_scheduler(char **args)
{
	if (strcmp(args[8], "fifo") != 0 && strcmp(args[8], "edf") != 0)
		return (0);
	return (1);
}

int	parse_args(t_sim *sim, char **args)
{
	if (!check_number(args))
	{
		fprintf(stderr, "Error: invalid argument\n");
		return (0);
	}
	if (!check_scheduler(args))
	{
		fprintf(stderr, "Error: invalid argument\n");
		return (0);
	}
	fill_sim(sim, args);
	if (!check_range(sim))
	{
		fprintf(stderr, "Error: invalid argument value\n");
		return (0);
	}
	return (1);
}

void	fill_sim(t_sim *sim, char **args)
{
	sim->n_coders = atoi(args[1]);
	sim->time_to_burnout = atoi(args[2]);
	sim->comp_time = atoi(args[3]);
	sim->debug_time = atoi(args[4]);
	sim->refactor_time = atoi(args[5]);
	sim->compiles_required = atoi(args[6]);
	sim->dongle_cooldown = atoi(args[7]);
	sim->scheduler = args[8];
}

int	check_range(t_sim *sim)
{
	if (sim->n_coders <= 0)
		return (0);
	if (sim->compiles_required <= 0)
		return (0);
	return (1);
}
