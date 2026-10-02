/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_program_name.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vabad-ro <vabad-ro@student.42madrid.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 12:04:56 by vabad-ro          #+#    #+#             */
/*   Updated: 2025/11/26 14:16:36 by vabad-ro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

int	main(int argc, char *argv[])
{
	int	i;

	i = 0;
	if (argc != 1)
	{
		write(2, "Error en el número de argumentos", 32);
		return (1);
	}
	while (argv[0][i] != '\0')
	{
		write(1, &argv[0][i], 1);
		i++;
	}
	write(1, "\n", 1);
	return (0);
}
