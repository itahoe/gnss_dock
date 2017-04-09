/**
  * @file    nmea_nvg.h
  * @brief   NMEA proprietary NVG service
  * @author  Igor T. <research.tahoe@gmail.com>
  */

#ifndef  NMEA_NVG_H
#define  NMEA_NVG_H

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
