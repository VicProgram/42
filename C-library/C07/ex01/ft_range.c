/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_range.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vic <vic@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 21:14:37 by vic               #+#    #+#             */
/*   Updated: 2025/12/14 21:14:55 by vic              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>

int	*ft_range(int min, int max)
{
	int		n;
	int		j;
	int		*arr;

	if (min >= max)
		return (NULL);
	else
		n = max - min;
	arr = malloc(n * sizeof(int));
	if (!arr)
		return (0);
	j = 0;
	while (min < max)
	{
		arr[j] = min;
		min++;
		j++;
	}
	return (arr);
}

// pruebas con ayuda

/*int main(void)
{
	int min = 4;
	int max = 9;
	int *arr;
	int n;
	int i;

	arr = ft_range(min, max);
	if (!arr)
		return (0);

	n = max - min;

	i = 0;
	while (i < n)
	{
		printf("%d ", arr[i]);
		i++;
	}

	free(arr);
	printf("\n");
	return (0);
}
*/
