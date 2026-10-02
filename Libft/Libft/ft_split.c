/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vabad-ro <vabad-ro@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/17 17:02:37 by vabad-ro          #+#    #+#             */
/*   Updated: 2026/01/23 19:30:18 by vabad-ro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	ft_count_words(const char *s, char c)
{
	size_t	word_count;
	int		i;

	word_count = 0;
	i = 0;
	while (s[i])
	{
		while (s[i] && s[i] == c)
			i++;
		if (s[i] && s[i] != c)
			word_count++;
		while (s[i] && s[i] != c)
			i++;
	}
	return (word_count);
}

static size_t	ft_word_len(const char *s, char c)
{
	size_t	i;

	i = 0;
	while (s[i] && s[i] != c)
		i++;
	return (i);
}

static void	ft_alloc_fail(char **arr, int i)
{
	int	count;

	count = 0;
	while (count <= i)
	{
		free(arr[count]);
		count++;
	}
	free(arr);
}

static char	**ft_cpy(char const *s, char c, size_t wcount, char **arr)
{
	size_t	i;
	size_t	j;
	size_t	word_len;

	i = 0;
	j = 0;
	while (j < wcount)
	{
		while (s[i] == c && s[i + 1])
			i++;
		word_len = ft_word_len(&s[i], c);
		arr[j] = malloc((word_len + 1) * sizeof(char));
		if (!arr[j] || word_len == 0)
		{
			ft_alloc_fail(arr, j);
			return (0);
		}
		ft_strlcpy(arr[j++], &s[i], word_len + 1);
		i += word_len;
	}
	arr[j] = NULL;
	return (arr);
}

char	**ft_split(char const *s, char c)
{
	char	**arr;
	size_t	wcount;

	if (!s)
		return (NULL);
	wcount = ft_count_words(s, c);
	arr = malloc((wcount + 1) * sizeof(char *));
	if (!arr)
		return (NULL);
	arr = ft_cpy(s, c, wcount, arr);
	return (arr);
}
