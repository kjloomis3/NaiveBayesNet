/*
 * dataStreamFramework.h
 *
 *  Created for cs546: Programming Languages
 *  Author: Kenneth J. Loomis
 */

#ifndef DATASTREAMMGR_H_
#define DATASTREAMMGR_H_

#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include "datastream.h"

typedef struct
{
	FILE* fd;
	datastream* stream;
} input_stream;

typedef struct
{
	FILE* fd;
	datastream* stream;
} output_stream;

typedef struct
{
	datastream** in_streams;
	int in_cnt;
	void* function;
	datastream** out_streams;
	int out_cnt;
	datastream* err_stream;
} stream_entry;

typedef struct
{
	stream_entry** s_entries;
	int fnct_cnt;
	input_stream** in_streams;
	int in_cnt;
	output_stream** out_streams;
	int out_cnt;
	output_stream* err_stream;
} data_stream_mgr;

void dsm_init ( data_stream_mgr *dsm, int stream_cnt, int input_cnt, int output_cnt );

void dsm_set_inputstream ( data_stream_mgr *dsm, FILE *fd, datastream* ds );

void dsm_set_outputstream ( data_stream_mgr *dsm, FILE *fd, datastream* ds );

void dsm_set_function ( data_stream_mgr *dsm, stream_entry* new_se );

void run ( data_stream_mgr *dsm );

#endif /* DATASTREAMMGR_H_ */
