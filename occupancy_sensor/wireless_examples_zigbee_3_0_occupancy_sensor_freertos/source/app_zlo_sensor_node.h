/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef APP_ZLO_SENSOR_NODE_H_
#define APP_ZLO_SENSOR_NODE_H_
     
/*!
\file       app_zlo_sensor_node.c
\brief      ZLO Demo : Stack <-> Light Sensor Control App Interaction(Interface)
*/     

/****************************************************************************/
/***        Include Files                                                 ***/
/****************************************************************************/
#include <jendefs.h>
#include "app_common.h"
#include "zcl_customcommand.h"
#ifdef PDM_EEPROM
 #include "app_pdm.h"
#endif

#ifdef CLD_OTA
#include "OTA.h"
#endif
/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/
/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

PUBLIC void APP_vInitialiseNode(void);
PUBLIC void APP_taskSensor(void);
PUBLIC void APP_vInitialiseTasks(void);
PUBLIC void APP_vFactoryResetRecords(void);
PUBLIC bool_t APP_bNodeIsInRunningState(void);
#ifdef CLD_OTA
PUBLIC teNODE_STATES eGetNodeState(void);
PUBLIC tsOTA_PersistedData sGetOTACallBackPersistdata(void);
#endif
/****************************************************************************/
/***        External Variables                                            ***/
/****************************************************************************/
extern bool_t bBDBJoinFailed;
extern tsDeviceDesc sDeviceDesc;
/****************************************************************************/
/****************************************************************************/
/****************************************************************************/

#endif /*APP_SENSOR_NDOE_H_*/
