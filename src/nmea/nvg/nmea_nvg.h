/**
  * @file    nmea_nvg.h
  * @brief   NMEA proprietary NVG service
  * @author  Igor T. <research.tahoe@gmail.com>
  */

#ifndef  NMEA_NVG_H
#define  NMEA_NVG_H

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

//-----------------------------------------------------------------------
*/


#include <stdbool.h>
#include <string.h>
#include "config.h"


typedef enum    nmea_nvg_log_mode_s
{
        NMEA_NVG_LOG_MODE_0,
        NMEA_NVG_LOG_MODE_1,
        NMEA_NVG_LOG_MODE_2,
        NMEA_NVG_LOG_MODE_3,
} nmea_nvg_log_mode_t;

typedef	struct	nmea_nvg_log_s
{
        nmea_nvg_log_mode_t     mode;
} nmea_nvg_log_t;


typedef	struct	nmea_nvg_ver_s
{
        size_t                  num;
} nmea_nvg_ver_t;

typedef	struct	nmea_nvg_bss_s
{
        size_t                  dummy;
} nmea_nvg_bss_t;

typedef	struct	nmea_nvg_s
{
        nmea_nvg_log_t          log;
        nmea_nvg_ver_t          ver;
        nmea_nvg_bss_t          bss;
} nmea_nvg_t;


bool nmea_nvg_recv(                         nmea_nvg_t *        p,
                                    const   char *              str );

bool nmea_nvg_recv_log(                     nmea_nvg_log_t *    p,
                                    const   char *              str );

bool nmea_nvg_recv_ver(                     nmea_nvg_ver_t *    p,
                                    const   char *              str );

bool nmea_nvg_recv_bss(                     nmea_nvg_bss_t *    p,
                                    const   char *              str );


#endif	//NMEA_NVG_H
