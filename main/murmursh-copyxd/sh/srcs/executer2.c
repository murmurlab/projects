#include "include.h"

void	close_pipes(t_main *data, t_execd *execd)
{
	size_t													_;

	_ = 1;
	if (execd->_ != 0)
		close(execd->fd[0][1]);
	else if ((execd->_ != (data->cmd_ct - 1)) && (data->cmd_ct != 1))
		close(execd->fd[data->cmd_ct - 2][0]);
	if (execd->_ == 0 && data->cmd_ct != 1 && data->cmd_ct != 2)
	{
		close(execd->fd[_ - 1][0]);
		close(execd->fd[_][1]);
	}
	while (_ < execd->_)
	{
		close(execd->fd[_ - 1][0]);
		close(execd->fd[_][1]);
		_++;
	}
	_++;
	while (_ < data->cmd_ct - 1)
	{
		close(execd->fd[_ - 1][0]);
		close(execd->fd[_][1]);
		_++;
	}
}