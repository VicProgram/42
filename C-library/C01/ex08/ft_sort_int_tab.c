/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_sort_int_tab.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vabad-ro <vabad-ro@student.42madrid.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/20 18:59:59 by vabad-ro          #+#    #+#             */
/*   Updated: 2025/11/24 18:10:35 by vabad-ro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
#include <stdio.h>
*/

void	ft_sort_int_tab(int *tab, int size)
{
	int	temp;
	int	i;
	int	j;

	j = 0;
	while (j < size - 1)
	{
		i = 0;
		while (i < size - 1)
		{
			if (tab[i] > tab[i + 1])
			{
				temp = tab[i];
				tab[i] = tab[i + 1];
				tab[i +1] = temp;
			}
			i++;
		}
		j++;
	}
}

/*
int	main(void)
{
	int	a[5] = {1, 3, 2, 5, 4};
	int	i;

	i = 0;

	ft_sort_int_tab(a, 5);
	for (i = 0; i < 5; i++)
	{
		printf("%d", a[i]);
	}

	return (0);
}
*/
