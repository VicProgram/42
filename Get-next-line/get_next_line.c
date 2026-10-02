/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vabad-ro <vabad-ro@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/12 17:15:08 by vabad-ro          #+#    #+#             */
/*   Updated: 2026/02/17 17:51:18 by vabad-ro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

/* char	*ft_freestash(char *stash)
{
	if(stash)
		free(stash);
	if(buffer)
	return (NULL);
} */

static char	*ft_read_and_stash(int fd, char *stash)
{
	char	*buffer;
	ssize_t	bytesread;

	buffer = malloc(BUFFER_SIZE + 1);
	if (!buffer)
	{
		if (stash)
			free(stash);
		stash = NULL;
		return (NULL);
	}
	bytesread = 1;
	while (bytesread > 0 && !ft_strchr(stash, '\n'))
	{
		bytesread = read(fd, buffer, BUFFER_SIZE);
		if (bytesread == -1)
		{
			free(stash);
			stash = NULL;
			return (free(buffer), NULL);
		}
		if (bytesread > 0)
		{
			buffer[bytesread] = '\0';
			stash = ft_strjoin(stash, buffer);
			if (!stash)
				return (free(buffer), NULL);
		}
	}
	free(buffer);
	return (stash);
}

char	*get_next_line(int fd)
{
	static char	*stash;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	stash = ft_read_and_stash(fd, stash);
	if (!stash)
	{
		free(stash);
		stash = NULL;
		return (NULL);
	}
	line = ft_cleanstash(stash);
	if (!line)
	{
		free(stash);
		stash = NULL;
		return (NULL);
	}
	stash = ft_makestash(stash);
	return (line);
}
