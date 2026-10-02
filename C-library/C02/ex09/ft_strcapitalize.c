/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcapitalize.c                                 :+:      :+:    :+:   */
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

char	*ft_strcapitalize(char *str)
{
	int	i;
	int	a;

	i = 0;
	while (str[i])
	{
		if (str[i] >= 65 && str[i] <= 90)
			str[i] = str[i] + 32;
		i++;
	}
	if (str[0] >= 97 && str[0] <= 122)
		str[0] = str [0] - 32;
	i = 1;
	while (str[i])
	{
		a = str[i - 1];
		if ((str[i] >= 97 && str[i] <= 122) && (!(a >= 97 && a <= 122)
				&& !(a >= 65 && a <= 90) && !(a >= 48 && a <= 57)))
		{
			str[i] = str[i] - 32;
		}
		i++;
	}
	return (str);
}

/*
int	main(void)
{
	char	str[] = "salut, coMMent tu vas ? 42mots quaRante-deux";

	ft_strcapitalize(str);
	printf("%s", str);
	return (0);
}
*/
