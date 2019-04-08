/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dstepane <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/31 14:35:08 by dstepane          #+#    #+#             */
/*   Updated: 2019/03/31 14:35:10 by dstepane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"

static void		check_duplicates(t_lemin *l, char *line)
{
	t_room		*r;
	char		**split;
	int			x;
	int			y;

	split = ft_strsplit(line, ' ');
	r = l->rooms;
	x = ft_atoi(split[1]);
	y = ft_atoi(split[2]);
	while (r)
	{
		if (!ft_strcmp(split[0], r->name))
			error("ERROR: Rooms with the same name.");
		if (r->x == x && r->y == y)
			error("ERROR: Rooms with the same coordinates.");
		r = r->next;
	}
	del_arr(split);
}

static int				validate_room2(t_lemin *l, char *str)
{
	int		i;

	i = 0;
	if (str[i] == '-')
	{
		if (!l->options[4])
			error("ERROR: Negative coordinates.\n[Option -n to allow]");
		i++;
	}
	while (str[i])
	{
		if (!ft_isdigit(str[i++]))
			return (0);
	}
	return (1);
}

int				validate_room(t_lemin *l, char *line)
{
	char		**split;
	int			i;
	int			sp;

	if (line[0] == 'L')
		error("Invalid room name.");
	i = 0;
	sp = 0;
	while (line[i])
	{
		if (line[i] == ' ')
			sp++;
		i++;
	}
	if (sp != 2)
		return (0);
	split = ft_strsplit(line, ' ');
	if (!split[0] || !split[1] || !split[2] || split[3])
	{
		del_arr(split);
		return (0);
	}
	if (!validate_room2(l, split[1]) || !validate_room2(l, split[2]))
	{
		del_arr(split);
		return (0);
	}
	del_arr(split);
	check_duplicates(l, line);
	return (1);
}

int				validate_ants(t_lemin *l, char *line)
{
	int		i;

	i = 0;
	if (ft_isdigit(line[0]) && !ft_strchr(line, ' ') && !ft_strchr(line, '-')
		&& l->rooms == NULL)
	{
		while (ft_isdigit(line[i]))
			i++;
		if (line[i] == '\0')
			return (1);
	}
	return (0);
}

int				parse_ants(t_lemin *l, char *line)
{
	int			i;
	intmax_t	ants;

	ants = 0;
	i = 0;
	while (line[i])
	{
		if (!ft_isdigit(line[i]) && line[i] != '\n')
		{
			ft_strdel(&line);
			error("ERROR: Invalid ants number.");
		}
		i++;
	}
	ants = ft_atoi(line);
	if (ants <= 0 || ants > INT_MAX)
	{
		ft_strdel(&line);
		error("ERROR: Invalid ants number.");
	}
	if (l->ants != 0)
		error("ERROR: Multiple ants input. Shame on you!");
	return ((size_t)ants);
}

void			parse_comment(t_lemin *l, char **line)
{
	int		flag;
	
	flag = 0;
	if (!ft_strcmp(*line, "##start"))
		flag = 1;
	else if (!ft_strcmp(*line, "##end"))
		flag = 2;
	if (flag)
	{
		while (*line[0] == '#')
		{
			ft_putendl(*line);
			ft_strdel(*&line);
			get_next_line(0, *&line);
			((!ft_strcmp(*line, "##start")) || (!ft_strcmp(*line, "##end")))
				? error("ERROR: Start/end flags are messed up!") : 0;
		}
		(validate_room(l, *line)) ? addroom(l, *line, flag) :
			error("ERROR: Invalid room input. Ants hate this!");
	}
}
