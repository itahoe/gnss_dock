/**
 * @file cli_hook.c
 * @brief	Command Line Interface
 * @author  Igor T. <research.tahoe@gmail.com>
 */

#include <stdlib.h>
#include "cli.h"

static const cli_cmd_t cli_cmd[] =
{
	{ CLI_CMD_ID_NVGLOG,    "NVGLOG",  },
};

/**
 * @brief CLI hook command.
 */
static
bool cli_hook_cmd( cli_t *p )
{
		bool	resp	=   false;
		size_t	id;
	const	size_t	id_max  =   sizeof(cli_cmd) / sizeof(cli_cmd_t);
	const	size_t	buf_max	=   sizeof(p->buf) / sizeof(cli_str_t);


	if (p->state == CLI_STATE_CMD) {
		for (id = 0; id < id_max - 1; id++) {
			int miss = strncmp( cli_cmd[id].str, p->buf, buf_max );
			if (miss == 0) {
				break;
			}
		}

		p->cmd_id       =    (cli_cmd_id_t) id;
		p->len          =    0;
		p->state        =    CLI_STATE_ARG1;
	}

	return resp;
}

/**
 * @brief CLI hook argument.
 */
static
bool cli_hook_arg( cli_t *p )
{
	bool resp;


	switch( p->state )
        {
                case CLI_STATE_ARG1:
                        p->arg  =    atoi( p->buf );
                        resp    =    true;
                        break;

                case CLI_STATE_CMD:
                        cli_hook_cmd( p );
                        resp    =    true;
                        break;

                default:
                        resp    =    false;
                        break;
	}

	p->state =    CLI_STATE_CMD;
	p->len   =    0;

	return resp;
}

/**
 * @brief CLI hook.
 */
bool cli_hook( cli_t *p, char c )
{
	bool         resp;
	const size_t buf_size = sizeof(p->buf) / sizeof(cli_str_t);


	switch( c )
        {
                case    ',':
                        *(p->buf + p->len) = '\0';
                        resp = cli_hook_cmd(p);
                        break;

                case    '\r':
                case    '\n':
                        // TODO: p->arg may still containe previous arg value
                        *(p->buf + p->len) = '\0';
                        resp = cli_hook_arg(p);
                        break;

                default:
                        if (p->len < buf_size)
                        {
                                *(p->buf + p->len++) = c;
                        }
                        resp = false;
                        break;
	}

	return resp;





/*
		case    '\n':
			resp            =   nmea_recv( &gnss->nmea, (char *) recv_buf );




	bool            resp        =   false;
	gnss_data_t *   recv        =   &gnss->recv;
	uint8_t *       recv_buf    =   recv->buf[ 0 ];


	switch( c )
	{
		case    '$':
			recv->len       =   0;
			break;

		case    '\n':
			resp            =   nmea_recv( &gnss->nmea, (char *) recv_buf );
			break;

		case    '\r':
			break;

		default:
			if( recv->len < NMEA_STRLEN_MAX_OCT-1 )
			{
				*(recv_buf + recv->len++)   =   c;
			}
			break;
	}

	return( resp );
*/




}
