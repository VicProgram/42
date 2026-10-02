/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_base.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vabad-ro <vabad-ro@student.42madrid.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 10:34:55 by vabad-ro          #+#    #+#             */
/*   Updated: 2025/11/26 11:36:28 by vabad-ro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

void	ft_putchar(char a)
{
	write(1, &a, 1);
}

int	base_len(char *base)
{
	int	i;

	i = 0;
	while (base[i] != '\0')
		i++;
	return (i);
}

int	base_control(char *base)
{
	int	i;
	int	j;
	int	len;

	len = base_len(base);
	i = 0;
	j = 0;
	if (len <= 1)
		return (1);
	while (base[i])
	{
		j = i + 1;
		while (base[j])
		{
			if (base[i] == base[j])
				return (1);
			if (base[i] == '+' || base[i] == '-')
				return (1);
			j++;
		}
		i++;
	}
	return (0);
}

void	ft_putnbr_base(int nbr, char *base)
{
	unsigned int	len;
	unsigned int	l;
	int				i;

	if (base_control(base) == 0)
	{
		len = base_len(base);
		if (nbr < 0)
		{
			l = -nbr;
			write (1, "-", 1);
		}
		else
			l = nbr;
		if (l >= len)
			ft_putnbr_base(l / len, base);
		i = l % len;
		ft_putchar(base[i]);
	}
}

/*int	main(void)
{
	char	base[] = "01";
	
	ft_putnbr_base(42, base);
	return (0);
}*/
