/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_iterative_power.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vabad-ro <vabad-ro@student.42madrid.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 22:42:12 by vabad-ro          #+#    #+#             */
/*   Updated: 2025/11/25 11:25:34 by vabad-ro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

int	ft_iterative_power(int nb, int power)
{
	int	r;

	r = 1;
	if (power < 0)
		return (0);
	if (nb == 0 && power == 0)
		return (1);
	if (power == 1)
		return (nb);
	while (power > 0)
	{
		r *= nb;
		power--;
	}
	return (r);
}

/*int	main(void)
{
	printf("%d", ft_iterative_power(0, 3));
	return (0);
}*/
