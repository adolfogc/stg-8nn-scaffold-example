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

#include "bsp.h"
#include "led.h"

Led g_led;

enum LedSignals {
    LED_BLINK_SIG = Q_USER_SIG,
    LED_OFF_SIG,
    LED_ON_SIG,
    LED_TIMEOUT_SIG
};

/* Prototypes */
static void Led_ctor(Led* me);

static QState Led_init(Led * const me, QEvt const * const e);

static QState Led_off(Led * const me, QEvt const * const e);
static QState Led_on(Led * const me, QEvt const * const e);

static QState Led_blinking(Led * const me, QEvt const * const e);
static QState Led_blinking_on(Led * const me, QEvt const * const e);
static QState Led_blinking_off(Led * const me, QEvt const * const e);

/* Implementations */
Led* initLed(void)
{
    Led_ctor(&g_led);
    return &g_led;
}

void Led_postOn(void)
{
    static const QEvt evt = {LED_ON_SIG, 0U, 0U};
    QACTIVE_POST(&g_led.super, &evt, (void*)0);
}

void Led_postOff(void)
{
    static const QEvt evt = {LED_OFF_SIG, 0U, 0U};
    QACTIVE_POST(&g_led.super, &evt, (void*)0);
}

void Led_postBlink()
{
    static const QEvt evt = {LED_BLINK_SIG, 0U, 0U};
    QACTIVE_POST(&g_led.super, &evt, (void*)0);
}

static void Led_ctor(Led* me)
{
    QActive_ctor(&me->super, Q_STATE_CAST(&Led_init));
    QTimeEvt_ctorX(&(me->timeEvent), &(me->super), LED_TIMEOUT_SIG, 0U);
}

static QState Led_init(Led * const me, QEvt const * const e)
{
    (void) e; /* unused parameter */

    return Q_TRAN(&Led_off);
}

static QState Led_off(Led * const me, QEvt const * const e)
{
    QState status;

    switch (e->sig) {
        case Q_ENTRY_SIG:
            BSP_Led_off();
            status = Q_HANDLED();
            break;
        case Q_EXIT_SIG:
            status = Q_HANDLED();
            break;
        case LED_BLINK_SIG:
            status = Q_TRAN(&Led_blinking);
            break;
        case LED_ON_SIG:
            status = Q_TRAN(&Led_on);
            break;
        default:
            status = Q_SUPER(&QHsm_top);
            break;
    }
    return status;
}

static QState Led_on(Led * const me, QEvt const * const e)
{
    QState status;

    switch (e->sig) {
        case Q_ENTRY_SIG:
            BSP_Led_on();
            status = Q_HANDLED();
            break;
        case Q_EXIT_SIG:
            status = Q_HANDLED();
            break;
        case LED_BLINK_SIG:
            status = Q_TRAN(&Led_blinking);
            break;
        case LED_OFF_SIG:
            status = Q_TRAN(&Led_off);
            break;
        default:
            status = Q_SUPER(&QHsm_top);
            break;
    }

    return status;
}

static QState Led_blinking(Led * const me, QEvt const * const e)
{
    QState status;

    switch (e->sig) {
        case Q_INIT_SIG:
            QTimeEvt_armX(&me->timeEvent, BSP_TICKS_PER_SEC, BSP_TICKS_PER_SEC);
            status = Q_TRAN(&Led_blinking_on);
            break;
        case Q_ENTRY_SIG:
            status = Q_HANDLED();
            break;
        case Q_EXIT_SIG:
            QTimeEvt_disarm(&me->timeEvent);
            status = Q_HANDLED();
            break;
        case LED_ON_SIG:
            status = Q_TRAN(&Led_on);
            break;
        case LED_OFF_SIG:
            status = Q_TRAN(&Led_off);
            break;
        default:
            status = Q_SUPER(&QHsm_top);
            break;
    }

    return status;
}

static QState Led_blinking_on(Led * const me, QEvt const * const e)
{
    QState status;

    switch (e->sig) {
        case Q_ENTRY_SIG:
            BSP_Led_on();
            status = Q_HANDLED();
            break;
        case Q_EXIT_SIG:
            status = Q_HANDLED();
            break;
        case LED_TIMEOUT_SIG:
            status = Q_TRAN(&Led_blinking_off);
            break;
        default:
            status = Q_SUPER(&Led_blinking);
            break;
    }

    return status;
}

static QState Led_blinking_off(Led * const me, QEvt const * const e)
{
    QState status;

    switch (e->sig) {
        case Q_ENTRY_SIG:
            BSP_Led_off();
            status = Q_HANDLED();
            break;
        case Q_EXIT_SIG:
            status = Q_HANDLED();
            break;
        case LED_TIMEOUT_SIG:
            status = Q_TRAN(&Led_blinking_on);
            break;
        default:
            status = Q_SUPER(&Led_blinking);
            break;
    }

    return status;
}
