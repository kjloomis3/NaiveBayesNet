/*
 * filter_odd.h
 *
 *  Created for cs546: Programming Languages
 *  Author: Kenneth J. Loomis
 */

#ifndef FILTER_ODD_H_
#define FILTER_ODD_H_

#include <stdio.h>
#include "timeval.h"
#include "dataStreamMgr.h"
#include "stream_fncts.h"

void *ds_filter_odd( stream_entry* se );

#endif