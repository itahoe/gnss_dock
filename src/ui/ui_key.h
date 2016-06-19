/**
  * @file    ui_key.h
  * @brief   User Interface - Key
  * @author  Igor T. <research.tahoe@gmail.com>
  */

#ifndef  UI_KEY_H
#define  UI_KEY_H

#include	<stdbool.h>
#include	<stdint.h>
#include	<string.h>
#include	"config.h"
#include	"bsp.h"

#define	UI_SYSTICK_HZ                           BSP_SYSTICK_HZ

#define	UI_KEY_DEBOUNCE_TCKS                    ( ((uint32_t) CFG_UI_KEY_DEBOUNCE_mSEC * UI_SYSTICK_HZ) / 1000 )
#define	UI_KEY_SHRT_TCKS                        ( ((uint32_t) CFG_UI_KEY_SHRT_MAX_mSEC * UI_SYSTICK_HZ) / 1000 )
#define	UI_KEY_LONG_TCKS                        ( ((uint32_t) CFG_UI_KEY_LONG_MAX_mSEC * UI_SYSTICK_HZ) / 1000 )
#define	UI_KEY_REPEAT_TCKS                      ( ((uint32_t) CFG_UI_KEY_REPEAT_MAX_mSEC * UI_SYSTICK_HZ) / 1000 )

typedef	enum	ui_key_status_e
{
	UI_KEY_STS_NONE,
	UI_KEY_STS_SHORT,
	UI_KEY_STS_LONG,
	UI_KEY_STS_REPEAT,
	UI_KEY_STS_RELEASE,
}	ui_key_status_t;

typedef    void    ( * ui_key_init_t )( void );

typedef    bool    ( * ui_key_get_t )( void );

typedef	struct	ui_key_s
{
	ui_key_init_t           init;
	ui_key_get_t            get;
	ui_key_status_t         status;
	size_t                  tick;
}	ui_key_t;

bool	ui_key_hook( ui_key_t *p );

#endif	//UI_KEY_H
