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

#include "app.h"
#include "led.h"

/* Prototype for override function */
int App_main(void);

Q_DEFINE_THIS_FILE

/* Overrides App_main/0 */
int App_main(void)
{
    static QEvt const * uavcanNode_queueBuffer[20];
    static QEvt const * led_queueBuffer[10];
    static UavcanNode* uavcanNode = NULL;
    static Led* led = NULL;

    uavcanNode = UavcanNode_initAO();
    led = Led_initAO();

    BSP_init(); /* Initialize the hardware. */

    BSP_Led_on();
    BSP_CAN_init(); /* Inititalize the CAN hardware for use with Libcanard */
    BSP_Led_off();

    QACTIVE_START((QActive*)&uavcanNode->super,
      2U,
      uavcanNode_queueBuffer,
      Q_DIM(uavcanNode_queueBuffer),
      (void*)0, 0U,
      (QEvt*)0);

    QACTIVE_START((QActive*)&led->super,
      1U,
      led_queueBuffer,
      Q_DIM(led_queueBuffer),
      (void*)0, 0U,
      (QEvt*)0);

      return QF_run();
}
