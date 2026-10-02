/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vic <vic@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 22:20:07 by vabad-ro          #+#    #+#             */
/*   Updated: 2025/12/14 21:12:03 by vic              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include <stdlib.h>

int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
		i++;
	return (i);
}

char	*ft_strdup(char *str)
{
	int		i;
	char	*dup;

	i = 0;
	dup = (char *)malloc(sizeof(*str) * (ft_strlen(str) + 1));
	if (!dup)
		return (NULL);
	while (str[i])
	{
		dup[i] = str[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}

/*int main(void)
{
	char *original = "Hello, World!";
	char *copy = ft_strdup(original);

	if (copy == NULL)
	{
		printf("Error: malloc falló\n");
		return 1;
	}
	printf("Original: %s\n", original);
	printf("Copia: %s\n", copy);
	free(copy);
	return 0;
}
*/
