/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_div_mod.c                                       :+:      :+:    :+:   */
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

void	ft_div_mod(int a, int b, int *div, int *mod)
{
	*div = a / b;
	*mod = a % b;
}

/*
int	 main (void)
{
	int div;
	int mod;

	div = 1;
	mod = 2;
	
	printf("%d, %d\n", div, mod);
	ft_div_mod(10, 3, &div, &mod);
	printf("%d, %d\n", div, mod);
}
*/
