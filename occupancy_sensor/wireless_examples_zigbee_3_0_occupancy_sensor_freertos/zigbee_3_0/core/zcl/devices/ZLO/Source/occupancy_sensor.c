/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/
     
/*!=============================================================================
\file       occupancy_sensor.c
\brief      ZigBee Occupancy Sensor profile functions
==============================================================================*/

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include <jendefs.h>
#include <string.h>
#include "zps_apl.h"
#include "zcl_heap.h"
#include "AliIotSecurity.h"
#include "occupancy_sensor.h"
/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/****************************************************************************/
/***        Local Function Prototypes                                     ***/
/****************************************************************************/

/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/

/****************************************************************************/
/***        Local Variables                                               ***/
/****************************************************************************/

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/
/****************************************************************************
 *
 * NAME: eZLO_RegisterOccupancySensorEndPoint
 *
 * DESCRIPTION:
 * Registers an occupancy sensor device with the ZCL layer
 *
 * PARAMETERS:  Name                            Usage
 *              u8EndPointIdentifier            Endpoint being registered
 *              cbCallBack                      Pointer to endpoint callback
 *              psDeviceInfo                    Pointer to struct containing
 *                                              data for endpoint
 *
 * RETURNS:
 * teZCL_Status
 *
 ****************************************************************************/
PUBLIC teZCL_Status eZLO_RegisterOccupancySensorEndPoint(uint8 u8EndPointIdentifier,
                                              tfpZCL_ZCLCallBackFunction cbCallBack,
                                              tsZLO_OccupancySensorDevice *psDeviceInfo)
{

    /* Fill in end point details */
    psDeviceInfo->sEndPoint.u8EndPointNumber = u8EndPointIdentifier;
    psDeviceInfo->sEndPoint.u16ManufacturerCode = ZCL_MANUFACTURER_CODE;
    psDeviceInfo->sEndPoint.u16ProfileEnum = HA_PROFILE_ID;
    psDeviceInfo->sEndPoint.bIsManufacturerSpecificProfile = FALSE;
    psDeviceInfo->sEndPoint.u16NumberOfClusters = sizeof(tsZLO_OccupancySensorDeviceClusterInstances) / sizeof(tsZCL_ClusterInstance);
    psDeviceInfo->sEndPoint.psClusterInstance = (tsZCL_ClusterInstance*)&psDeviceInfo->sClusterInstance;
    psDeviceInfo->sEndPoint.bDisableDefaultResponse = ZCL_DISABLE_DEFAULT_RESPONSES;
    psDeviceInfo->sEndPoint.pCallBackFunctions = cbCallBack;

    /* Mandatory server clusters */
    #if (defined CLD_BASIC) && (defined BASIC_SERVER)
        /* Create an instance of a Basic cluster as a server */
        if(eCLD_BasicCreateBasic(&psDeviceInfo->sClusterInstance.sBasicServer,
                              TRUE,
                              &sCLD_Basic,
                              &psDeviceInfo->sBasicServerCluster,
                              &au8BasicClusterAttributeControlBits[0]) != E_ZCL_SUCCESS)
        {
            // Need to convert from cluster specific to ZCL return type so we lose the extra information of the return code
            return E_ZCL_FAIL;
        } 
    #endif

    #if (defined CLD_IDENTIFY) && (defined IDENTIFY_SERVER)
        /* Create an instance of an Identify cluster as a server */
        if(eCLD_IdentifyCreateIdentify(&psDeviceInfo->sClusterInstance.sIdentifyServer,
                              TRUE,
                              &sCLD_Identify,
                              &psDeviceInfo->sIdentifyServerCluster,
                              &au8IdentifyAttributeControlBits[0],
                              &psDeviceInfo->sIdentifyServerCustomDataStructure) != E_ZCL_SUCCESS)
        {
            // Need to convert from cluster specific to ZCL return type so we lose the extra information of the return code
            return E_ZCL_FAIL;
        }    
    #endif

    #if (defined CLD_OCCUPANCY_SENSING) && (defined OCCUPANCY_SENSING_SERVER)
        /* Create an instance of an Occupancy Sensing cluster as a server */
        eCLD_OccupancySensingCreateOccupancySensing(
                              &psDeviceInfo->sClusterInstance.sOccupancySensingServer,
                              TRUE,
                              &sCLD_OccupancySensing,
                              &psDeviceInfo->sOccupancySensingServerCluster,
                              &au8OccupancySensingAttributeControlBits[0]);
    #endif

    /* Optional server clusters */
    #if (defined CLD_POLL_CONTROL) && (defined POLL_CONTROL_SERVER)
        /* Create an instance of a Poll Control cluster as a server */
        if(eCLD_PollControlCreatePollControl(
                              &psDeviceInfo->sClusterInstance.sPollControlServer,
                              TRUE,
                              &sCLD_PollControl,
                              &psDeviceInfo->sPollControlServerCluster,
                              &au8PollControlAttributeControlBits[0],
                              &psDeviceInfo->sPollControlServerCustomDataStructure) != E_ZCL_SUCCESS)
        {
            // Need to convert from cluster specific to ZCL return type so we lose the extra information of the return code
            return E_ZCL_FAIL;
        }    
    #endif

    /* Mandatory client clusters */
    #if (defined CLD_IDENTIFY) && (defined IDENTIFY_CLIENT)
        /* Create an instance of an Identify cluster as a client */
        if(eCLD_IdentifyCreateIdentify(&psDeviceInfo->sClusterInstance.sIdentifyClient,
                              FALSE,
                              &sCLD_Identify,
                              &psDeviceInfo->sIdentifyClientCluster,
                              &au8IdentifyAttributeControlBits[0],
                              &psDeviceInfo->sIdentifyClientCustomDataStructure) != E_ZCL_SUCCESS)
        {
            // Need to convert from cluster specific to ZCL return type so we lose the extra information of the return code
            return E_ZCL_FAIL;
        }    
    #endif

    /* Recommended Optional client clusters */
    #if (defined CLD_GROUPS) && (defined GROUPS_CLIENT)
        /* Create an instance of a Groups cluster as a client */
        if(eCLD_GroupsCreateGroups(&psDeviceInfo->sClusterInstance.sGroupsClient,
                              FALSE,
                              &sCLD_Groups,
                              &psDeviceInfo->sGroupsClientCluster,
                              &au8GroupsAttributeControlBits[0],
                              &psDeviceInfo->sGroupsClientCustomDataStructure,
                              &psDeviceInfo->sEndPoint) != E_ZCL_SUCCESS)
        {
            // Need to convert from cluster specific to ZCL return type so we lose the extra information of the return code
            return E_ZCL_FAIL;
        } 
    #endif

    #if(defined CLD_OTA) && (defined OTA_CLIENT)
       if (eOTA_Create(
           &psDeviceInfo->sClusterInstance.sOTAClient,
           FALSE,  /* client */
           &sCLD_OTA,
           &psDeviceInfo->sCLD_OTA,  /* cluster definition */
           u8EndPointIdentifier,
           au8OTAAttributeControlBits,
           &psDeviceInfo->sCLD_OTA_CustomDataStruct
           )!= E_ZCL_SUCCESS)

        {
            // Need to convert from cluster specific to ZCL return type so we lose the extra information of the return code
            return E_ZCL_FAIL;
        }
    #endif
#if (defined CLD_ALIIOTSECURITY) && (defined ALIIOTSECURITY_SERVER)
	/* Create an instance of a On/Off cluster as a server */
	if(eCLD_AliIotSecurityCreateAliIotSecurity(&psDeviceInfo->sClusterInstance.sAliIotSecurityServer,
		TRUE,
		&sCLD_AliIotSecurity,
		&psDeviceInfo->sAliIotSecurityServerCluster,
		&au8AliIotSecurityAttributeControlBits[0],
		NULL) != E_ZCL_SUCCESS)
	{
		// Need to convert from cluster specific to ZCL return type so we lose the extra information of the return code
		return E_ZCL_FAIL;
	}
#endif

    return eZCL_Register(&psDeviceInfo->sEndPoint);
}

/****************************************************************************/
/***        Local Functions                                               ***/
/****************************************************************************/

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/

