/**
  * @file    nvg.h
  * @brief   proprietary NVG rotocol
  * @author  Igor T. <research.tahoe@gmail.com>
  */

#ifndef  NVG_H
#define  NVG_H

/*

//-----------------------------------------------------------------------
��������� ������ �����������
//-----------------------------------------------------------------------
	a. ��� ������ ������� �� BT1
	$PNVGLOG,MODE,1*<CRC>, ��� <CRC> - ����������� �����
� MCU ��������� ���������� �� ������������� ����������� ������ NMEA �
����� UART1 ��������� RTK
	������������� $PNVGLOG,MODE,1*<CRC>

	�. $PNVGLOG,MODE,2*<CRC>
� MCU ��������� ���������� �� ������������� ����������� ������ DEBUG �
����� UART2 ��������� RTK.
	������������� $PNVGLOG,MODE,2*<CRC>

	�. $PNVGLOG,MODE,3*<CRC>
� MCU ��������� ���������� �� ������������� ����������� ������ NMEA �
����� UART1 � DEBUG � ����� UART2 (����� ������ ���� ���������������� MCU).
	������������� $PNVGLOG,MODE,3*<CRC>
��� ��������� ����������� � ���������� ������.

��� ������� $PNVGLOG,MODE*<CRC> � ���� ������ ���������� ����� � �������
���������� $PNVGLOG,MODE,N*<CRC>

���� SD ������ SD ����� � ����������� ������� �� ������ "����"
�������������� ����������� ���������� � ������������ � ������� �������:
	����� 1 - ���� ���� NMEA

	����� 2 - ���� ���� DBG
	���� UART2 ����e� ���� ���������������� MCU �������� -
$PNVGRZA,2,230400,9,0

	����� 3 - ��� ����� NMEA + DBG
	���� UART2 ����e� ���� ���������������� MCU �������� -
$PNVGRZA,2,230400,9,0

��� ��������� ������� �� ������ "����", ���� ���������� �������, ����
"������������" ������ - �������������� ��������� ����������� ������ ���
���� ������ � ������� ������ UART1/2 ��������� RTK � ����� ������ ��
���������:
	UART1 - PNVGRZA,1,115200,1
	UART2 - PNVGRZA,2,115200,1

� ������ ���� SD ����� �� ��������� �� �����-�� �������� ��������������
���������������� ������� 3 ���� ����������� (SD) � �������� 2 ��� �
����� ������������������ 20 ������ ���� ���� �� ����� ��������� SD �����.

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
        //NVG_TYPE_LOG_MODE,
        NVG_TYPE_LOG_MODE_GET,
        NVG_TYPE_LOG_MODE_SET,
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
