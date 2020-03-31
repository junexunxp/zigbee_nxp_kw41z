/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/


#ifndef ANALOG_INPUT_BASIC_H
#define ANALOG_INPUT_BASIC_H

/*!
\file       AnalogInputBasic.h
\brief      Header for Analog Input Basic Cluster
*/

/****************************************************************************/
/***        Include Files                                                 ***/
/****************************************************************************/
#include <jendefs.h>
#include "zcl.h"
#include "zcl_options.h"

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/****************************************************************************/
/*             Analog Input Basic Cluster - Optional Attributes             */
/*                                                                          */
/* Add the following #define's to your zcl_options.h file to add optional   */
/* attributes to the Analog input basic cluster.                            */
/****************************************************************************/

/* Enable the optional Stack Version attribute*/
//#define   CLD_ANALOG_INPUT_BASIC_ATTR_DESCRIPTION

/* Enable the optional Max Present Value attribute */
//#define   CLD_ANALOG_INPUT_BASIC_ATTR_MAX_PRESENT_VALUE

/* Enable the optional Min Present Value attribute */
//#define   CLD_ANALOG_INPUT_BASIC_ATTR_MIN_PRESENT_VALUE

/* Enable the optional Reliability attribute */
//#define   CLD_ANALOG_INPUT_BASIC_ATTR_RELIABILITY

/* Enable the optional Resolution attribute */
//#define   CLD_ANALOG_INPUT_BASIC_ATTR_RESOLUTION

/* Enable the optional Engineering Units attribute */
//#define   CLD_ANALOG_INPUT_BASIC_ATTR_ENGINEERING_UNITS

/* Enable the optional Application Type attribute */
//#define   CLD_ANALOG_INPUT_BASIC_ATTR_APPLICATION_TYPE

/* Enable the optional Attribute Reporting Status attribute */
//#define   CLD_ANALOG_INPUT_BASIC_ATTR_ATTRIBUTE_REPORTING_STATUS

/* End of optional attributes */


/* Cluster ID's */
#define GENERAL_CLUSTER_ID_ANALOG_INPUT_BASIC                            0x000C


#ifndef CLD_ANALOG_INPUT_BASIC_CLUSTER_REVISION
    #define CLD_ANALOG_INPUT_BASIC_CLUSTER_REVISION                     1
#endif    

/* Status Flags - bit definitions and bit masks */
#define ANALOG_INPUT_BASIC_STATUS_FLAGS_IN_ALARM                              0
#define ANALOG_INPUT_BASIC_STATUS_FLAGS_FAULT                                 1
#define ANALOG_INPUT_BASIC_STATUS_FLAGS_OVERRIDDEN                            2
#define ANALOG_INPUT_BASIC_STATUS_FLAGS_OUT_OF_SERVICE                        3

#define ANALOG_INPUT_BASIC_STATUS_FLAGS_IN_ALARM_MASK                             (1 << ANALOG_INPUT_BASIC_STATUS_FLAGS_IN_ALARM)
#define ANALOG_INPUT_BASIC_STATUS_FLAGS_FAULT_MASK                                (1 << ANALOG_INPUT_BASIC_STATUS_FLAGS_FAULT)
#define ANALOG_INPUT_BASIC_STATUS_FLAGS_OVERRIDDEN_MASK                           (1 << ANALOG_INPUT_BASIC_STATUS_FLAGS_OVERRIDDEN)
#define ANALOG_INPUT_BASIC_STATUS_FLAGS_OUT_OF_SERVICE_MASK                       (1 << ANALOG_INPUT_BASIC_STATUS_FLAGS_OUT_OF_SERVICE)

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/* Command codes */


typedef enum
{
    /* Analog Input (Basic) Device Information attribute set attribute ID's (3.14.1.1.2) */
    E_CLD_ANALOG_INPUT_BASIC_ATTR_ID_DESCRIPTION                    =                0x001C, /*Optional */
    E_CLD_ANALOG_INPUT_BASIC_ATTR_ID_MAX_PRESENT_VALUE              =                0x0041, /*Optional */
    E_CLD_ANALOG_INPUT_BASIC_ATTR_ID_MIN_PRESENT_VALUE              =                0x0045, /*Optional */
    E_CLD_ANALOG_INPUT_BASIC_ATTR_ID_OUT_OF_SERVICE                 =                0x0051, /*Mandatory */
    E_CLD_ANALOG_INPUT_BASIC_ATTR_ID_PRESENT_VALUE                  =                0x0055, /* Mandatory */
    E_CLD_ANALOG_INPUT_BASIC_ATTR_ID_RELIABILITY                    =                0x0067, /*Optional */
    E_CLD_ANALOG_INPUT_BASIC_ATTR_ID_RESOLUTION                     =                0x006A, /*Optional */
    E_CLD_ANALOG_INPUT_BASIC_ATTR_ID_STATUS_FLAGS                   =                0x006F, /* Mandatory */
    E_CLD_ANALOG_INPUT_BASIC_ATTR_ID_ENGINEERING_UNITS              =                0x0075, /* Optional */
    E_CLD_ANALOG_INPUT_BASIC_ATTR_ID_APPLICATION_TYPE               =                0x0100, /*Optional */
} teCLD_AnalogInputBasicCluster_AttrID;



/* Polarity attribute */
typedef enum
{
    E_CLD_ANALOG_INPUT_BASIC_OUT_OF_SERVICE_FALSE                       =                0x0000,
    E_CLD_ANALOG_INPUT_BASIC_OUT_OF_SERVICE_TRUE
}teCLD_AnalogInputBasic_OutOfService;


/* Reliability attribute */
typedef enum
{
    E_CLD_ANALOG_INPUT_BASIC_RELIABILITY_NO_FAULT_DETECTED         =                0x0000,
    E_CLD_ANALOG_INPUT_BASIC_RELIABILITY_NO_SENSOR,
    E_CLD_ANALOG_INPUT_BASIC_RELIABILITY_OVER_RANGE,
    E_CLD_ANALOG_INPUT_BASIC_RELIABILITY_UNDER_RANGE,
    E_CLD_ANALOG_INPUT_BASIC_RELIABILITY_OPEN_LOOP,
    E_CLD_ANALOG_INPUT_BASIC_RELIABILITY_SHORTED_LOOP,
    E_CLD_ANALOG_INPUT_BASIC_RELIABILITY_NO_OUTPUT,
    E_CLD_ANALOG_INPUT_BASIC_RELIABILITY_UNRELIABLE_OTHER,
    E_CLD_ANALOG_INPUT_BASIC_RELIABILITY_PROCESS_ERROR,
    E_CLD_ANALOG_INPUT_BASIC_RELIABILITY_CONFIGURATION_ERROR       =                0x000A
 }teCLD_AnalogInputBasic_Reliability;


#ifdef ANALOG_INPUT_BASIC_SERVER
/* Analog Input Basic Cluster */
typedef struct
{
#ifdef CLD_ANALOG_INPUT_BASIC_ATTR_DESCRIPTION
    tsZCL_CharacterString       sDescription;
    zuint8                      au8Description[16];
#endif

#ifdef CLD_ANALOG_INPUT_BASIC_ATTR_MAX_PRESENT_VALUE
    zsingle                     fMaxPresentValue;
#endif

#ifdef CLD_ANALOG_INPUT_BASIC_ATTR_MIN_PRESENT_VALUE
    zsingle                     fMinPresentValue;
#endif

    zbool                       bOutOfService;
    
    zsingle                     fPresentValue;

#ifdef CLD_ANALOG_INPUT_BASIC_ATTR_RELIABILITY
    zenum8                      u8Reliability;
#endif

#ifdef CLD_ANALOG_INPUT_BASIC_ATTR_RESOLUTION
    zsingle                     fResolution;
#endif

    zbmap8                      u8StatusFlags;

#ifdef CLD_ANALOG_INPUT_BASIC_ATTR_ENGINEERING_UNITS
    zenum16                     u16EngineeringUnits;
#endif

#ifdef CLD_ANALOG_INPUT_BASIC_ATTR_APPLICATION_TYPE
    zuint32                     u32ApplicationType;
#endif

#ifdef CLD_ANALOG_INPUT_BASIC_ATTR_ATTRIBUTE_REPORTING_STATUS
    zenum8                      u8AttributeReportingStatus;
#endif
    
    zuint16                     u16ClusterRevision;
} tsCLD_AnalogInputBasic;
#endif

#ifdef ANALOG_INPUT_BASIC_CLIENT
/* Analog Input Basic Cluster */
typedef struct
{
    zuint16                     u16ClusterRevision;
} tsCLD_AnalogInputBasicClient;
#endif
/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

PUBLIC teZCL_Status eCLD_AnalogInputBasicCreateAnalogInputBasic(
                tsZCL_ClusterInstance              *psClusterInstance,
                bool_t                              bIsServer,
                tsZCL_ClusterDefinition            *psClusterDefinition,
                void                               *pvEndPointSharedStructPtr,
                uint8                              *pu8AttributeControlBits);

/****************************************************************************/
/***        External Variables                                            ***/
/****************************************************************************/
#ifdef ANALOG_INPUT_BASIC_SERVER
    extern tsZCL_ClusterDefinition sCLD_AnalogInputBasic;
    extern tsZCL_AttributeDefinition asCLD_AnalogInputBasicClusterAttributeDefinitions[];
    extern uint8 au8AnalogInputBasicAttributeControlBits[];
#endif
#ifdef ANALOG_INPUT_BASIC_CLIENT
    extern tsZCL_ClusterDefinition sCLD_AnalogInputBasicClient;
    extern tsZCL_AttributeDefinition asCLD_AnalogInputBasicClientClusterAttributeDefinitions[];
    extern uint8 au8AnalogInputBasicClientAttributeControlBits[];
#endif
/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/

#endif /* BASIC_H */
