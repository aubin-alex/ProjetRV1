/*
 * ParameterStudy.cpp
 *
 *  Created on: Feb 23, 2014
 *      Author: fanzhang
 */

#include "ParameterStudy.h"

float DecreaseCurrentPara(float x, float Para) {
	Para = Para - x;
	return Para;
}

float IncreaseCurrentPara(float x, float Para) {
	Para = Para + x;
	return Para;
}
