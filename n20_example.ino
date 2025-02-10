#include "n20.h"

struct n20 a = {10, 11, 4, 5};
struct n20 b = {6, 9, 7, 8};

void setup(){
	Serial.begin(9600);
	n20_init(&a);
	n20_init(&b);
}

void loop(){
	if(Serial.available()){
		String s = Serial.readString();
		switch(s.charAt(0)){
			case 'a':
				n20_turnabs(&a, s.substring(1).toInt());
				break;
			case 'b':
				n20_turnabs(&b, s.substring(1).toInt());
				break;
			default:
				n20_turnabs(&a, s.toInt());
				n20_turnabs(&b, s.toInt());
				break;
		}
	}
}
