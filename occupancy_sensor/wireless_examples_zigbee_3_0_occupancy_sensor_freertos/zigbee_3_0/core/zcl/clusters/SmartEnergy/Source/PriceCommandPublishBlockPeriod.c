/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/*!=============================================================================
\file       PriceCommandPublishBlockPeriod.c
\brief      Send a publish Block Period command
==============================================================================*/
#include "zcl_options.h"

#ifdef CLD_PRICE
/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include <jendefs.h>

#include "zcl.h"
#include "zcl_customcommand.h"

#include "Price.h"
#include "Price_internal.h"

#include "pdum_apl.h"
#include "zps_apl.h"
#include "zps_apl_af.h"



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
/***        Public Functions                                              ***/
/****************************************************************************/

/****************************************************************************
 **
 ** NAME:       eSE_PricePublishBlockPeriodSend
 **
 ** DESCRIPTION:
 ** Builds and sends the Publish Block Period command
 **
 ** PARAMETERS:                 Name                           Usage
 ** uint8                       u8SourceEndPointId             Source EP Id
 ** uint8                       u8DestinationEndPointId        Destination EP Id
 ** tsZCL_Address              *psDestinationAddress           Destination Address
 ** uint8                      *pu8TransactionSequenceNumber   Sequence number Pointer
 ** tsSE_PricePublishBlockPeriodCmdPayload   *psPublishBlockPeriodCmdPayload       Message contents structure
 **
 ** RETURN:
 ** teSE_PriceStatus
 **
 ****************************************************************************/

PUBLIC  teSE_PriceStatus eSE_PricePublishBlockPeriodSend(
                    uint8                       u8SourceEndPointId,
                    uint8                       u8DestinationEndPointId,
                    tsZCL_Address              *psDestinationAddress,
                    uint8                      *pu8TransactionSequenceNumber,
                    tsSE_PricePublishBlockPeriodCmdPayload   *psPublishBlockPeriodCmdPayload)
{

    //uint32 u32UTCTime;

    tsZCL_TxPayloadItem asPayloadDefinition[] = {
            {1, E_ZCL_UINT32,   &psPublishBlockPeriodCmdPayload->u32ProviderId},
            {1, E_ZCL_UINT32,   &psPublishBlockPeriodCmdPayload->u32IssuerEventId},
            {1, E_ZCL_UINT32,   &psPublishBlockPeriodCmdPayload->u32BlockPeriodStartTime},
            {1, E_ZCL_UINT24,   &psPublishBlockPeriodCmdPayload->u32BlockPeriodDurationInMins},
            {1, E_ZCL_BMAP8,    &psPublishBlockPeriodCmdPayload->b8NoOfTiersAndNoOfBlockThreshold},
            {1, E_ZCL_BMAP8,    &psPublishBlockPeriodCmdPayload->b8BlockPeriodControl},
                                            };

    // Time must be synchronised as current time is sent in the publish message
    if(!bZCL_GetTimeHasBeenSynchronised())
    {
        return(E_ZCL_ERR_TIME_NOT_SYNCHRONISED);
    }

    // get time
    //u32UTCTime = u32ZCL_GetUTCTime();
    return eZCL_CustomCommandSend(u8SourceEndPointId,
                                  u8DestinationEndPointId,
                                  psDestinationAddress,
                                  SE_CLUSTER_ID_PRICE,
                                  TRUE,
                                  E_SE_PUBLISH_BLOCK_PERIOD,
                                  pu8TransactionSequenceNumber,
                                  asPayloadDefinition,
                                  FALSE,
                                  0,
                                  sizeof(asPayloadDefinition) / sizeof(tsZCL_TxPayloadItem));
}

#endif /* CLD_PRICE */
/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/