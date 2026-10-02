/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_rev_int_tab.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vabad-ro <vabad-ro@student.42madrid.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/20 18:59:59 by vabad-ro          #+#    #+#             */
/*   Updated: 2025/11/24 18:10:35 by vabad-ro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

void	ft_rev_int_tab(int *tab, int size)
{
	int	temp;
	int	i;
	int	j;

	i = 0;
	j = size - 1;
	while (i < size)
	{
		temp = tab[i];
		tab[i] = tab[j];
		tab[j] = temp;
		i++;
		j--;
		size--;
	}
}

/*int	main(void)
{
	int	tab[5] = {1, 2, 3, 4, 5};
	ft_rev_int_tab(tab, 5);
	printf("%d %d %d %d %d", tab[0], tab[1], tab[2], tab[3], tab[4]);
}*/
