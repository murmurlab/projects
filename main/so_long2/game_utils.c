#include "game.h"

int	check_lines\
(
	struct s_read_map *s_read_map
)
{
	if (*(s_read_map->line) != '1' || *(s_read_map->line + \
	(s_read_map->old_x_len - 2)) != '1')
		return (1);
	if (s_read_map->old_x_len != xstrlen(s_read_map->line))
	{
		if (!xstrchr(s_read_map->line, '\n'))
			s_read_map->end = 1;
		else
			return (2);
	}
	s_read_map->count_P += (short)strclen(s_read_map->line, 'P');
	s_read_map->count_E += (short)strclen(s_read_map->line, 'E');
	findex(s_read_map->s_game->map, (*s_read_map).i - 1)->content = \
	malloc(s_read_map->old_x_len);
	//p("%s", findex(s_read_map->s_game->map, (*s_read_map).i - 1)->content);
	if (s_read_map->count_E > 1 || s_read_map->count_P > 1)
		return (6);
	s_read_map->p_p = xstrchr(s_read_map->line, 'P'); // maybe not need player coords
	s_read_map->s_game->colls += (short)strclen(s_read_map->line, 'C');
	if (s_read_map->p_p)
	{
		*s_read_map->p_p = '0';
		s_read_map->s_game->p = malloc(2);
		s_read_map->s_game->p[0] = (s_read_map->p_p - s_read_map->line);
		s_read_map->s_game->p[1] = s_read_map->i - 1;
	}
	llend(s_read_map->s_game->map)->content = s_read_map->line;
	// xstrlcpy(findex(s_read_map->s_game->map, (*s_read_map).i - 1)->content, \
	s_read_map->line, s_read_map->old_x_len + s_read_map->end);
	return (0);
}

int	check_y_border(char *line)
{
	// p("<%s>", line);
	int	i;

	i = 0;
	while (line[i] != '\n' && line[i])
	{
		if (line[i++] != '1')
			return (1);
	}
	return (0);
}

int	load_map(struct s_read_map *s_read_map, char **c)
{
	int		fd_map;

	fd_map = open(*(c + 1), O_RDONLY);
	(*s_read_map).line = multiRowRead(fd_map);
	p("\n<%s>", (*s_read_map).line);
	(*s_read_map).old_x_len = xstrlen((*s_read_map).line); //first check_lines(s_read_map)
	if (check_y_border((*s_read_map).line))
		return (5);
	(*s_read_map).i = 1;
	(*s_read_map).s_game->map = llnew(0);
	while (1)
	{
		s_read_map->exit_code = check_lines(s_read_map);
		if (s_read_map->exit_code)
			return (s_read_map->exit_code);
		(*s_read_map).old_x_len = xstrlen((*s_read_map).line);
		// free((*s_read_map).line);
		(*s_read_map).line = multiRowRead(fd_map);
		p("<%s>", (*s_read_map).line);
		if (s_read_map->line)
		{
			(*s_read_map).i++; //optimize
			lladd(&s_read_map->s_game->map, llnew(0));
		}
		else
			break ;
	}
	s_read_map->s_game->y_len = s_read_map->i;
	s_read_map->s_game->x_len = s_read_map->old_x_len;
	p("|%s|", findex(s_read_map->s_game->map, s_read_map->i - 1)->content);
	if (check_y_border(llend(s_read_map->s_game->map)->content))
		return (7);
	free((*s_read_map).line);
	close(fd_map);
	return (0);
}

void iter(void *ss)
{
	p("%s\n", ss);
}
// struct arithmetic
void	wasd(struct s_game *s_game, int *xy, int aa)
{
	if (aa)
	{
		draw_block(xy[0] + s_game->set_wasd[s_game->select][0], xy[1] + s_game->set_wasd[s_game->select][1], s_game, s_game->key_map['P']);
		draw_block(xy[0], xy[1], s_game, NULL);
		xy[1] += s_game->set_wasd[s_game->select][1];
		xy[0] += s_game->set_wasd[s_game->select][0];
		return ;
	}
	while (s_game->ct < 4)
		s_game->get_wasd[s_game->key_arr[s_game->ct++]] = draw_all(s_game, xy[1] + s_game->set_wasd[s_game->key_arr[s_game->ct]][1]) + xy[0] + (s_game->set_wasd[s_game->key_arr[s_game->ct]][0]);
	s_game->ct = 0;
	s_game->get_wasd[3] = (draw_all(s_game, xy[1]) + xy[0]);
}

int		update(t_game *s_game)
{
	wasd(s_game, s_game->p, 0);
	if (s_game->get_wasd[s_game->select][0] == 'E' && s_game->my_colls == s_game->colls)
			exit(0);
	if (s_game->get_wasd[s_game->select][0] == 'C')
	{
		*s_game->get_wasd[s_game->select] = '0';
		wasd(s_game, s_game->p, 1);
		// lliter(s_game->map, &iter);
		s_game->my_colls++;
	}
	else if(s_game->get_wasd[s_game->select][0] != '1')
		wasd(s_game, s_game->p, 1);
	p("x:%d, y:%d ", s_game->p[0], s_game->p[1]);
	return (1);
}

int	validate_map(struct s_game *s_game, t_pf *pf)
{
	char	*pointer1;
	// return (0);
	draw_all(s_game, 0);
	// p("%d\n", s_game->x_len);
	// p("%d\n", s_game->x_len);

	char xy[2];
	t_list	*tmp;
	pf->i = 0;
	pf->p[0] = s_game->p[0];
	pf->p[1] = s_game->p[1];
	//path-finder
	pf->stack = llnew(0);
	// pf->first = 0;
	s_game->select = -1;

	while (1)
	{
		wasd(s_game, pf->p, 0);
		// update directions in link
		while (pf->i != 4)
		{
			if (s_game->get_wasd[s_game->key_arr[pf->i]][0] != '1' &&
				s_game->get_wasd[s_game->key_arr[pf->i]][0] != '#')
			{
				if (s_game->select != -1)
				{
					tmp = llnew(0); // key map[:)] - lladd(llnew) llend.i =)
					tmp->i = s_game->key_arr[pf->i];
					lladd(&llend(pf->stack)->que, tmp);
				}
				else
					s_game->select = s_game->key_arr[pf->i];
				// if (s_game->get_wasd[s_game->key_arr[pf->i]][0] == 'E')
				// {
				// 	if (s_game->my_colls == s_game->colls)
				// 		return(0);
				// 	pf->e_flag = 1;
				// }
				// else if (s_game->get_wasd[s_game->key_arr[pf->i]][0] == 'C')
				// {
				// 	if (pf->e_flag && (s_game->my_colls == s_game->colls))
				// 		return(0);
				// 	s_game->my_colls++;
				// }
				// register coord in stack(murmurlibc) if has now alternate path
				// go in map
				// make footprint ('#')
				// pf->i = 0;
				// continue ;
			}
			pf->i++;
		}
		// if (lllen(pf->que) > 1)
		// 	pf->stack->que = pf->que;
		// pf->first;
		// s_game->select = pf->first;
		if (s_game->select != -1)
		{
			pointer1 = draw_all(s_game, pf->p[1]);
			pointer1[(pf->p)[0]]='#';
			wasd(s_game, pf->p, 1);
		}
		else
		{
			pointer1 = draw_all(s_game, pf->p[1]);
			pointer1[(pf->p)[0]]='#';
			wasd(s_game, pf->p, 1);
			// tp to last alternate path
		}
		s_game->select = -1;
		pf->i = 0;
		print_map(s_game);
	}
	s_game->my_colls = 0;
	return (0);
}
