/*
 * n20.cpp
 *
 * Copyright (C) 2025 Vivek Goutam
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, see
 * <https://www.gnu.org/licenses/>.*
 */

#include <Arduino.h>
#include <YetAnotherPcInt.h>
extern "C"{
#include "n20.h"
}

void n20_stop(struct n20 *motor){
	digitalWrite(motor->m1, HIGH);
	digitalWrite(motor->m2, HIGH);
	motor->busy = false;
}

//Credits: https://curiousscientist.tech/blog/positioning-with-the-n20-miniature-geared-dc-motors
void n20_encoder_isr(struct n20 *a, bool state){
	int e1curr = state;
	if(e1curr != a->e1prev && e1curr == 1){
		if(digitalRead(a->e2) != e1curr)
			a->val--;
		else
			a->val++;
		if(a->val == a->num)
			a->tfunc(a->args);
	}
	a->e1prev = e1curr;
}

void n20_init(struct n20 *motor){
	pinMode(motor->m1, OUTPUT);
	pinMode(motor->m2, OUTPUT);
	pinMode(motor->e1, INPUT_PULLUP);
	pinMode(motor->e2, INPUT_PULLUP);
	PcInt::attachInterrupt(motor->e1, n20_encoder_isr, motor , CHANGE);
	if(!motor->speed) motor->speed = 255;
	if(!motor->tfunc){
		motor->tfunc = n20_stop;
		motor->args = motor;
	}
	n20_stop(motor);
}

void n20_moverel(struct n20 *motor, int16_t step){
	n20_stop(motor);
	motor->num = motor->val + step;
	if(step > 0){
		analogWrite(motor->m1, 0);
		analogWrite(motor->m2, motor->speed);
		motor->busy = true;
	}
	else if(step < 0){
		analogWrite(motor->m2, 0);
		analogWrite(motor->m1, motor->speed);
		motor->busy = true;
	}
}

void n20_moveabs(struct n20 *motor, int16_t step){
	n20_stop(motor);
	motor->num = step;
	if(step > motor->val){
		analogWrite(motor->m1, 0);
		analogWrite(motor->m2, motor->speed);
		motor->busy = true;
	}
	else if(step < motor->val){
		analogWrite(motor->m2, 0);
		analogWrite(motor->m1, motor->speed);
		motor->busy = true;
	}
}

void n20_setspeed(struct n20 *motor, uint8_t speed){
	motor->speed = speed;
	if(digitalRead(motor->m2) == LOW)
		analogWrite(motor->m1, speed);
	else if(digitalRead(motor->m1) == LOW)
		analogWrite(motor->m2, speed);
}	
