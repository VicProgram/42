/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ultimate_range2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vic <vic@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/17 13:21:29 by viabad-r          #+#    #+#             */
/*   Updated: 2026/01/02 17:33:47 by vic              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>

int	ft_ultimate_range(int **range, int min, int max)
{
	size_t	res;
	unsigned int	i;
	//cuidado por que no puede valer negativo
	res = max - min;
	if (min >= max)
	{
		range = NULL;
		return (0);
	}

	*range = malloc(sizeof (int) * res);
	if (*range == NULL)
		return(0);

	i = 0;
	while(i < res)
	{
		range[i] = min + i;
		i++;
	}
	return (range);
}

int	main(void)
{
	int	**range;

	printf("%d", ft_ultimate_range(&range, -10, 23));
	free (range);
	return (0);
}
