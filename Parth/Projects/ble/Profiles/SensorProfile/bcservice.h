/**************************************************************************************************
  Filename:       bcservice.h
  Revised:        $Date: 2010-08-06 08:56:11 -0700 (Fri, 06 Aug 2010) $
  Revision:       $Revision: 23333 $

  Description:    This file contains the proprietary Beacon Config Service
				          Prototypes.

  Copyright 2012-2013 Texas Instruments Incorporated. All rights reserved.

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

#ifndef BCSERVICE_H
#define BCSERVICE_H

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * INCLUDES
 */

/*********************************************************************
 * CONSTANTS
 */

// Service Parameters
#define BCSERVICE_B_UUID                 	0  // RW   - Beacon UUID
#define BCSERVICE_B_MAJOR                 	1  // RW   - Beacon Major
#define BCSERVICE_B_MINOR                 	2  // RW   - Beacon Minor
#define BCSERVICE_B_POWER                 	3  // RW   - Beacon Power
#define BCSERVICE_B_INTERVAL                    4  // RW   - Beacon Interval
#define BCSERVICE_B_PIN                         5  // RW   - Beacon Interval
  
// Service UUID
#define BCSERVICE_SERV_UUID             	0xBCC0
#define BCSERVICE_B_UUID_UUID            	0xBCC1 // F000BCC1-0451-4000-B000-00000000-0000
#define BCSERVICE_B_MAJOR_UUID            	0xBCC2
#define BCSERVICE_B_MINOR_UUID            	0xBCC3
#define BCSERVICE_B_POWER_UUID            	0xBCC4
#define BCSERVICE_B_INTERVAL_UUID               0xBCC5
#define BCSERVICE_B_PIN_UUID                    0xBCC6

// Services bit fields
#define BCSERVICE                       0x00000001

// Length of Characteristics in bytes
#define BCSERVICE_B_UUID_LEN             16
#define BCSERVICE_B_MAJOR_LEN             2
#define BCSERVICE_B_MINOR_LEN             2
#define BCSERVICE_B_POWER_LEN             1
#define BCSERVICE_B_INTERVAL_LEN          2
#define BCSERVICE_B_PIN_LEN               4
  
/*********************************************************************
 * TYPEDEFS
 */


/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * Profile Callbacks
 */

// Callback when a characteristic value has changed
typedef void (*bcChange_t)( uint8 paramID );

typedef struct
{
  bcChange_t        pfnBcChange;  // Called when characteristic value changes
} bcCBs_t;



/*********************************************************************
 * API FUNCTIONS
 */


/*
 * BcService_AddService- Initializes the Simple GATT Profile service by registering
 *          GATT attributes with the GATT server.
 *
 * @param   services - services to add. This is a bit map and can
 *                     contain more than one service.
 */

extern bStatus_t BcService_AddService( uint32 services );

/*
 * BcService_RegisterAppCBs - Registers the application callback function.
 *                    Only call this function once.
 *
 *    appCallbacks - pointer to application callbacks.
 */
extern bStatus_t BcService_RegisterAppCBs( bcCBs_t *appCallbacks );

/*
 * BcService_SetParameter - Set a Simple GATT Profile parameter.
 *
 *    param - Profile parameter ID
 *    len - length of data to right
 *    value - pointer to data to write.  This is dependent on
 *          the parameter ID and WILL be cast to the appropriate
 *          data type (example: data type of uint16 will be cast to
 *          uint16 pointer).
 */
extern bStatus_t BcService_SetParameter( uint8 param, uint8 len, void *value );

/*
 * BcService_GetParameter - Get a Simple GATT Profile parameter.
 *
 *    param - Profile parameter ID
 *    value - pointer to data to write.  This is dependent on
 *          the parameter ID and WILL be cast to the appropriate
 *          data type (example: data type of uint16 will be cast to
 *          uint16 pointer).
 */
extern bStatus_t BcService_GetParameter( uint8 param, void *value );


/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* BCSERVICE_H */
