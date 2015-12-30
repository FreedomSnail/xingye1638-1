/*
*********************************************************************************************************
*                                              EXAMPLE CODE
*
*                          (c) Copyright 2003-2006; Micrium, Inc.; Weston, FL
*
*               All rights reserved.  Protected by international copyright laws.
*               Knowledge of the source code may NOT be used to develop a similar product.
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                           MASTER INCLUDES
*
*                                     ST Microelectronics STM32
*                                              with the
*                                   STM3210B-EVAL Evaluation Board
*
* Filename      : includes.h
* Version       : V1.10
* Programmer(s) : BAN
*********************************************************************************************************
*/

#ifndef  __INCLUDES_H__
#define  __INCLUDES_H__

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include "math.h"

#include "globals.h"



#include "os_cfg.h"
#include "cpu_def.h"
#include "cpu.h"
#include "os_cpu.h"
#include "ucos_ii.h"




#include "stm32f10x.h"
#include "stm32f10x_conf.h"
//#include  <stm32f10x_lib.h>

#include "config.h"


#include "Task.h"


#include "m100_sdk.h"
#include "bsp.h"
#include "COM.h"

#include "sim900a.h"
#include "MX25xxx.h"
#include "MCP41xxx.h"


#include "NVIC.h"




#include "app_cfg.h"

#include "Task_Decode.h"
#include "Task_GprsProc.h"
#include "Task_Write_SN.h"


#endif


