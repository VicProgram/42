/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_recursive_factorial.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vabad-ro <vabad-ro@student.42madrid.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 22:20:07 by vabad-ro          #+#    #+#             */
/*   Updated: 2025/11/27 13:41:34 by vabad-ro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_recursive_factorial(int nb)
{
	if (nb < 0)
		return (0);
	if (nb == 1 || nb == 0)
		return (1);
	return (nb * ft_recursive_factorial(nb -1));
}

/*int	main(void)
{
	printf("%d", ft_recursive_factorial(1));
	printf("%d", ft_recursive_factorial(0));
	printf("%d", ft_recursive_factorial(-3));
	printf("%d", ft_recursive_factorial(5));
	return (0);
}*/
