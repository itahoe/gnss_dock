/**
  * @file    ui_key_hook.c
  * @brief   UI - Key Services
  * @author  Igor T. <research.tahoe@gmail.com>
  */

#include	"ui_key.h"

/**
 * @brief UI - Key hook.
 */
bool	ui_key_hook( ui_key_t *p )
{
	bool            resp                    =   false;
	bool            forced                  =   p->get();
        const   size_t  debounce_max_ticks      =   ((size_t) CFG_UI_KEY_DEBOUNCE_mSEC * CFG_UI_SYSTICK_HZ) / 1000;
        const   size_t  short_max_ticks         =   ((size_t) CFG_UI_KEY_SHRT_MAX_mSEC * CFG_UI_SYSTICK_HZ) / 1000;
        const   size_t  long_max_ticks          =   ((size_t) CFG_UI_KEY_LONG_MAX_mSEC * CFG_UI_SYSTICK_HZ) / 1000;



	if( forced == true )
	{
		p->tick++;

		if( p->tick == long_max_ticks )
		{
			p->status       =    UI_KEY_STS_LONG;
			resp            =    true;
		}
	}
	else
	{
		if( p->tick > 0 )
		{
			p->status       =   p->tick < debounce_max_ticks ? UI_KEY_STS_NONE :
                                                   p->tick < short_max_ticks  ? UI_KEY_STS_SHORT : UI_KEY_STS_RELEASE;

			p->tick		=	0;

			if( p->status != UI_KEY_STS_NONE )
			{
				resp            =    true;
			}
		}
	}

	return( resp );
}
