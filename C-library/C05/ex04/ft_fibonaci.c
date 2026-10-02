/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_fibonaci.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vabad-ro <vabad-ro@student.42madrid.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 13:31:03 by vabad-ro          #+#    #+#             */
/*   Updated: 2025/11/25 18:55:18 by vabad-ro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

int	ft_fibonacci(int index)
{
	if (index == 0)
		return (0);
	if (index == 1)
		return (1);
	if (index < 0)
		return (-1);
	return (ft_fibonacci(index - 1) + ft_fibonacci(index - 2));
}

/*int	main(void)
{
	printf("%d", ft_fibonacci(5));
	return (0);
}*/
