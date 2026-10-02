/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   push_swap.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vabad-ro <vabad-ro@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/20 14:15:13 by vabad-ro          #+#    #+#             */
/*   Updated: 2026/03/20 14:15:16 by vabad-ro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PUSH_SWAP_H
# define PUSH_SWAP_H

# include <stdarg.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <limits.h>

/*------------------------------Strategy Macros------------------------------*/

# define SIMPLE 1
# define MEDIUM 2
# define COMPLEX 3
# define ADAPTIVE 4

/*--------------------------------Structures---------------------------------*/

/*
** t_list - Doubly linked list node representing a stack element
**   nb 		: The integer value stored in this node
**   index		: Rank of this value with respect to the whole stack 
				  (0 = smallest)
**   next		: Pointer to the node below in the stack
**   prev		: Pointer to the node above in the stack
*/
typedef struct s_list
{
	int				nb;
	int				index;
	struct s_list	*next;
	struct s_list	*prev;

}					t_list;

/*
** t_count - Tracks every operation performed and stack information.
**   sa/sb/ss   : Swap operation counters.
**   pa/pb      : Push operation counters.
**   ra/rb/rr   : Rotate operation counters.
**   rra/rrb/rrr: Reverse rotate operation counters.
**   disorder   : Disorder metric [0.0 - 1.0] computed before sorting.
**   bench_active: Set to 1 when --bench flag is passed.
*/

typedef struct s_count
{
	int				sa;
	int				sb;
	int				ss;
	int				pa;
	int				pb;
	int				ra;
	int				rb;
	int				rr;
	int				rra;
	int				rrb;
	int				rrr;

	int				count_list;
	float			disorder;
	int				bench_active;

}					t_count;

/*---------------------------------ft_printf---------------------------------*/

int					ft_printf(char const *str, ...);
int					ft_printchr(int c);
int					ft_printstr(char *s);
int					ft_printnbr(int n);
int					ft_printunbr(unsigned int n);
int					ft_printptr(void *c);
int					ft_printhexa(unsigned long c, const char *base);

/*-----------------------------String / Math Utils----------------------------*/

long				ft_atol(const char *str);
char				**ft_split(const char *s, char c);
size_t				ft_strlen(const char *str);
size_t				ft_strlcpy(char *dest, const char *src, size_t size);
int					ft_strncmp(const char *s1, const char *s2, size_t n);
int					ft_isdigit(int c);
int					ft_sqrt(int nb);

/*---------------------------------Stack Utils--------------------------------*/

int					stack_size(t_list *stack_a);
int					find_min(t_list *stack_a);
int					find_max(t_list *stack_a);
int					find_min_index(t_list *stack_a);
int					short_way_medium(t_list **stack, int i, int range);
int					short_way_simple_adj(t_list **stack);
int					argv_isnumber(char *argv);
int					not_equal_number(t_list *stack_a, int new_number);
t_list				*ft_new_node(int nb);
t_list				*ft_lstlast(t_list *lst);
void				ft_lstadd_back(t_list **lst, t_list *new);
void				ft_lstclear(t_list **lst);
void				ft_free_split(char **split);
void				assign_index(t_list **stack_a);
void				initialize_list(t_count **count_list);
void				rotate(t_list **stack_a, t_count **count_list, int count,
						int size);
float				ft_compute_disorder(t_list *stack);

/*----------------------------Program Entry Points---------------------------*/

void				push_swap(int argc, char **argv);
void				arg_control(char **argv, t_count **count_list);
int					ft_switch(char *strat, t_list **stack_a,
						t_count **count_list);
void				benchmark(t_count *count_list, int type);

/*------------------------------Swap Operations------------------------------*/

void				sa_move(t_list **stack, t_count **count_list);
void				sb_move(t_list **stack, t_count **count_list);
void				ss_move(t_list **stack_a, t_list **stack_b,
						t_count **count_list);

/*------------------------------Push Operations------------------------------*/

void				pa_move(t_list **stack_a, t_list **stack_b,
						t_count **count_list);
void				pb_move(t_list **stack_a, t_list **stack_b,
						t_count **count_list);

/*-----------------------------Rotate Operations-----------------------------*/

void				ra_move(t_list **stack, t_count **count_list);
void				rb_move(t_list **stack, t_count **count_list);
void				rr_move(t_list **stack_a, t_list **stack_b,
						t_count **count_list);

/*-------------------------Reverse Rotate Operations-------------------------*/

void				rra_move(t_list **stack, t_count **count_list);
void				rrb_move(t_list **stack, t_count **count_list);
void				rrr_move(t_list **stack_a, t_list **stack_b,
						t_count **count_list);

/*---------------------------------Algorithms--------------------------------*/

void				tiny_sort(t_list **stack_a, t_count **count_list);
void				ft_simple(t_list **stack_a, t_count **count_list);
void				ft_medium(t_list **stack_a, t_count **count_list);
void				ft_complex(t_list **stack_a, t_count **count_list);
void				ft_adaptive(t_list **stack_a, t_count **count_list);

#endif
