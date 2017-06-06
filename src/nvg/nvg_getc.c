/**
  * @file    nvg_getc.c
  * @brief   NVG get single char
  * @author  Igor T. <research.tahoe@gmail.com>
  */

#include <stdio.h>
#include "nvg.h"


/**
 * @brief recieve single char
 */
nvg_type_t nvg_getc(                            nvg_t *                 p,
                                        const   char                    c )
{
        nvg_type_t      type    =   NVG_TYPE_INVALID;


        if(      c == '$' )
        {
                p->size =   0;
        }
        else if( c == '\r' )
        {
                type    =   nvg_recv( p, p->str_inp );
        }
        else if( c == '\n' )
        {
        }
        else if( p->size < sizeof( p->str_inp ) )
        {
                *(p->str_inp + p->size++)   =   c;
        }

	return( type );
}

