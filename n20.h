/*
 * n20.h
 *
 * A barebones library to use n20 motors with encoders as stepper motors. 
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

#ifndef N20_H
#define N20_H

struct n20{
	uint8_t m1; 			//Motor driver pin M1 (pwm)
	uint8_t m2; 			//Motor driver pin M2 (pwm)
	uint8_t e1; 			//Encoder pin C1
	uint8_t e2; 			//Encoder pin C2
	uint8_t speed;			//Motor driver speed (pwm)
	void (*tfunc)(void *args); 	//Stop function
	void *args; 			//Stop function parameters
	int16_t val; 			//Current encoder value
	int16_t num; 			//Target encoder value
	bool e1prev;
	volatile bool busy; 		//Motor status
};

#ifndef N20_TURN_DENOMINATOR
//Defines one complete rotation for turn functions
// yourunits*encoderticks (for complete rotation)
#define N20_TURN_DENOMINATOR 360*2018
#endif

//Turn x units relative to current position
#define n20_turnrel(MOTOR, ANGLE)\
	n20_moverel(MOTOR, (float)ANGLE/N20_TURN_DENOMINATOR);

//Turn x units relative to zero
#define n20_turnabs(MOTOR, ANGLE)\
	n20_moveabs(MOTOR, (float)ANGLE/N20_TURN_DENOMINATOR);

//Updates encoder value
void n20_encoder_isr(struct n20 *a);

//Turn x steps relative to current position
void n20_moverel(struct n20 *motor, int16_t step);

//Turn x steps relative to zero
void n20_moveabs(struct n20 *motor, int16_t step);

//Stop motor
void n20_stop(struct n20 *motor);

//Set speed 0-255 (pwm)
void n20_setspeed(struct n20 *motor, uint8_t speed);

//Initialize motor
void n20_init(struct n20 *motor);

#endif
