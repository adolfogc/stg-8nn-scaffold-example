/*
Copyright (C) 2019 Adolfo E. García

This file is part of STG-8nn-Scaffold.

STG-8nn-Scaffold is free software: you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

STG-8nn-Scaffold is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with STG-8nn-Scaffold.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <string.h>
#include "app.h"
#include "led.h"
#include "uavcan_node.h"
#include "blinky/LedControl.h"

#include <stdio.h>

/* Prototypes */
static void blinky_LedControlHandle(CanardRxTransfer* transfer);

/* Overrides' prototypes */
bool shouldAcceptTransferExtend(const CanardInstance* instance, uint64_t* outDataTypeSignature, uint16_t dataTypeId, CanardTransferType transferType, uint8_t sourceNodeId);
void onTransferReceivedExtend(CanardInstance* instance, CanardRxTransfer* transfer);

/* Overrides shouldAcceptTransferExtend/5 */
bool shouldAcceptTransferExtend(const CanardInstance* instance, uint64_t* outDataTypeSignature, uint16_t dataTypeId, CanardTransferType transferType, uint8_t sourceNodeId)
{
      switch(dataTypeId) {
        case BLINKY_LEDCONTROL_ID:
          *outDataTypeSignature = BLINKY_LEDCONTROL_SIGNATURE;
          return true;
        default:
          return false;
      }
}

/* Overrides onTransferReceivedExtend/2 */
void onTransferReceivedExtend(CanardInstance* instance, CanardRxTransfer* transfer)
{
    switch(transfer->data_type_id) {
      case BLINKY_LEDCONTROL_ID:
        blinky_LedControlHandle(transfer);
        return;
      default:
        return;
    }
}

/* Local implementations */
static void blinky_LedControlHandle(CanardRxTransfer* transfer)
{
    uint8_t buffer[BLINKY_LEDCONTROL_REQUEST_MAX_SIZE];
    memset(buffer, 0, BLINKY_LEDCONTROL_REQUEST_MAX_SIZE);

    blinky_LedControlRequest ledControlRequest;
    blinky_LedControlRequest_decode(transfer, transfer->payload_len, &ledControlRequest, (uint8_t**)0);
    const uint8_t action = ledControlRequest.action;

    //_Static_assert(BLINKY_LEDCONTROL_RESPONSE_MAX_SIZE <= BLINKY_LEDCONTROL_REQUEST_MAX_SIZE, "response size is greater than request size");
    blinky_LedControlResponse ledControlResponse;
    ledControlResponse.ok = true;
    const uint32_t len = blinky_LedControlResponse_encode(&ledControlResponse, buffer);

    int result = canardRequestOrRespond(getCanardInstance(),
                                        transfer->source_node_id,
                                        BLINKY_LEDCONTROL_SIGNATURE,
                                        BLINKY_LEDCONTROL_ID,
                                        &transfer->transfer_id,
                                        transfer->priority,
                                        CanardResponse,
                                        buffer,
                                        (uint16_t)len);

    if(result >= 0) {
        /* Do action if we were able to confirm. */
        switch(action) {
            case BLINKY_LEDCONTROL_REQUEST_ACTION_ON:
                Led_postOn();
                break;
            case BLINKY_LEDCONTROL_REQUEST_ACTION_OFF:
                Led_postOff();
                break;
            case BLINKY_LEDCONTROL_REQUEST_ACTION_BLINK:
                Led_postBlink();
                break;
            default:
                break;
        }
    }
}
