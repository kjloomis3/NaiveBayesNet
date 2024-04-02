/*
 * datastream.h
 *
 *  Created for cs546: Programming Languages
 *  Author: Kenneth J. Loomis
 */

#ifndef DATASTREAM_H_
#define DATASTREAM_H_

#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include "timeval.h"

typedef struct timeval tv;

typedef struct
{
	int num;
	tv* ts; // timestamp
} tuple;

typedef struct stream_node
{
	struct stream_node* next;
	struct stream_node* prev;
	tv* ts; // timestamp
	tuple* info;
} stream_node;

typedef struct
{
	pthread_mutex_t protect_ds;
	stream_node* head;
	stream_node* tail;
	int count;
} datastream;

void create_dstream ( datastream* ds );
//void destroy_dstream ( datastream* ds );

void dstream_lock ( datastream* ds );
void dstream_unlock ( datastream* ds );

short dstream_push ( datastream* ds, tuple* tup, tv* t );
short dstream_pop ( datastream* ds, tuple** tup );
short dstream_peek ( datastream* ds, tuple** tup );

int dstream_length ( datastream *ds );

#endif /* DATASTREAM_H_ */
