/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vabad-ro <vabad-ro@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/12 17:15:08 by vabad-ro          #+#    #+#             */
/*   Updated: 2026/02/17 18:59:56 by vabad-ro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

char	*ft_clean_memo_bonus(char *stash, char *buffer)
{
	if (stash)
		free(stash);
	if (buffer)
		free(buffer);
	return (NULL);
}

static char	*ft_read_and_stash_bonus(int fd, char *stash)
{
	char	*buffer;
	ssize_t	bytesread;

	buffer = malloc(BUFFER_SIZE + 1);
	if (!buffer)
	{
		if (stash)
			return (ft_clean_memo_bonus(stash, NULL));
	}
	bytesread = 1;
	while (bytesread > 0 && !ft_strchr_bonus(stash, '\n'))
	{
		bytesread = read(fd, buffer, BUFFER_SIZE);
		if (bytesread == -1)
			return (ft_clean_memo_bonus(stash, buffer));
		if (bytesread > 0)
		{
			buffer[bytesread] = '\0';
			stash = ft_strjoin_bonus(stash, buffer);
			if (!stash)
				return (ft_clean_memo_bonus(NULL, buffer));
		}
	}
	free(buffer);
	return (stash);
}

char	*get_next_line(int fd)
{
	static char	*stash[FD_LIMIT];
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	stash[fd] = ft_read_and_stash_bonus(fd, stash[fd]);
	if (!stash[fd])
	{
		free(stash[fd]);
		stash[fd] = NULL;
		return (NULL);
	}
	line = ft_makeline_bonus(stash[fd]);
	if (!line)
	{
		free(stash[fd]);
		stash[fd] = NULL;
		return (NULL);
	}
	stash[fd] = ft_makestash_bonus(stash[fd]);
	return (line);
}
