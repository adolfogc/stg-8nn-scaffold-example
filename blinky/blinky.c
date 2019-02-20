#include "app.h"
#include "led.h"

/* Prototype for override function */
int App_main(void);

Q_DEFINE_THIS_FILE

/* App_main/0 override */
int App_main(void)
{
    static QEvt const * uavcanNode_queueBuffer[20];
    static QEvt const * led_queueBuffer[5];
    static UavcanNode* uavcanNode;
    static Led* led;

    uavcanNode = initUavcanNode();
    led = initLed();

    App_init();

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
