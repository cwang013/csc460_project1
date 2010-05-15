/*
 * sonar.h
 *
 *  Created on: 2010-05-12
 *      Author: lienh
 */

#ifndef SONAR_H_
#define SONAR_H_

void sonar_init();
void sonar_trigger();
uint8_t sonar_echo_received();
uint16_t sonar_get_distance();

#endif /* SONAR_H_ */
