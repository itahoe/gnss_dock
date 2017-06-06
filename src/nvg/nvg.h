/**
  * @file    nvg.h
  * @brief   proprietary NVG rotocol
  * @author  Igor T. <research.tahoe@gmail.com>
  */

#ifndef  NVG_H
#define  NVG_H

/*

//-----------------------------------------------------------------------
Настройка режима логирования
//-----------------------------------------------------------------------
	a. При подаче команды по BT1
	$PNVGLOG,MODE,1*<CRC>, где <CRC> - контрольная сумма
в MCU заносится информация об инициализации логирования потока NMEA с
порта UART1 приемника RTK
	Подтверждение $PNVGLOG,MODE,1*<CRC>

	б. $PNVGLOG,MODE,2*<CRC>
в MCU заносится информация об инициализации логирования потока DEBUG с
порта UART2 приемника RTK.
	Подтверждение $PNVGLOG,MODE,2*<CRC>

	в. $PNVGLOG,MODE,3*<CRC>
в MCU заносится информация об инициализации логирования потока NMEA с
порта UART1 и DEBUG с порта UART2 (Порты должны быть сконфигурированы MCU).
	Подтверждение $PNVGLOG,MODE,3*<CRC>
Все настройки сохраняются в постоянную память.

При запросе $PNVGLOG,MODE*<CRC> в порт должен выдаваться ответ с текущей
настройкой $PNVGLOG,MODE,N*<CRC>

Если SD холдер SD карты и последующем нажатии на кнопку "меню"
осуществляется логирование информации в соответствии с текущим режимом:
	Режим 1 - один файл NMEA

	Режим 2 - один файл DBG
	Порт UART2 должeн быть сконфигурированы MCU командой -
$PNVGRZA,2,230400,9,0

	Режим 3 - два файла NMEA + DBG
	Порт UART2 должeн быть сконфигурированы MCU командой -
$PNVGRZA,2,230400,9,0

При повторном нажатии на кнопку "меню", либо отключении питания, либо
"выдергивании" флешки - осуществляется остановка логирования данных для
всех файлов и перевод портов UART1/2 приемника RTK в режим работы по
умолчанию:
	UART1 - PNVGRZA,1,115200,1
	UART2 - PNVGRZA,2,115200,1

В случае если SD карта не вставлена по каким-то причинам осуществляется
последовательное мигание 3 раза светодиодом (SD) и ожидание 2 сек и
такая последовательность 20 секунд либо пока не будет вставлена SD карта.

$PNVGRZA,2,230400,9,0
$PNVGCFG,W

$PNVGRZA,0,230400,9,0


//-----------------------------------------------------------------------
*/


#include <stdbool.h>
#include <string.h>
#include "config.h"


typedef enum    nvg_type_s
{
        NVG_TYPE_INVALID,
        NVG_TYPE_BSS,
        NVG_TYPE_VER,
        NVG_TYPE_LOG_MODE,
        NVG_TYPE_LOG_CLOSE,
        NVG_TYPE_LOG_OPEN,
        NVG_TYPE_LOG_RESTART,
} nvg_type_t;

typedef enum    nvg_log_mode_s
{
        NVG_LOG_MODE_INVALID,
        NVG_LOG_MODE_1,
        NVG_LOG_MODE_2,
        NVG_LOG_MODE_3,
} nvg_log_mode_t;

typedef	struct	nvg_log_s
{
        nvg_log_mode_t          mode;
} nvg_log_t;


typedef	struct	nvg_ver_s
{
        size_t                  num;
} nvg_ver_t;

typedef	struct	nvg_bss_s
{
        size_t                  dummy;
} nvg_bss_t;

typedef	struct	nvg_s
{
        nvg_log_t               log;
        nvg_ver_t               ver;
        nvg_bss_t               bss;
        size_t                  size;
        char                    data[80];
        char                    str_inp[80];
        char                    str_out[80];
} nvg_t;


nvg_type_t nvg_recv(                            nvg_t *                 p,
                                        const   char *                  str );

nvg_type_t nvg_getc(                            nvg_t *                 p,
                                        const   char                    c );

void nvg_resp(                                  nvg_t *                 p,
                                        const   nvg_log_mode_t          mode );


#endif	//NVG_H
