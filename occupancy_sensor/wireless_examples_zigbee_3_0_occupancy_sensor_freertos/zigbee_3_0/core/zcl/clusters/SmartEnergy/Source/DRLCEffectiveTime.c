/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/*!=============================================================================
\file       DRLCEffectiveTime.c
\brief      Event Time Duration Calculation Code
==============================================================================*/


/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/

#include <jendefs.h>
#include <string.h>
#include "dlist.h"
#include "zcl.h"
#include "zcl_customcommand.h"
#include "DRLC.h"
#include "DRLC_internal.h"
#if defined(CPU_MKW41Z512VHT4) || defined(CPU_K32W042S1M2VPJ_cm4)
  #include "RNG_Interface.h"
#else
  #include "rnd_pub.h"
#endif


/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/****************************************************************************/
/***        Local Function Prototypes                                     ***/
/****************************************************************************/
PRIVATE uint32 u32GetEffectiveStartTime(
                    tsSE_DRLCLoadControlEventRecord *psLoadControlEventRecord,
                    tsZCL_EndPointDefinition        *psEndPointDefinition,
                    tsZCL_ClusterInstance           *psClusterInstance);


/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/

/****************************************************************************/
/***        Local Variables                                               ***/
/****************************************************************************/

/****************************************************************************/
/***        Local Functions                                               ***/
/****************************************************************************/

/****************************************************************************
 **
 ** NAME:       vSetRandomizationValues
 **
 ** DESCRIPTION:
 ** sets randomisation values in the event
 **
 ** PARAMETERS:                         Name                        Usage
 ** tsZCL_EndPointDefinition            *psEndPointDefinition       EP structure
 ** tsZCL_ClusterInstance               *psClusterInstance          Cluster structure
 ** tsSE_DRLCLoadControlEventRecord     *psLoadControlEventRecord   LC Record
 **
 ** RETURN:
 ** nothing
 **
 ****************************************************************************/

PUBLIC  void vSetRandomizationValues(
                tsZCL_EndPointDefinition        *psEndPointDefinition,
                tsZCL_ClusterInstance           *psClusterInstance,
                tsSE_DRLCLoadControlEventRecord *psLoadControlEventRecord)
{
    // set randomization values
    uint8 u8RandomizeStartTimeInMinutes;
    uint8 u8RandomizeStopTimeInMinutes;

    // initialise for client and server
    psLoadControlEventRecord->u8StartRandomizeMinutes = 0;
    psLoadControlEventRecord->u8StopRandomizeMinutes = 0;

    // don't use in the server
    if(psClusterInstance->bIsServer==TRUE)
    {
        return;
    }

    if(psLoadControlEventRecord->sLoadControlEvent.u8EventControl & SE_DRLC_CONTROL_RANDOMISATION_START_TIME_MASK)
    {
        eZCL_GetLocalAttributeValue(E_CLD_DRLC_START_RANDOMIZE_MINUTES, FALSE, TRUE,psEndPointDefinition, psClusterInstance, &u8RandomizeStartTimeInMinutes);
        psLoadControlEventRecord->u8StartRandomizeMinutes = (uint8)RND_u32GetRand(0, u8RandomizeStartTimeInMinutes);
    }

    if(psLoadControlEventRecord->sLoadControlEvent.u8EventControl & SE_DRLC_CONTROL_RANDOMISATION_STOP_TIME_MASK)
    {
        eZCL_GetLocalAttributeValue(E_CLD_DRLC_STOP_RANDOMIZE_MINUTES, FALSE,TRUE, psEndPointDefinition, psClusterInstance, &u8RandomizeStopTimeInMinutes);
        psLoadControlEventRecord->u8StopRandomizeMinutes = (uint8)RND_u32GetRand(0, u8RandomizeStopTimeInMinutes);
    }

}

/****************************************************************************
 **
 ** NAME:       boInEffectiveTimeExtendedEventOperation
 **
 ** DESCRIPTION:
 ** Checks whether the event is is extended time operation due to randomisation
 **
 ** PARAMETERS:                     Name                        Usage
 ** tsSE_DRLCLoadControlEvent       *psLoadControlEventRecord   LC Record
 ** uint32                          u32UTCtime                  present time
 **
 ** RETURN:
 ** true/false
 **
 ****************************************************************************/

PUBLIC  bool_t boInEffectiveTimeExtendedEventOperation(
               tsSE_DRLCLoadControlEvent  *psLoadControlEvent,
               uint32                      u32UTCtime)
{

    if((psLoadControlEvent->u32StartTime + psLoadControlEvent->u16DurationInMinutes*60) > u32UTCtime)
    {
        // no it isn't
        return(FALSE);
    }

    // yes it is
    return(TRUE);

}

/****************************************************************************
 **
 ** NAME:       boInEffectiveActiveTimeCheck
 **
 ** DESCRIPTION:
 ** Checks if the Event is in the active time period
 **
 ** PARAMETERS:                         Name                    Usage
 ** tsSE_DRLCLoadControlEventRecord *psLoadControlEventRecord  LC Record
 ** tsZCL_EndPointDefinition        *psEndPointDefinition      EP structure
 ** tsZCL_ClusterInstance           *psClusterInstance         Cluster structure
 ** uint32                           u32UTCtime                present time
 **
 ** RETURN:
 ** true/false
 **
 ****************************************************************************/

PUBLIC  bool boInEffectiveActiveTimeCheck(
               tsSE_DRLCLoadControlEventRecord *psLoadControlEventRecord,
               tsZCL_EndPointDefinition   *psEndPointDefinition,
               tsZCL_ClusterInstance      *psClusterInstance,
               uint32                      u32UTCtime)
{
    uint32 u32EffectiveStartTime;

    // assuming start random is included in this calculation - is it ????
    // this needs clarification
    u32EffectiveStartTime= u32GetEffectiveStartTime(psLoadControlEventRecord, psEndPointDefinition, psClusterInstance);

    if((u32EffectiveStartTime <= u32UTCtime) &&
       ((u32EffectiveStartTime + psLoadControlEventRecord->sLoadControlEvent.u16DurationInMinutes*60) > u32UTCtime)
    )
    {
        // in active time bound
        return(TRUE);
    }

    // not in active time bound
    return FALSE;

}

/****************************************************************************
 **
 ** NAME:       boEffectiveExpiredTimeCheck
 **
 ** DESCRIPTION:
 ** checks if event has expired - accounts for randomisation
 **
 ** PARAMETERS:                     Name                        Usage
 ** tsSE_DRLCLoadControlEventRecord *psLoadControlEventRecord   LC Record
 ** tsZCL_EndPointDefinition        *psEndPointDefinition       EP structure
 ** tsZCL_ClusterInstance           *psClusterInstance          Cluster structure
 ** uint32                           u32UTCtime                 present time
 **
 ** RETURN:
 ** true/false
 **
 ****************************************************************************/

PUBLIC  bool boEffectiveExpiredTimeCheck(
               tsSE_DRLCLoadControlEventRecord  *psLoadControlEventRecord,
               tsZCL_EndPointDefinition         *psEndPointDefinition,
               tsZCL_ClusterInstance            *psClusterInstance,
               uint32                           u32UTCtime)
{

    if(u32EffectiveDuration(psLoadControlEventRecord, psEndPointDefinition, psClusterInstance) > u32UTCtime)
    {
        // time not reached yet
        return(FALSE);
    }

    // expired
    return TRUE;

}

/****************************************************************************
 **
 ** NAME:       u32EffectiveDuration
 **
 ** DESCRIPTION:
 ** calculates effective duration including randomisation
 **
 ** PARAMETERS:                         Name                    Usage
 ** tsSE_DRLCLoadControlEventRecord *psLoadControlEventRecord  LC Record
 ** tsZCL_EndPointDefinition        *psEndPointDefinition      EP structure
 ** tsZCL_ClusterInstance           *psClusterInstance         Cluster structure
 **
 ** RETURN:
 ** uint32 time
 **
 ****************************************************************************/

PUBLIC  uint32 u32EffectiveDuration(
               tsSE_DRLCLoadControlEventRecord  *psLoadControlEventRecord,
               tsZCL_EndPointDefinition         *psEndPointDefinition,
               tsZCL_ClusterInstance            *psClusterInstance)
{
    uint8 u8RandomizationTimeInMinutes=0;

    // only use randomization if its a client
    if(psClusterInstance->bIsServer==FALSE)
    {
        u8RandomizationTimeInMinutes = psLoadControlEventRecord->u8StartRandomizeMinutes + psLoadControlEventRecord->u8StopRandomizeMinutes;
    }

    return (psLoadControlEventRecord->sLoadControlEvent.u32StartTime +
           (psLoadControlEventRecord->sLoadControlEvent.u16DurationInMinutes + u8RandomizationTimeInMinutes)*60);

}

/****************************************************************************
 **
 ** NAME:       boCancelTimeCheck
 **
 ** DESCRIPTION:
 ** calculates event time to be cancelled and compares it to present value
 **
 ** PARAMETERS:                     Name                        Usage
 ** tsSE_DRLCLoadControlEventRecord *psLoadControlEventRecord   LC Record
 ** tsZCL_EndPointDefinition        *psEndPointDefinition       EP structure
 ** tsZCL_ClusterInstance           *psClusterInstance          Cluster structure
 ** uint32                           u32UTCtime                 present Time
 **
 ** RETURN:
 ** uint32 time
 **
 ****************************************************************************/

PUBLIC  uint32 boCancelTimeCheck(
               tsSE_DRLCLoadControlEventRecord  *psLoadControlEventRecord,
               tsZCL_EndPointDefinition         *psEndPointDefinition,
               tsZCL_ClusterInstance            *psClusterInstance,
               uint32                            u32UTCtime)
{
    uint8 u8RandomizationTimeInMinutes=0;
    uint32 u32EffectiveCancelTime;

    // only use randomization if its a client
    if((psClusterInstance->bIsServer==FALSE) && (psLoadControlEventRecord->eCancelControl == E_SE_DRLC_CANCEL_CONTROL_USE_RANDOMISATION))
    {
        u8RandomizationTimeInMinutes = psLoadControlEventRecord->u8StartRandomizeMinutes + psLoadControlEventRecord->u8StopRandomizeMinutes;
    }

    u32EffectiveCancelTime =  psLoadControlEventRecord->u32EffectiveCancelTime + u8RandomizationTimeInMinutes*60;

    if(u32EffectiveCancelTime > u32UTCtime)
    {
        // time not reached yet
        return(FALSE);
    }

    // expired
    return TRUE;

}

/****************************************************************************
 **
 ** NAME:       u32GetEffectiveStartTime
 **
 ** DESCRIPTION:
 ** Gets the event effective start time, taking into account of randomisation
 **
 ** PARAMETERS:                         Name                        Usage
 ** tsSE_DRLCLoadControlEventRecord     *psLoadControlEventRecord   LC Record
 ** tsZCL_EndPointDefinition            *psEndPointDefinition       EP structure
 ** tsZCL_ClusterInstance               *psClusterInstance          Cluster structure
 **
 ** RETURN:
 ** uint32 time
 **
 ****************************************************************************/

PRIVATE  uint32 u32GetEffectiveStartTime(
               tsSE_DRLCLoadControlEventRecord  *psLoadControlEventRecord,
               tsZCL_EndPointDefinition         *psEndPointDefinition,
               tsZCL_ClusterInstance            *psClusterInstance)
{

    uint8 u8StartRandomizeMinutes=0;

    // only use randomization if its a client
    if(psClusterInstance->bIsServer==FALSE)
    {
        if(psLoadControlEventRecord->sLoadControlEvent.u8EventControl & SE_DRLC_CONTROL_RANDOMISATION_START_TIME_MASK)
        {
             u8StartRandomizeMinutes = psLoadControlEventRecord->u8StartRandomizeMinutes;
        }
    }

    return psLoadControlEventRecord->sLoadControlEvent.u32StartTime + (u8StartRandomizeMinutes*60);

}

/****************************************************************************
 **
 ** NAME:       u32GetEffectiveStopTime
 **
 ** DESCRIPTION:
 ** Gets the event effective stop time, taking into account of randomisation
 **
 ** PARAMETERS:                         Name                        Usage
 ** tsSE_DRLCLoadControlEventRecord     *psLoadControlEventRecord   LC Record
 ** tsZCL_EndPointDefinition            *psEndPointDefinition       EP structure
 ** tsZCL_ClusterInstance               *psClusterInstance          Cluster structure
 **
 ** RETURN:
 ** uint32 time
 **
 ****************************************************************************/
/* Not currently used but may be needed with more randomisation support
PRIVATE  uint32 u32GetEffectiveStopTime(
               tsSE_DRLCLoadControlEventRecord  *psLoadControlEventRecord,
               tsZCL_EndPointDefinition         *psEndPointDefinition,
               tsZCL_ClusterInstance            *psClusterInstance)
{

    uint8 u8StopRandomizeMinutes=0;
    uint32 u32EffectiveStopTime;

    // only use randomization if its a client
    if(psClusterInstance->bIsServer==FALSE)
    {
        if(psLoadControlEventRecord->sLoadControlEvent.u8EventControl & SE_DRLC_CONTROL_RANDOMISATION_STOP_TIME_MASK)
        {
             u8StopRandomizeMinutes = psLoadControlEventRecord->u8StopRandomizeMinutes;
        }
    }

    u32EffectiveStopTime = u32GetEffectiveStartTime(psLoadControlEventRecord, psEndPointDefinition, psClusterInstance);
    u32EffectiveStopTime +=(u8StopRandomizeMinutes + psLoadControlEventRecord->sLoadControlEvent.u16DurationInMinutes)*60;

    return u32EffectiveStopTime;

}
*/
/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/

