/*
 * event.h
 *
 * Created: 26.04.2016 20:57:26
 *  Author: Win7
 */ 
#ifndef EVENT_H

#define EVENT_H

#include "stdio.h"
#define READ_COMPASS_BASE 0x40
#define READ_SIN_COMPASS		(READ_COMPASS_BASE + 0)
#define READ_COS_COMPASS		(READ_COMPASS_BASE + 1)
#define READ_SIN_COS_COMPASS	(READ_COMPASS_BASE + 2)

void event_handler_rcv(int8_t evt);

#endif