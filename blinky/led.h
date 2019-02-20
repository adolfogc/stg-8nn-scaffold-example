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

#ifndef _LED_AO_H
#define _LED_AO_H

#include "qpc.h"

typedef struct LedTag {
    QActive super;

    QTimeEvt timeEvent;
} Led;

Led* initLed(void);

void Led_postOn(void);
void Led_postOff(void);
void Led_postBlink(uint16_t blink_duration, uint16_t blink_period, uint16_t blinking_time);

#endif /* _LED_AO_H */