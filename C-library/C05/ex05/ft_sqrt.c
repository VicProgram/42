/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_sqrt.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vabad-ro <vabad-ro@student.42madrid.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 18:56:09 by vabad-ro          #+#    #+#             */
/*   Updated: 2025/11/27 13:56:51 by vabad-ro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

int	ft_sqrt(int nb)
{
	int	i;

	i = 0;
	if (nb == 1)
		return (1);
	while (i <= nb / 2)
	{
		if (i * i == nb)
			return (i);
		i++;
	}
	return (0);
}

/*int	main(void)
{
	printf("%d", ft_sqrt(1));
	return (0);
}
*/