/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vabad-ro <vabad-ro@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/28 15:32:38 by vabad-ro          #+#    #+#             */
/*   Updated: 2026/02/02 12:50:19 by vabad-ro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stdarg.h>
# include <unistd.h>

int	ft_printf(char const *str, ...);
int	ft_printchr(int c);
int	ft_printstr(char *s);
int	ft_printnbr(int n);
int	ft_printunbr(unsigned int n);
int	ft_printptr(void *c);
int	ft_printhexa(unsigned long c, const char *base);
int	ft_printhexamin(unsigned long c);

#endif
