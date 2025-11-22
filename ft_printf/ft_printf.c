/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmurugan <kmurugan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 12:27:53 by kmurugan          #+#    #+#             */
/*   Updated: 2025/11/19 17:50:15 by kmurugan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int		ft_vprintf(const char *fmt, va_list ap);
static int		print_literal(const char **fmt);
static size_t	ft_strcspn_char(const char *s, int c);

/*
	NAME
		ft_printf
	SYNOPSIS
		#include "ft_printf.h"
	PROTOTYPE
		int ft_printf(const char *, ...);
	DESCRIPTION:
		Prints a formatted string to the standard output.
		Conversions are: cspdiuxX%
	RETURN VALUE
		Returns the number of characters printed.
		The printf utility exits 0 on success, and >0 if an error occurs.
	EXTERNAL FUNC(S)
		write, va_start, va_arg, va_end
*/

/*
	STDARG (3) / va_arg
	stdarg  variable argument lists
	va_list	<variable_name>
			structure used by the following functions / macros

	VARIADIC FUNCTIONS / MACROS
	void	va_start(va_list ap, last);			// initializes ap
	type	va_arg(va_list ap, type);	// returns n bytes after 1st param
	void	va_copy(va_list dest, va_list src)	//
	void	va_end(va_list ap);					// frees the ap
*/

int	ft_printf(const char *fmt, ...)
{
	int		ret;
	va_list	ap;

	va_start(ap, fmt);
	ret = ft_vprintf(fmt, ap);
	va_end(ap);
	return (ret);
}

static int	ft_vprintf(const char *fmt, va_list ap)
{
	int		ret;
	ssize_t	written;
	t_fmt	flag;

	ret = 0;
	while (*fmt)
	{
		if (*fmt == '%')
		{
			fmt++;
			if (!parse_fmt(&fmt, &flag))
				return (-1);
			written = handle_spec(ap, &flag);
		}
		else
			written = print_literal(&fmt);
		if (written < 0 || written > INT_MAX || (int)written > (INT_MAX - ret))
			return (-1);
		ret += written;
	}
	return (ret);
}

static int	print_literal(const char **fmt)
{
	ssize_t	written;
	size_t	len;

	len = ft_strcspn_char(*fmt, '%');
	if (len > INT_MAX)
		return (-1);
	written = write(FD, *fmt, len);
	*fmt += len;
	return (written);
}

/*
DESCRIPTION:
	Returns the number of bytes in the initial part of 's' which precedes
	the first occurrence of character 'c'. If 'c' is not found, returns
	the length of the string.

	Simplified version of strcspn(const char *s, const char *charset);
*/

static size_t	ft_strcspn_char(const char *s, int c)
{
	size_t	n;

	n = 0;
	while (s[n] && s[n] != (unsigned char)c)
		n++;
	return (n);
}
