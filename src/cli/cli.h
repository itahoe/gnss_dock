/**
 * @file cli.c
 * @brief	Command Line Interface
 * @author  Igor T. <research.tahoe@gmail.com>
 */


#ifndef  CLI_H
#define  CLI_H

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "config.h"

typedef char cli_str_t;

typedef enum    cli_cmd_id_e
{
	CLI_CMD_ID_NVGLOG = 0,
} cli_cmd_id_t;

typedef enum    cli_state_e
{
	CLI_STATE_CMD = 0,
	CLI_STATE_ARG1,
	CLI_STATE_ARG2,
} cli_state_t;

typedef struct  cli_struct_s
{
	cli_state_t     state;
	cli_cmd_id_t    cmd_id;
	size_t          len;
	int32_t         arg;
	cli_str_t buf[ CFG_CLI_BUF_SIZE_OCT ];
} cli_t;

typedef struct  cli_cmd_s
{
	cli_cmd_id_t           id;
	cli_str_t *            str;
} cli_cmd_t;


void cli_init(                                  cli_t *                 p );

bool cli_hook(                                  cli_t *                 p,
                                                char                    c );

void cli_send(                                  char *                  data );

#endif	//CLI_H
