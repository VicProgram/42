/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ultimate_range.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vic <vic@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 21:14:27 by vic               #+#    #+#             */
/*   Updated: 2025/12/27 02:07:28 by vic              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

int	ft_ultimate_range(int **range, int min, int max)
{
	int	res;
	int	i;

	//cuidado por que no puede valer negativo
	res = max - min;
	if (min >= max)
	{
		range = 0;
		return (NULL);
	}

	range = malloc(sizeof (int) * res);

	i = 0;
	while(i < res)
	{
		range[i] += min + i;
		i++;
	}
}

int	main(void)
{
	int	range;

	range = 3;
	ft_ultimate_range(range, 4, 9);
	return (0);
}
