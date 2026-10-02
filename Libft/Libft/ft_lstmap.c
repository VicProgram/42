/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vabad-ro <vabad-ro@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/22 15:41:24 by vabad-ro          #+#    #+#             */
/*   Updated: 2026/01/23 19:21:27 by vabad-ro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*new;
	t_list	*new_node;
	void	*content;

	if (!lst)
		return (NULL);
	new = NULL;
	while (lst)
	{
		content = f(lst -> content);
		new_node = ft_lstnew(content);
		if (!new_node)
		{
			del(content);
			ft_lstclear(&new, del);
			return (NULL);
		}
		ft_lstadd_back(&new, new_node);
		lst = lst -> next;
	}
	return (new);
}
