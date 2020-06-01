/*
 * dbg.h
 *
 *  Created on: May 3, 2020
 *      Author: belkacem
 */

#ifndef INC_DBG_H_
#define INC_DBG_H_


/*******DEFINES**********/
#define TIME_OUT 1000 /*1000 ms time out for text out put*/
#define TIME_IN 1000 /*1000 ms time in for char input*/
#define MSG_SIZE 255
#define TAG_SIZE 255

char dbg_tag[TAG_SIZE];
char dbg_msg[MSG_SIZE];

#ifndef DEBUG
#define DEBUG
#endif

#ifdef DEBUG /* DeBUGGING MODE */

#define TRACE true

/* macro to trace debug on serial monitor  with ticks */
#define dbg_printf(...) { \
if ( sprintf(dbg_tag, "<%lu>", HAL_GetTick()) &&  /* ticks tag */ \
	sprintf(dbg_msg, __VA_ARGS__) ) /* args */ \
	printf("\r\n%s\t%s", dbg_tag, dbg_msg); /* concat */ \
}; /* end debug macro */

/* macro to trace debug on serial monitor with file & line number */
#define dbg_log(...) { \
if ( sprintf(dbg_tag, "%s[%d]", __FILE__, __LINE__) &&  /* file line tag */ \
	sprintf(dbg_msg, __VA_ARGS__) ) /* args */ \
	printf("\r\n%s\t%s", dbg_tag, dbg_msg); /* concat */ \
}; /* end debug macro */

#else /* NO DEBUG */

#define TRACE false

/* macro are NOP */
#define dbg_printf(...) {}
#define dbg_log(...) {}

#endif /* DEBUG */



#endif /* INC_DBG_H_ */
