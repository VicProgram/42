/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vabad-ro <vabad-ro@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 18:51:29 by vabad-ro          #+#    #+#             */
/*   Updated: 2026/02/17 18:56:51 by vabad-ro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

char	*ft_makestash_bonus(char *stash)
{
	int		i;
	int		j;
	char	*newstash;

	i = 0;
	if (!stash)
		return (NULL);
	while (stash[i] && stash[i] != '\n')
		i++;
	if (!stash[i])
		return (stash = ft_clean_memo_bonus(stash, NULL));
	i++;
	if (!stash[i])
		return (stash = ft_clean_memo_bonus(stash, NULL));
	newstash = malloc(ft_strlen_bonus(stash) - i + 1);
	if (!newstash)
		return (stash = ft_clean_memo_bonus(stash, NULL));
	j = 0;
	while (stash[i])
		newstash[j++] = stash[i++];
	newstash[j] = '\0';
	stash = ft_clean_memo_bonus(stash, NULL);
	return (newstash);
}

int	ft_strlen_bonus(const char *s)
{
	int	i;

	if (!s)
		return (0);
	i = 0;
	while (s[i] != '\0')
		i++;
	return (i);
}

char	*ft_strchr_bonus(const char *s, int c)
{
	if (!s)
		return (NULL);
	while (*s)
	{
		if (*s == (char)c)
			return ((char *)s);
		s++;
	}
	if ((char)c == '\0')
		return ((char *)s);
	return (NULL);
}

char	*ft_makeline_bonus(char *stash)
{
	int		i;
	char	*newline;

	i = 0;
	if (!stash || !stash[0])
		return (NULL);
	while (stash[i] && stash[i] != '\n')
		i++;
	if (stash[i] == '\n')
		i++;
	newline = malloc(i + 1);
	if (!newline)
		return (NULL);
	i = 0;
	while (stash[i] && stash[i] != '\n')
	{
		newline[i] = stash[i];
		i++;
	}
	if (stash[i] == '\n')
		newline[i++] = '\n';
	newline[i] = '\0';
	return (newline);
}

char	*ft_strjoin_bonus(char *s1, char *s2)
{
	char	*result;
	int		i;
	int		j;

	i = 0;
	j = 0;
	if (!s1)
	{
		s1 = malloc(sizeof(char) * 1);
		if (!s1)
			return (NULL);
		s1[0] = '\0';
	}
	if (!s2)
		return (s1);
	result = malloc(ft_strlen_bonus(s1) + ft_strlen_bonus(s2) + 1);
	if (!result)
		return (free(s1), NULL);
	while (s1[i])
		result[j++] = s1[i++];
	i = 0;
	while (s2[i])
		result[j++] = s2[i++];
	result[j] = '\0';
	return (free(s1), result);
}
