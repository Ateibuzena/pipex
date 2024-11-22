/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azubieta <azubieta@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 23:09:38 by azubieta          #+#    #+#             */
/*   Updated: 2024/11/22 23:18:21 by azubieta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipexft.h"

char *ft_extract_quoted(char **argv, char quote)
{
    char *start;
    char *result;
    size_t len;

    (*argv)++; // Saltamos la comilla inicial
    start = *argv;
    while (**argv && **argv != quote)
        (*argv)++;
    if (**argv != quote)
        return NULL; // Error: Comillas no cerradas

    len = *argv - start; // Calculamos la longitud del contenido entre comillas
    result = malloc(len + 1);
    if (!result)
        return NULL; // Error de memoria

    ft_strncpy(result, start, len);
    result[len] = '\0';
    (*argv)++; // Saltamos la comilla final

    return result;
}

char *ft_extract_word(char **argv)
{
    char *start;
    char *result;
    size_t len;

    start = *argv;
    while (**argv && !ft_isspace(**argv))
        (*argv)++;

    len = *argv - start; // Calculamos la longitud de la palabra
    result = malloc(len + 1);
    if (!result)
        return NULL; // Error de memoria

    ft_strncpy(result, start, len);
    result[len] = '\0';

    return result;
}