/*
 * stream_fncts.h
 *
 *  Created for cs546: Programming Languages
 *  Author: Kenneth J. Loomis
 */

#ifndef STREAM_FNCTS_H_
#define STREAM_FNCTS_H_

#include <stdio.h>
#include "timeval.h"
#include "dataStreamMgr.h"

#define IN_DELAY 1000
#define OUT_DELAY 1000
#define DEBUG 0 // FALSE

void *read_source ( input_stream* is );

void *write_output ( output_stream* os );

void *ds_union ( stream_entry* se );

void *ds_split ( stream_entry* se );

//void *ds_filter_greater_n( stream_entry* se );

//void *ds_filter_nth( stream_entry* se );

#endif /* STREAM_FNCTS_H_ */
