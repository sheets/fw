/**************************************************************************************************
Filename:       bcservice.c
Revised:        $Date: 2010-08-06 08:56:11 -0700 (Fri, 06 Aug 2010) $
Revision:       $Revision: 23333 $

Description:    This file contains the proprietary Beacon Config Service

Copyright 2010 - 2013 Texas Instruments Incorporated. All rights reserved.

IMPORTANT: Your use of this Software is limited to those specific rights
granted under the terms of a software license agreement between the user
who downloaded the software, his/her employer (which must be your employer)
and Texas Instruments Incorporated (the "License").  You may not use this
Software unless you agree to abide by the terms of the License. The License
limits your use, and you acknowledge, that the Software may not be modified,
copied or distributed unless embedded on a Texas Instruments microcontroller
or used solely and exclusively in conjunction with a Texas Instruments radio
frequency transceiver, which is integrated into your product.  Other than for
the foregoing purpose, you may not use, reproduce, copy, prepare derivative
works of, modify, distribute, perform, display or sell this Software and/or
its documentation for any purpose.

YOU FURTHER ACKNOWLEDGE AND AGREE THAT THE SOFTWARE AND DOCUMENTATION ARE
PROVIDED “AS IS” WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED,
INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY, TITLE,
NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT SHALL
TEXAS INSTRUMENTS OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER CONTRACT,
NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR OTHER
LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE
OR CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT
OF SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.

Should you have any questions regarding your right to use this Software,
contact Texas Instruments Incorporated at www.TI.com.
**************************************************************************************************/

/*********************************************************************
* INCLUDES
*/
#include "bcomdef.h"
#include "OSAL.h"
#include "linkdb.h"
#include "att.h"
#include "gatt.h"
#include "gatt_uuid.h"
#include "gattservapp.h"
#include "gapbondmgr.h"

#include "bcservice.h"
#include "st_util.h"
/*********************************************************************
* MACROS
*/
#if !defined(FEATURE_OAD)
// Characteristic Descriptors don't fit on the OAD images, so only
// allow them on CC2541DK-Sensor Build 
#define USE_BEACON_DESCRIPTORS
#endif
/*********************************************************************
* CONSTANTS
*/

/*********************************************************************
* TYPEDEFS
*/

/*********************************************************************
* GLOBAL VARIABLES
*/
// Simple GATT Profile Service UUID: 0xBCC0
CONST uint8 bcServiceServUUID[TI_UUID_SIZE] =
{
  TI_UUID(BCSERVICE_SERV_UUID),
};

// Characteristic 1 UUID: 0xBCC1
CONST uint8 bcServicechar1UUID[TI_UUID_SIZE] =
{
  TI_UUID(BCSERVICE_B_UUID_UUID),
};

// Characteristic 2 UUID: 0xBCC2
CONST uint8 bcServicechar2UUID[TI_UUID_SIZE] =
{
  TI_UUID(BCSERVICE_B_MAJOR_UUID),
};

// Characteristic 3 UUID: 0xBCC3
CONST uint8 bcServicechar3UUID[TI_UUID_SIZE] =
{
  TI_UUID(BCSERVICE_B_MINOR_UUID),
};

// Characteristic 4 UUID: 0xBCC4
CONST uint8 bcServicechar4UUID[TI_UUID_SIZE] =
{
  TI_UUID(BCSERVICE_B_POWER_UUID),
};

// Characteristic 5 UUID: 0xBCC5
CONST uint8 bcServicechar5UUID[TI_UUID_SIZE] =
{
  TI_UUID(BCSERVICE_B_INTERVAL_UUID),
};

// Characteristic 6 UUID: 0xBCC6
CONST uint8 bcServicechar6UUID[TI_UUID_SIZE] =
{
  TI_UUID(BCSERVICE_B_PIN_UUID),
};

/*********************************************************************
* EXTERNAL VARIABLES
*/

/*********************************************************************
* EXTERNAL FUNCTIONS
*/

/*********************************************************************
* LOCAL VARIABLES
*/

static bcCBs_t *bcService_AppCBs = NULL;

/*********************************************************************
* Profile Attributes - variables
*/

// Beacon Config Service attribute
static CONST gattAttrType_t bcServiceService = { TI_UUID_SIZE, bcServiceServUUID };

// Beacon Config Service Characteristic 1 Properties
static uint8 bcServiceChar1Props = GATT_PROP_WRITE | GATT_PROP_READ;

// Beacon Config Service Characteristic 1 Value
static uint8 bcServiceChar1[BCSERVICE_B_UUID_LEN] = {0xFA,0x5F,0x55,0xD9,0xBC,
                                                    0x63,0x40,0x2E,0xA2,0x54,
                                                    0x09,0x1B,0x8F,0xE8,0xC9,
                                                    0x91};
#if defined USE_BEACON_DESCRIPTORS
// Beacon Config Service Characteristic 1 User Description
static uint8 bcServiceChar1UserDesp[] = "Beacon UUID";
#endif

// Beacon Config Service Characteristic 2 Properties
static uint8 bcServiceChar2Props = GATT_PROP_WRITE | GATT_PROP_READ;

// Beacon Config Service Characteristic 2 Value
static uint8 bcServiceChar2[BCSERVICE_B_MAJOR_LEN] = {0x00,0x01};

// Beacon Config Service Characteristic 2 User Description
#if defined USE_BEACON_DESCRIPTORS
static uint8 bcServiceChar2UserDesp[] = "Beacon Major";
#endif

// Beacon Config Service Characteristic 3 Properties
static uint8 bcServiceChar3Props = GATT_PROP_WRITE | GATT_PROP_READ;

// Beacon Config Service Characteristic 3 Value
static uint8 bcServiceChar3[BCSERVICE_B_MINOR_LEN] = {0x00,0x01};

// Beacon Config Service Characteristic 3 User Description
#if defined USE_BEACON_DESCRIPTORS
static uint8 bcServiceChar3UserDesp[] = "Beacon Minor";
#endif

// Beacon Config Service Characteristic 4 Properties
static uint8 bcServiceChar4Props = GATT_PROP_WRITE | GATT_PROP_READ;

// Beacon Config Service Characteristic 4 Value
static uint8 bcServiceChar4 = 0xC5;

// Beacon Config Service Characteristic 4 User Description
#if defined USE_BEACON_DESCRIPTORS
static uint8 bcServiceChar4UserDesp[] = "Beacon Power";
#endif

// Beacon Config Service Characteristic 5 Properties
static uint8 bcServiceChar5Props = GATT_PROP_WRITE | GATT_PROP_READ;

// Beacon Config Service Characteristic 5 Value
static uint8 bcServiceChar5[BCSERVICE_B_INTERVAL_LEN] = {0,0};

// Beacon Config Service Characteristic 5 User Description
#if defined USE_BEACON_DESCRIPTORS
static uint8 bcServiceChar5UserDesp[] = "Beacon Interval";
#endif

// Beacon Config Service Characteristic 6 Properties
static uint8 bcServiceChar6Props = GATT_PROP_WRITE | GATT_PROP_READ;

// Beacon Config Service Characteristic 6 Value
static uint8 bcServiceChar6[BCSERVICE_B_PIN_LEN] = {0,0};

// Beacon Config Service Characteristic 6 User Description
#if defined USE_BEACON_DESCRIPTORS
static uint8 bcServiceChar6UserDesp[] = "Beacon PIN";
#endif

/*********************************************************************
* Profile Attributes - Table
*/

static gattAttribute_t bcServiceAttrTbl[] =
{
  // Barometer Profile Service
  {
    { ATT_BT_UUID_SIZE, primaryServiceUUID }, /* type */
    GATT_PERMIT_READ,                         /* permissions */
    0,                                        /* handle */
    (uint8 *)&bcServiceService                /* pValue */
  },
  
  // Characteristic 1 Declaration
  {
    { ATT_BT_UUID_SIZE, characterUUID },
    GATT_PERMIT_READ,
    0,
    &bcServiceChar1Props
  },
  
  // Characteristic Value 1
  {
    { TI_UUID_SIZE, bcServicechar1UUID },
    GATT_PERMIT_READ | GATT_PERMIT_WRITE,
    0,
    bcServiceChar1
  },
#if defined USE_BEACON_DESCRIPTORS  
  // Characteristic 1 User Description
  {
    { ATT_BT_UUID_SIZE, charUserDescUUID },
    GATT_PERMIT_READ,
    0,
    bcServiceChar1UserDesp
  },
  
#endif
  // Characteristic 2 Declaration
  {
    { ATT_BT_UUID_SIZE, characterUUID },
    GATT_PERMIT_READ,
    0,
    &bcServiceChar2Props
  },
  
  // Characteristic Value 2
  {
    { TI_UUID_SIZE, bcServicechar2UUID },
    GATT_PERMIT_READ | GATT_PERMIT_WRITE,
    0,
    bcServiceChar2
  },
#if defined USE_BEACON_DESCRIPTORS
  // Characteristic 2 User Description
  {
    { ATT_BT_UUID_SIZE, charUserDescUUID },
    GATT_PERMIT_READ,
    0,
    bcServiceChar2UserDesp
  },
#endif
  // Characteristic 3 Declaration
  {
    { ATT_BT_UUID_SIZE, characterUUID },
    GATT_PERMIT_READ,
    0,
    &bcServiceChar3Props
  },
  
  // Characteristic Value 3
  {
    { TI_UUID_SIZE, bcServicechar3UUID },
    GATT_PERMIT_READ | GATT_PERMIT_WRITE,
    0,
    bcServiceChar3
  },
#if defined USE_BEACON_DESCRIPTORS 
  // Characteristic 3 User Description
  {
    { ATT_BT_UUID_SIZE, charUserDescUUID },
    GATT_PERMIT_READ,
    0,
    bcServiceChar3UserDesp
  },
  
#endif
  // Characteristic 4 Declaration
  {
    { ATT_BT_UUID_SIZE, characterUUID },
    GATT_PERMIT_READ,
    0,
    &bcServiceChar4Props
  },
  
  // Characteristic Value 4
  {
    { TI_UUID_SIZE, bcServicechar4UUID },
    GATT_PERMIT_READ | GATT_PERMIT_WRITE,
    0,
    &bcServiceChar4
  },
#if defined USE_BEACON_DESCRIPTORS  
  // Characteristic 4 User Description
  {
    { ATT_BT_UUID_SIZE, charUserDescUUID },
    GATT_PERMIT_READ,
    0,
    bcServiceChar4UserDesp
  },
#endif  
    // Characteristic 5 Declaration
  {
    { ATT_BT_UUID_SIZE, characterUUID },
    GATT_PERMIT_READ,
    0,
    &bcServiceChar5Props
  },
  
  // Characteristic Value 5
  {
    { TI_UUID_SIZE, bcServicechar5UUID },
    GATT_PERMIT_READ | GATT_PERMIT_WRITE,
    0,
    bcServiceChar5
  },
 #if defined USE_BEACON_DESCRIPTORS 
  // Characteristic 5 User Description
  {
    { ATT_BT_UUID_SIZE, charUserDescUUID },
    GATT_PERMIT_READ,
    0,
    bcServiceChar5UserDesp
  },
#endif
    // Characteristic 6 Declaration
  {
    { ATT_BT_UUID_SIZE, characterUUID },
    GATT_PERMIT_READ,
    0,
    &bcServiceChar6Props
  },
  
  // Characteristic Value 6
  {
    { TI_UUID_SIZE, bcServicechar6UUID },
    GATT_PERMIT_READ | GATT_PERMIT_WRITE,
    0,
    bcServiceChar6
  },
#if defined USE_BEACON_DESCRIPTORS 
  // Characteristic 6 User Description
  {
    { ATT_BT_UUID_SIZE, charUserDescUUID },
    GATT_PERMIT_READ,
    0,
    bcServiceChar6UserDesp
  },
#endif
};


/*********************************************************************
* LOCAL FUNCTIONS
*/
static uint8 bcService_ReadAttrCB( uint16 connHandle, gattAttribute_t *pAttr,
                                  uint8 *pValue, uint8 *pLen, uint16 offset, uint8 maxLen );
static bStatus_t bcService_WriteAttrCB( uint16 connHandle, gattAttribute_t *pAttr,
                                       uint8 *pValue, uint8 len, uint16 offset );

static void bcService_HandleConnStatusCB( uint16 connHandle, uint8 changeType );


/*********************************************************************
* PROFILE CALLBACKS
*/
// Service Callbacks
CONST gattServiceCBs_t bcServiceCBs =
{
  bcService_ReadAttrCB,  // Read callback function pointer
  bcService_WriteAttrCB, // Write callback function pointer
  NULL                       // Authorization callback function pointer
};

/*********************************************************************
* PUBLIC FUNCTIONS
*/

/*********************************************************************
* @fn      BcService_AddService
*
* @brief   Initializes the service by registering
*          GATT attributes with the GATT server.
*
* @param   services - services to add. This is a bit map and can
*                     contain more than one service.
*
* @return  Success or Failure
*/
bStatus_t BcService_AddService( uint32 services )
{
  uint8 status = SUCCESS;
  
  // // Initialize Client Characteristic Configuration attributes
  // GATTServApp_InitCharCfg( INVALID_CONNHANDLE, ccDataConfig );
  
  // Register with Link DB to receive link status change callback
  VOID linkDB_Register( bcService_HandleConnStatusCB );
  
  if ( services & BCSERVICE )
  {
    // Register GATT attribute list and CBs with GATT Server App
    status = GATTServApp_RegisterService( bcServiceAttrTbl,
                                         GATT_NUM_ATTRS( bcServiceAttrTbl ),
                                         &bcServiceCBs );
  }
  
  return ( status );
}


/*********************************************************************
* @fn      BcService_RegisterAppCBs
*
* @brief   Registers the application callback function. Only call
*          this function once.
*
* @param   callbacks - pointer to application callbacks.
*
* @return  SUCCESS or bleAlreadyInRequestedMode
*/
bStatus_t BcService_RegisterAppCBs( bcCBs_t *appCallbacks )
{
  if ( appCallbacks )
  {
    bcService_AppCBs = appCallbacks;
    
    return ( SUCCESS );
  }
  else
  {
    return ( bleAlreadyInRequestedMode );
  }
}


/*********************************************************************
* @fn      BcService_SetParameter
*
* @brief   Set a Simple Profile parameter.
*
* @param   param - Profile parameter ID
* @param   len - length of data to right
* @param   value - pointer to data to write.  This is dependent on
*          the parameter ID and WILL be cast to the appropriate
*          data type (example: data type of uint16 will be cast to
*          uint16 pointer).
*
* @return  bStatus_t
*/
bStatus_t BcService_SetParameter( uint8 param, uint8 len, void *value )
{
  bStatus_t ret = SUCCESS;
  
  switch ( param )
  {
  case BCSERVICE_B_UUID:
    if ( len == BCSERVICE_B_UUID_LEN )
    {
      VOID osal_memcpy(bcServiceChar1, value, BCSERVICE_B_UUID_LEN );
    }
    else
    {
      ret = bleInvalidRange;
    }
    break;
    
  case BCSERVICE_B_MAJOR:
    if ( len == BCSERVICE_B_MAJOR_LEN )
    {
      VOID osal_memcpy( bcServiceChar2, value, BCSERVICE_B_MAJOR_LEN );
    }
    else
    {
      ret = bleInvalidRange;
    }
    break;
    
  case BCSERVICE_B_MINOR:
    if ( len == BCSERVICE_B_MINOR_LEN )
    {
      VOID osal_memcpy( bcServiceChar3, value, BCSERVICE_B_MINOR_LEN );
    }
    else
    {
      ret = bleInvalidRange;
    }
    break;
    
  case BCSERVICE_B_POWER:
    if ( len == sizeof ( uint8 ) )
    {
      bcServiceChar4 = *((uint8*)value);
    }
    else
    {
      ret = bleInvalidRange;
    }
    break;
    
  case BCSERVICE_B_INTERVAL:
    if ( len == BCSERVICE_B_INTERVAL_LEN )
    {          
      VOID osal_memcpy( bcServiceChar5, value, BCSERVICE_B_INTERVAL_LEN );
    }
    else
    {
      ret = bleInvalidRange;
    }
    break;
    
  case BCSERVICE_B_PIN:
    if ( len == BCSERVICE_B_PIN_LEN )
    {
      VOID osal_memcpy( bcServiceChar6, value, BCSERVICE_B_PIN_LEN );
    }
    else
    {
      ret = bleInvalidRange;
    }
    break;
    
  default:
    ret = INVALIDPARAMETER;
    break;
   
  }
  
  return ( ret );
}

/*********************************************************************
* @fn      BcService_GetParameter
*
* @brief   Get a CC Profile parameter.
*
* @param   param - Profile parameter ID
* @param   value - pointer to data to put.  This is dependent on
*          the parameter ID and WILL be cast to the appropriate
*          data type (example: data type of uint16 will be cast to
*          uint16 pointer).
*
* @return  bStatus_t
*/
bStatus_t BcService_GetParameter( uint8 param, void *value )
{
  bStatus_t ret = SUCCESS;
  switch ( param )
  {
  case BCSERVICE_B_UUID:
    VOID osal_memcpy( value, bcServiceChar1, BCSERVICE_B_UUID_LEN );
    break;
    
  case BCSERVICE_B_MAJOR:
    VOID osal_memcpy( value, bcServiceChar2, BCSERVICE_B_MAJOR_LEN );
    break;
    
  case BCSERVICE_B_MINOR:
    VOID osal_memcpy( value, bcServiceChar3, BCSERVICE_B_MINOR_LEN );
    break;
    
  case BCSERVICE_B_POWER:
    *((uint8*)value) = bcServiceChar4;
    break;
      
  case BCSERVICE_B_INTERVAL:
    VOID osal_memcpy( value, bcServiceChar5, BCSERVICE_B_INTERVAL_LEN );
    break;
    
  case BCSERVICE_B_PIN:
    VOID osal_memcpy( value, bcServiceChar6, BCSERVICE_B_PIN_LEN );
    break;
    
  default:
    ret = INVALIDPARAMETER;
    break;
  }
  
  return ( ret );
}

/*********************************************************************
* @fn          bcService_ReadAttrCB
*
* @brief       Read an attribute.
*
* @param       connHandle - connection message was received on
* @param       pAttr - pointer to attribute
* @param       pValue - pointer to data to be read
* @param       pLen - length of data to be read
* @param       offset - offset of the first octet to be read
* @param       maxLen - maximum length of data to be read
*
* @return      Success or Failure
*/
static uint8 bcService_ReadAttrCB( uint16 connHandle, gattAttribute_t *pAttr,
                                  uint8 *pValue, uint8 *pLen, uint16 offset, uint8 maxLen )
{
  bStatus_t status = SUCCESS;
  uint16 uuid;
  
  // If attribute permissions require authorization to read, return error
  if ( gattPermitAuthorRead( pAttr->permissions ) )
  {
    // Insufficient authorization
    return ( ATT_ERR_INSUFFICIENT_AUTHOR );
  }
  
  // Make sure it's not a blob operation (no attributes in the profile are long)
  if ( offset > 0 )
  {
    return ( ATT_ERR_ATTR_NOT_LONG );
  }
  
  if (utilExtractUuid16(pAttr,&uuid) == FAILURE)
  {
    // Invalid handle
    *pLen = 0;
    return ATT_ERR_INVALID_HANDLE;
  }
  
  switch ( uuid )
  {
  case BCSERVICE_B_UUID_UUID:
    *pLen = BCSERVICE_B_UUID_LEN;
    VOID osal_memcpy( pValue, pAttr->pValue, BCSERVICE_B_UUID_LEN );
    break;
    
  case BCSERVICE_B_MAJOR_UUID:
    *pLen = BCSERVICE_B_MAJOR_LEN;
    VOID osal_memcpy( pValue, pAttr->pValue, BCSERVICE_B_MAJOR_LEN );
    break;
    
  case BCSERVICE_B_MINOR_UUID:
    *pLen = BCSERVICE_B_MINOR_LEN;
    VOID osal_memcpy( pValue, pAttr->pValue, BCSERVICE_B_MINOR_LEN );
    break;
    
  case BCSERVICE_B_POWER_UUID:
    *pLen = BCSERVICE_B_POWER_LEN;
    pValue[0] = *pAttr->pValue;
    break;
    
  case BCSERVICE_B_INTERVAL_UUID:
    *pLen = BCSERVICE_B_INTERVAL_LEN;
    VOID osal_memcpy( pValue, pAttr->pValue, BCSERVICE_B_INTERVAL_LEN );
    break;
    
  case BCSERVICE_B_PIN_UUID:
    *pLen = BCSERVICE_B_PIN_LEN;
    VOID osal_memcpy( pValue, pAttr->pValue, BCSERVICE_B_PIN_LEN );
    break;
    
  default:
    *pLen = 0;
    status = ATT_ERR_ATTR_NOT_FOUND;
    break;
  }
  
  return ( status );
}

/*********************************************************************
* @fn      bcService_WriteAttrCB
*
* @brief   Validate attribute data prior to a write operation
*
* @param   connHandle - connection message was received on
* @param   pAttr - pointer to attribute
* @param   pValue - pointer to data to be written
* @param   len - length of data
* @param   offset - offset of the first octet to be written
*
* @return  Success or Failure
*/
static bStatus_t bcService_WriteAttrCB( uint16 connHandle, gattAttribute_t *pAttr,
                                       uint8 *pValue, uint8 len, uint16 offset )
{
  uint16 uuid;
  bStatus_t status = SUCCESS;
  uint8 notifyApp = 0xFF;
  
  // If attribute permissions require authorization to write, return error
  if ( gattPermitAuthorWrite( pAttr->permissions ) )
  {
    // Insufficient authorization
    return ( ATT_ERR_INSUFFICIENT_AUTHOR );
  }
  
  if (utilExtractUuid16(pAttr,&uuid) == FAILURE)
  {
    // Invalid handle
    return ATT_ERR_INVALID_HANDLE;
  }
  
  switch ( uuid )
  {
  case BCSERVICE_B_UUID_UUID:
    
    // Validate the value
    // Make sure it's not a blob oper
    if ( offset == 0 )
    {
      if ( len != BCSERVICE_B_UUID_LEN )
      {
        status = ATT_ERR_INVALID_VALUE_SIZE;
      }
    }
    else
    {
      status = ATT_ERR_ATTR_NOT_LONG;
    }
    
    // Write the value
    if ( status == SUCCESS )
    {
      VOID osal_memcpy( bcServiceChar1, pValue, BCSERVICE_B_UUID_LEN );
      notifyApp = BCSERVICE_B_UUID;
    }
    
    break;
    
  case BCSERVICE_B_MAJOR_UUID:
    
    // Validate the value
    // Make sure it's not a blob oper
    if ( offset == 0 )
    {
      if ( len != BCSERVICE_B_MAJOR_LEN )
      {
        status = ATT_ERR_INVALID_VALUE_SIZE;
      }
    }
    else
    {
      status = ATT_ERR_ATTR_NOT_LONG;
    }
    
    // Write the value
    if ( status == SUCCESS )
    {
      VOID osal_memcpy( bcServiceChar2, pValue, BCSERVICE_B_MAJOR_LEN );
      notifyApp = BCSERVICE_B_MAJOR;
    }
    
    break;
    
  case BCSERVICE_B_MINOR_UUID:
    
    // Validate the value
    // Make sure it's not a blob oper
    if ( offset == 0 )
    {
      if ( len != BCSERVICE_B_MINOR_LEN )
      {
        status = ATT_ERR_INVALID_VALUE_SIZE;
      }
    }
    else
    {
      status = ATT_ERR_ATTR_NOT_LONG;
    }
    
    // Write the value
    if ( status == SUCCESS )
    {
      VOID osal_memcpy( bcServiceChar3, pValue, BCSERVICE_B_MINOR_LEN );
      notifyApp = BCSERVICE_B_MINOR;
    }
    
    break;
    
  case BCSERVICE_B_POWER_UUID:
    
    if ( offset == 0 )
    {
      if ( len != BCSERVICE_B_POWER_LEN )
      {
        status = ATT_ERR_INVALID_VALUE_SIZE;
      }
    }
    else
    {
      status = ATT_ERR_ATTR_NOT_LONG;
    }
    
    // Write the value
    if ( status == SUCCESS )
    {
      uint8 *pCurValue = (uint8 *)pAttr->pValue;
      *pCurValue = pValue[0];
      notifyApp = BCSERVICE_B_POWER;
    }
    
    break;
    
  case BCSERVICE_B_INTERVAL_UUID:

    // Validate the value
    // Make sure it's not a blob oper
    if ( offset == 0 )
    {
      if ( len != BCSERVICE_B_INTERVAL_LEN )
      {
        status = ATT_ERR_INVALID_VALUE_SIZE;
      }
    }
    else
    {
      status = ATT_ERR_ATTR_NOT_LONG;
    }
    
    // Write the value
    if ( status == SUCCESS )
    {
      uint16 tmp = BUILD_UINT16(pValue[0],pValue[1]);   
      if((tmp >= 160) && (tmp <= 16000)) //Min 160=100ms and max 16000= 10s 
      {
        VOID osal_memcpy( bcServiceChar5, pValue, BCSERVICE_B_INTERVAL_LEN );
        notifyApp = BCSERVICE_B_INTERVAL;
      }  
      else
      {
        status = ATT_ERR_INVALID_VALUE;
      }
    }
    
    break;
    
  case BCSERVICE_B_PIN_UUID:

    // Validate the value
    // Make sure it's not a blob oper
    if ( offset == 0 )
    {
      if ( len != BCSERVICE_B_PIN_LEN )
      {
        status = ATT_ERR_INVALID_VALUE_SIZE;
      }
    }
    else
    {
      status = ATT_ERR_ATTR_NOT_LONG;
    }
    
    // Write the value
    if ( status == SUCCESS )
    {
      VOID osal_memcpy( bcServiceChar6, pValue, BCSERVICE_B_PIN_LEN );
      notifyApp = BCSERVICE_B_PIN;
    }
    
    break;
    
  case GATT_CLIENT_CHAR_CFG_UUID:
    // status = GATTServApp_ProcessCCCWriteReq( connHandle, pAttr, pValue, len,
    // offset, GATT_CLIENT_CFG_NOTIFY );
    break;
    
  default:
    status = ATT_ERR_ATTR_NOT_FOUND;
    break;
  }
  
  
  // If a charactersitic value changed then callback function to notify application of change
  if ( (notifyApp != 0xFF ) && bcService_AppCBs && bcService_AppCBs->pfnBcChange )
  {
    bcService_AppCBs->pfnBcChange( notifyApp );
  }
  
  return ( status );
}

/*********************************************************************
* @fn          bcService_HandleConnStatusCB
*
* @brief       Simple Profile link status change handler function.
*
* @param       connHandle - connection handle
* @param       changeType - type of change
*
* @return      none
*/
static void bcService_HandleConnStatusCB( uint16 connHandle, uint8 changeType )
{
  // Make sure this is not loopback connection
  if ( connHandle != LOOPBACK_CONNHANDLE )
  {
    // Reset "TBD" if connection has dropped
    if ( ( changeType == LINKDB_STATUS_UPDATE_REMOVED )      ||
        ( ( changeType == LINKDB_STATUS_UPDATE_STATEFLAGS ) &&
         ( !linkDB_Up( connHandle ) ) ) )
    {
      // TODO - Reset all Characteristics
    }
  }
}


/*********************************************************************
*********************************************************************/
