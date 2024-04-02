/*
 * multiply3.c
 *
 *  Created for cs546: Programming Languages
 *  Author: Kenneth J. Loomis
 */

#include "multiply3.h"


/*
 * Takes tuples from an input stream and puts it in the output
 * stream with the number multiplied by 3.
 */
void *ds_multiply( stream_entry* se )
{
	while ( se->out_cnt && se->in_cnt ) // will exit if no streams set
	{
		tuple* tmp;
		// Always input from the first stream
		if ( dstream_pop ( se->in_streams[0], &tmp ) )
		{
			tmp->num = tmp->num * 3;
			if (DEBUG)
			{
				printf ( "Multiply: %d\n", tmp->num );
				fflush(NULL);
			}
			// Always output to the first stream
			dstream_push ( se->out_streams[0], tmp, tmp->ts );
		}
	}
	if (DEBUG)
	{
		printf ( "Killing Multiply:\n");
		fflush(NULL);
	}
	return NULL;
}


/*
 * Mulitplications function input/output test
 * 
 * - Copies all tuples from the source file to the output file. The output tuples
 * will be mulitplied by three.
 * 
 */ 
int main ( )
{
	data_stream_mgr* dsm = ( data_stream_mgr* ) malloc ( sizeof ( data_stream_mgr ) );
	datastream* ds1 = ( datastream* ) malloc ( sizeof ( datastream ) );
	datastream* ds2 = ( datastream* ) malloc ( sizeof ( datastream ) );

	// Initialization
	create_dstream ( ds1 );
	create_dstream ( ds2 );
	dsm_init ( dsm, 0, 1, 1 );

	// Define i/o streams
	FILE* fin = fopen ( "source1.dat", "r" );
	FILE* fout = fopen ( "output.dat", "w" );
	dsm_set_inputstream ( dsm, fin, ds1 );
	dsm_set_outputstream ( dsm, fout, ds2 );

	// Define the first function entry
	stream_entry entry_one;
	entry_one.in_streams = &ds1;
	entry_one.in_cnt = 1;
	entry_one.function = ds_multiply;
	entry_one.out_streams = &ds2;
	entry_one.out_cnt = 1;

	dsm_set_function ( dsm, &entry_one);
	
	
	run ( dsm );

	return 0;
}