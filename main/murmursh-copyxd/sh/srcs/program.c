/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   program.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahbasara <ahbasara@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 21:30:20 by ahbasara          #+#    #+#             */
/*   Updated: 2024/01/02 02:14:18 by ahbasara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include.h"
#include "testing.h"
#include <stdarg.h>

int		qsignal;

// char	*mod_strcpy(char *dest, char *src)
// {
// 	unsigned int i;

// 	i = 0;
// 	while (src[i] != '\0')
// 	{
// 		dest[i] = src[i];
// 		i++;
// 	}
// 	dest[i] = src[i];
// 	return (dest + i);
// }

// void	*mod_memcpy(void *dest, const void *src, size_t n)
// {
// 	char	*d;
// 	char	*s;

// 	if (!src && !dest)
// 		return (dest);
// 	d = (char *)dest;
// 	s = (char *)src;
// 	while (n--)
// 		*d++ = *s++;
// 	return (d);
// }

char	*ft_strsjoin(t_merge *strs[])
{
	void		*ret;
	char		*s;
	size_t		_;

	_ = 0;
	s = 0;
	while (strs[_])
		s += strs[_++]->len;
	if (!s)
		return (NULL);
	s = (ret = ft_calloc(sizeof(char), (size_t)s + 1));
	if (s == NULL)
		return (NULL);
	_ = 0;
	while (strs[_])
	  {
		if (strs[_]->str)
			while (strs[_]->len--)
				*s++ = *(strs[_]->str)++;
		_++;
	}
	return (ret);
}

char	*ft_strcpy(char *dest, char *src)
{
	unsigned int i;

	i = 0;
	while (src[i] != '\0')
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = src[i];
	return (dest);
}

char	*ft_strncpy(char *dest, char *src, unsigned int n)
{
	unsigned int	i;

	i = 0;
	while (src[i] != '\0' && i < n)
	{
		dest[i] = src[i];
		++i;
	}
	while (i < n)
	{
		dest[i] = '\0';
		i++;
	}
	return (dest);
}

void	e(int err)
{
	write(2, "shell: ", 7);
	write(2, strerror(EACCES), ft_strlen(strerror(EACCES)));
	write(2, "\n", 1);
}

void	e2(char *s)
{
	write(2, s, ft_strlen(s));
}

int		set(t_main *data, char const *name, char const *value)
{
	char const	**a;
	t_list	*find;

	find = lst_filter(data->vars, check, name);
	if (!find)
	{
		a = malloc(2 * sizeof(char *));
		a[0] = name;
		a[1] = value;
		ft_lstadd_back(&data->vars, ft_lstnew(a));
	}
	else
	{
		free(((char **)find->content)[1]);
		((char const **)find->content)[1] = value;
	}
	return (0);
}

int	check(t_list *node, void *cmp)
{
	if (!strcmp(((char **)(node->content))[0], (char const *)cmp))
		return (1);
	return (0);
}

char	*get(t_main *data, char const *var)
{
	t_list	*find;

	find = lst_filter(data->vars, check, var);
	if (find)
		return (strdup(((char **)find->content)[1]));
	return (strdup(getenv(var)));
}

char	*get_ref(t_main *data, char const *var)
{
	t_list	*find;

	find = lst_filter(data->vars, check, var);
	if (find)
		return (((char **)find->content)[1]);
	return (getenv(var));
}

void	coix(int sig)
{
	// sleep(1);
	// printf("%c%c", 8, 8);
	rl_on_new_line();
	// rl_replace_line("", 1);
	printf("\033[K");
	rl_redisplay();
	qsignal = 1;
}

covid	ctrl_c(int sig)
{
	rl_replace_line("", 0);
	rl_on_new_line();
	printf("\n");
	rl_redisplay();
}

int	sh_exit(t_com *coms)
{
	// rl_on_new_line();
	// rl_replace_line("goodbye👋\n", 12);
	// \033[A\033[;9H 
	printf("exit\n");
	// rl_redisplay();
	exit(0);
}

int	sh_cd(t_com *coms, char *dir)
{
	char	*buff;

	buff = malloc(1024);
	chdir(dir);
	getcwd(buff, 1024);
	set(coms->data, "PWD", buff);
	return (0);
}

void	resolve(char *string)
{
	struct stat sb;
	char		*loc;

	if(ft_strchr(string, '/'))
	{
		if (!access(string, F_OK))
		{
			stat(string, &sb);
			if (S_ISREG(sb.st_mode)) {
				access(string, X_OK);
				if (errno == EACCES)
					e(EACCES);
			}
			if (S_ISDIR(sb.st_mode)) {
				printf("%s: is a directory\n", string);
			}
		}
		else
		{
			printf("%s: No such file or directory\n", string);
		}
	}
	else
	{
		loc = check_cmd(string);
		if (!loc)
		{
			if (errno == EPERM || errno == EACCES)
				e(EACCES);
			else
				e2("command not found\n");
		}
	}
}

int	find_exe(char *string)
{
	struct stat sb;

	perror("hata basmak;");
	stat(string, &sb);
	if (!errno)
	{
		if (S_ISREG(sb.st_mode))
		{
			access(string, X_OK);
			return (0);
		}
		else if (S_ISDIR(sb.st_mode))
		{
			printf("%s: is a directory\n", string);
			return (1);
		}
	}
	else
		return (2);
	return (0);
}

/**
 * replace `if (path[_] == ':' || !path[_])` to `if (path[_] == ':')`
 * 
 * move the following lines to outside the while
 * if (!path[_])
 * 		break ;
 */
char	*check_cmd(char *cmd)
{
	char	*path;
	char	**dirs;
	char	*tmp;
	size_t	_;
	size_t	_2;

	path = (getenv("PATH"));
	if (!path)
		return (NULL);
	printf(": %s\n", path);
	_ = 0;
	while ("")
	{
		if (path[_] == ':' || !path[_])
		{
			if (!path[_] && !_)
				break ;
			tmp = ft_strsjoin((t_merge *[]){ \
									&(t_merge){path, _},
									&(t_merge){"/", 1},
									&(t_merge){cmd, ft_strlen(cmd)}, NULL
								});
			printf("; %s\n", tmp);
			if (!find_exe(tmp))
				break ;
			free(tmp);
			tmp = NULL;
			if (!path[_])
				break ;
			path += _ + 1;
			_ = 0;
		}
		if (path[_])
			_++;
	}
	return (tmp);
}

void	child(t_main *data, void *exec_data)
{
	struct {size_t _; pid_t *pids; int fd[];} *execd = exec_data;
	// dup2(1, data->cmds[execd->_].out);
	// dup2(0, data->cmds[execd->_].in);
	if (execd->_ != 0) // not first
		dup2(0, execd->fd[0]);
	if ((execd->_ != (data->cmd_ct - 1)) && (data->cmd_ct != 1)) // not last
		dup2(1, execd->fd[1]);
	if (execd->_)
		waitpid(execd->pids[execd->_ - 1], NULL, 0);
	// execve()
	printf("%s%d parent: %d\n", "mypid: ", getpid(), getppid());
	exit(0);
}

void	check_all(t_main *data)
{
	size_t	_;

	_ = 0;
	while (data->cmd_ct > _)
	{
		resolve(data->cmds[_++].cmd);
	}
	
}

void	exe_cute_cat(t_main *data)
{
	struct
	{
		size_t		_;
		pid_t		*pids;
		int			fd[2];
	}				execd;
	pipe(execd.fd);
	check_all(data);
	execd.pids = malloc(sizeof(pid_t) * data->cmd_ct);
	execd._ = 0;
	execd.pids[execd._] = 1;
	while (execd._ < data->cmd_ct)
	{
		execd.pids[execd._] = fork();
		if (execd.pids[execd._] == 0)
			break ;
		execd._++;
	}
	if (execd.pids[execd._] == 0 && execd._ != data->cmd_ct)
		child(data, &execd);
	else
	{
		
	}
// 	bash  defines the following built-in commands: :, ., [, alias, bg, bind, break, builtin, case, cd, command, compgen, complete, continue, declare, dirs, disown, echo, enable, eval, exec, exit, ex‐pts, hash, help, history, if, jobs, kill, let, local, logout, popd, printf, pushd, pwd, read, readonly, return, set, shift, shopt, source,  suspend,  te
// st,  times,  trap,  type,mask, unalias, unset, until, wait, while.
	
	printf("r\n");
}

int	exe(t_com *coms, char *cmd)
{
	int		_;

	_ = 0;
	while (_ < CMD_COUNT)
	{
		if (!strcmp(cmd, coms[_].name))
		{
			(free(cmd), coms[_].func(coms));
			return (0);
		}
		_++;
	}
	if (cmd && cmd[0] != '\0')
		printf("%s %s", cmd, ": command not found\n");
	// printf("\n");
	free(cmd);
	return (0);
}
// size_t	get_len(t_var *var)
// {
// 	strlen
// }

int	is_var(int c)
{
	return (((c >= 'a' && c <= 'z')
			|| (c >= 'A' && c <= 'Z')) || (c >= '0' && c <= '9')
			|| (c == '?') || (c == '_')); // soru işareti sadece başta olacak
}

    // {"'$a'\"''$a''\"\"'$a'\"\"  '$a' $ $a \"\"  '$'  \"\"''\"'\"$a\"'\"$_\"'$_'-=$$\"$a\"$''$$$-$'a'4\"$a $a\"a'$a''a'\"v$a$av$\"$' ''$'a'\"$a\"'\"''\"31","aa0000=$a99$-$$$$$$$1$2$$11$$17"},


/**
 * @brief	('$' != c) && ('<' != c) &&
 * 			('>' != c) && ('\\\\'' != c) &&
 *			('"' != c) && ('|' != c) && (' ' != c)
 * @param c 
 * @return int 
 */
int	is_word(int c)
{
	return (('<' != c) && ('>' != c) && ('\'' != c) && \
			('"' != c) && ('|' != c) && (' ' != c) && ('\0' != c));
}

int	is_text(int c)
{
	return (('<' != c) && ('>' != c) && \
			('|' != c) && (' ' != c) && ('\0' != c));
}

size_t	var_name_len(char *start)
{
	int		_;

	_ = 0;
	while (is_var(start[_]))
		_++;
	return (_);
}

size_t	*len_literal(t_main *data, size_t offset)
{
	t_exp	exp;

	exp.duo = malloc(sizeof(size_t) * 2);
	exp.duo[1] = offset;
	exp.duo[0] = 0;
	while (data->line[exp.duo[1]] != '"' && data->line[exp.duo[1]])
	{
		if (data->line[exp.duo[1]] == '$')
		{
			exp.duo[1]++;
			exp.var_value = get_var_ref(data, data->line + exp.duo[1], exp.size = \
										var_name_len(data->line + exp.duo[1]));
			if (exp.size)
			{
				if (exp.var_value)
					exp.duo[0] += ft_strlen(exp.var_value);
				exp.duo[1] += exp.size;
			}
			else
				exp.duo[0]++;
		}
		else
			(void)(exp.duo[0]++, exp.duo[1]++);
	}
	// printf("len literal: %zu sizereal: %zu\n", exp.duo[0], exp.duo[1] - offset);
	exp.duo[1] -= offset;
	return (exp.duo);
}

size_t	len_string(t_main *data, size_t offset)
{
	size_t	len;
	size_t	_;

	len = 0;
	_ = offset;
	while (data->line[_] != '\'' && data->line[_])
		(void)(len++, _++);
	// printf("len string: %zu\n", len);
	return (len);
}

size_t	len_word(t_main *data, size_t offset)
{
	size_t	len;
	size_t	_;

	len = 0;
	_ = offset;
	while (is_word(data->line[_]) && !((data->line[_] == '$') && is_var(data->line[_ + 1])))
	{
		_++;
		len++;
	}
	// printf("len normal: %zu\n", len);
	return (len);
}

size_t	len_all(t_main *data, size_t offset)
{
	t_all	exp;
	size_t	total;

	exp.len = 0;
	total = 0;
	exp.quote = 0;
	exp.index = offset;
	exp.ptr = malloc(sizeof(size_t) * 2);
	ft_bzero(exp.ptr, sizeof(size_t) * 2);
	while (is_text(data->line[exp.index]) && !((data->line[exp.index] == '$') \
										&& is_var(data->line[exp.index + 1])))
	{
		exp.quote = data->increases[data->line[exp.index]];
		if (data->line[exp.index] == '\'')
			exp.len = len_string(data, exp.index + 1);
		else if (data->line[exp.index] == '"' && (free(exp.ptr), 1))
			exp.ptr = len_literal(data, exp.index + 1);
		else if (is_word(data->line[exp.index]) && !((data->line[exp.index] == \
									'$') && is_var(data->line[exp.index + 1])))
			exp.len = len_word(data, exp.index);
		exp.index += exp.len + (size_t)exp.quote + exp.ptr[1];
		total += exp.len + exp.ptr[0];
		exp.len = 0;
		ft_bzero(exp.ptr, sizeof(size_t) * 2);
	}
	free(exp.ptr);
	// printf("len: %zu\n", total);
	return (total);
}

/**
 * @brief get_var(data, after_dollar, var_name_len(after_dollar))
 * 
 * @param data 
 * @param after_dollar 
 * @param len 
 * @return char* 
 */
char	*get_var_ref(t_main *data, char *var_name, size_t len)
{
	const char	*name = ft_substr(var_name, 0, len);
	char		*tmp;

	if (name && name[0])
	{
		tmp = get_ref(data, name);
		return (free((void *)name), tmp);
	}
	else if (name)
		return (free((void *)name), NULL);
	return (NULL);
}

int		cpy_var(t_main *data, t_exp *exp, size_t offset)
{
	size_t _;
	_ = offset;
	char		*tmp;
	const char	*name = ft_substr(data->line + _, 0, exp->size = var_name_len(data->line + _));

	if (name && name[0])
	{
		tmp = get_ref(data, name);
		if (tmp)
			while (*tmp)
				exp->ret[exp->duo[1]++] = *tmp++;
		_ += exp->size;
		return (free((void *)name), 0);
	}
	else if (name)
	{
		exp->ret[exp->duo[1]++] = data->line[_ - 1]; // "$"
		return (free((void *)name), 0);
	}
	return (0);
}

size_t	*expander_exp(t_main *data, char *dst, size_t offset)
{
	t_exp	exp;

	offset++;
	exp.duo = malloc(sizeof(size_t) * 2);
	exp.duo[0] = 0;
	exp.duo[1] = 0;
	// printf("len: %i data_: %zu\n", len_literal(data, _)[0], _);
	exp.ret = dst;
	while (data->line[offset + exp.duo[0]] != '"' && data->line[offset + exp.duo[0]])
	{
		if (data->line[offset + exp.duo[0]] == '$')
		{
			if (cpy_var(data, &exp, offset + exp.duo[0] + 1))
				return (NULL);
			exp.duo[0] += exp.size + 1;
		}
		else
			exp.ret[exp.duo[1]++] = data->line[offset + exp.duo[0]++];
	}
	exp.duo[0] += 2;
	// printf("0: [%zu] 1: %zu\n", exp.duo[0], exp.duo[1]);
	return (exp.duo);
}

t_turn	join_all(t_main *data, size_t offset)
{
	t_all			exp;
	t_turn			turn;

	turn.buffer = calloc((len_all(data, 0) * sizeof(char)) + sizeof(char), 1);
	turn.ptr = turn.buffer;
	turn.index = offset;
	exp.len = 0;
	exp.quote = 0;
	while (is_text(data->line[turn.index]) && !((data->line[turn.index] == '$') && \
			is_var(data->line[turn.index + 1])))
	{
		if (data->line[turn.index] == '\'')
		{
			exp.quote = data->increases[data->line[turn.index]];
			exp.len = len_string(data, turn.index + 1) + 2;
			ft_memcpy(turn.ptr, (data->line + turn.index + 1), exp.len - 2);
		}
		else if (data->line[turn.index] == '"') // else
		{
			exp.ptr = expander_exp(data, turn.ptr, turn.index);
			turn.ptr += exp.ptr[1];
			turn.index += exp.ptr[0];
			free(exp.ptr);
		}
		else if (is_word(data->line[turn.index]) && !((data->line[turn.index] == '$') && is_var(data->line[turn.index + 1]))) // else
		{
			exp.len = len_word(data, turn.index);
			ft_memcpy(turn.ptr, (data->line + turn.index), exp.len);
		}
		turn.index += exp.len;
		turn.ptr += exp.len - exp.quote;
		exp.quote = 0;
		exp.len = 0;
	}
	// printf("%s: %s\n", __func__, turn.buffer);
	return (turn);
}

// int	set_value(t_main *data, char *str)
// {
// 		if (!data->flags[5])
// 			{
// 				// data->cmds = ;
// 			}
// }

void	check_operation(t_main *data)
{
	if (data->line[data->_] == '>' && data->line[data->_ + 1] == '>')
		return ;
	if (data->line[data->_] == '<' && data->line[data->_ + 1] == '<')
		return ;
	if (data->line[data->_] == '<')
		return ;
	if (data->line[data->_] == '>')
		return ;
	if (data->line[data->_] == '|')
		return ;
	if (data->line[data->_] == '$')
		return ;
	if (data->line[data->_] == '\'')
		return ;
	if (data->line[data->_] == '"')
		return ;
}

int		parser(t_main *data)
{
	static char	*a;
	t_turn		turn;
	

	if (TEST)
		run_test();
	// while (data->line[data->_] != 0)
	// {
		
	// 	check_operation()
	// 	break ;
	// 	data->_++;
	// }
	
	return (0);
}

void	f(t_list *node)
{
	printf("var %s: %s\n", ((char **)(node))[0], ((char **)node)[1]);
}

void	f2(t_list *node)
{
	free(((char **)(node))[0]);
	free(((char **)(node))[1]);
	free(node);
}

int	main(void)
{
	int			fd;
	int			p[2][2];
	pid_t		pid;
	char		**args;
	t_main		data;

	// printf("%s\n", getenv("PATH"));
	data.vars = NULL;

	set(&data, strdup("a"), strdup("0000"));
	// printf("ENV: %s\n", cy = get(&data, "PATH"));
	// set(&data, strdup("PATH"), get(&data, "PATH"));
	// set(&data, strdup("PATH"), get(&data, "PATH"));
	// printf("a: %s\n", (((char **)data.vars->content)[1]));
	// printf("a: %s\n", get(&data, "array"));

	ft_lstiter(data.vars, (void (*)(void *))f);
	
	// ioctl(STDIN_FILENO, TIOCSTI, "minishell$ ``");
	// write(1, "\033[A", 3);
	ft_memset(data.flags, 0, INT8_MAX);
	// tekte tanimla
	// data.fun = malloc(sizeof(size_t (*)(t_main *data, size_t offset)) * 3);
	ft_bzero(data.increases, INT8_MAX);
	data.increases['"'] = (char)2;
	data.increases['\''] = (char)2;
	data.increases[0] = (char)0;
	data.cmds = NULL;
	data.coms = (t_com []){
		(t_com){"echo", 0, NULL},
		(t_com){"cd", 0, &data},
		(t_com){"pwd", 0, NULL},
		(t_com){"export", 0, NULL},
		(t_com){"unset", 0, NULL},
		(t_com){"env", 0, NULL},
		(t_com){"exit", sh_exit, NULL},
		(t_com){"x", sh_exit, NULL}
	};
	chdir("/");
	// printf("prog started %s\n", getcwd(NULL, 0));
	// data.coid = 1;
	signal(SIGINT, ctrl_c);
	signal(SIGQUIT, coix);
	// args = malloc(3 * sizeof(char *));
	// args[0] = strdup("/bin/ls");
	// args[1] = strdup("-la");
	// args[2] = 0;
			// getchar();
	fd = open("program.c", O_RDONLY);
	pipe(p[0]);
	pipe(p[1]);
	// read(0, data.line, 99999);
	// printf("00000000000\n");
	//printf("%s", GREEN PROMT RESET);

	// data.line = readline(GREEN PROMT RESET);
	// parser(&data);
	// if (data.line)
	// 	exe(data.coms, data.line);
	// else
	// {
	// 	free(data.line);
	// 	if (1)
	// 	{
	// 		printf("\033[A");
	// 		printf(GREEN PROMT RESET);
	// 		sh_exit(&data.coms[6]);
	// 	}
	// 	qsignal = 0;
	// 	printf("\n");
	// }
	while (1)
	{
		// printf("%s", );
		// rl_erase_empty_line = 1;

		// rl_already_prompted = 1;
		data.line = readline(GREEN PROMT RESET);
		if (data.line)
		{
			if (data.line[0] != 0)
			{
				data._ = 0;
				
				parser(&data);
				// system("valgrind --leak-check=full /Users/ahbasara/sources/repos/projects/main/murmursh-copyxd/program");
				// exe(data.coms, data.line);
			}
			free(data.line);

			exit(0);
		}
		else
		{
			free(data.line);
			if (1)
			{
				printf("\033[A");
				printf(GREEN PROMT RESET);
				fflush(stdout);
				sh_exit(&data.coms[6]);
			}
			qsignal = 0;
			printf("\n");
		}
		// rl_on_new_line();
		// free(data.line);
	}
	ft_lstiter(data.vars, (void (*)(void *))f2);
	t_list *tmp;
	while (data.vars)
	{
		tmp = data.vars->next;
		free(data.vars);
		data.vars = tmp;
	}
	
	// if (fork())
	// {

	// }
	// else
	// {
		
	// }
}

/**2.
 *  in pipe 
 * out ft_değisken
*/
// <file grep | cat >"ft_$out"
/**
 * void	__attribute__((destructor))after_main()
 * {
 * 	pause();
 * }
 */

// 🙂
// int smile()
// {
// 	return(1);
// }
// 🤔
// int think()
// {
// 	while (1)
// 	{}
// }
// 😡
// int angry()
// {
// 	return(-1);
// }
// 😴
// void sleep()
// {
// 	pause();
// }
// 😐
// char indecisive()
// {
// 	return (0);
// }
