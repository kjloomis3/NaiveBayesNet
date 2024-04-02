/*
 * filter_odd.c
 *
 *  Created for cs546: Programming Languages
 *  Author: Kenneth J. Loomis
 */

#include "filter_odd.h"

/*
 * Takes tuples from an input stream and puts it in the output
 * stream if the value is odd.
 */
void *ds_filter_odd( stream_entry* se )
{
	while ( se->out_cnt && se->in_cnt ) // will exit if no streams set
	{
		tuple* tmp;
		// Always input from the first stream
		if ( dstream_pop ( se->in_streams[0], &tmp ) )
		{
			if (DEBUG)
			{
				printf ( "Filter: %d\n", tmp->num );
				fflush(NULL);
			}
			// Always output to the first stream
			if ( tmp->num % 2 == 0)
				dstream_push ( se->out_streams[0], tmp, tmp->ts );
			else
				dstream_push ( se->err_stream, tmp, tmp->ts );
		}
	}
	if (DEBUG)
	{
		printf ( "Killing Filter:\n");
		fflush(NULL);
	}
	return NULL;
}

/*
 * filter_odd input/output test
 * 
 * - Copies all tuples from the source file to the output file. Impliments
 * some simple stream direction functions. The output will have two duplicates
 * of the original even numbers but only a single duplicate of the odd numbers.
 * 
 */ 
int main ( )
{
	data_stream_mgr* dsm = ( data_stream_mgr* ) malloc ( sizeof ( data_stream_mgr ) );
	datastream* streams[5];

	// Initialization
	int cnt = 0;
	while ( cnt < 5 )
	{
		streams[cnt] = ( datastream* ) malloc ( sizeof ( datastream ) );
		create_dstream ( streams[cnt] );
		cnt++;
	}
	dsm_init ( dsm, 2, 1, 1 );

	// Define i/o streams
	FILE* fin = fopen ( "source1.dat", "r" );
	FILE* fout = fopen ( "output.dat", "w" );
	if ( fin==NULL || fout == NULL )
		exit(0);
	dsm_set_inputstream ( dsm, fin, streams[0] );
	dsm_set_outputstream ( dsm, fout, streams[4] );

	// Define the first function entry
	stream_entry entry_one;
	entry_one.in_streams = &streams[0];
	entry_one.in_cnt = 1;
	entry_one.function = ds_split;
	entry_one.out_streams = &streams[1];
	entry_one.out_cnt = 2;

	dsm_set_function ( dsm, &entry_one);

	// Define second entry
	stream_entry entry_two;
	entry_two.in_streams = &streams[1];
	entry_two.in_cnt = 1;
	entry_two.function = ds_filter_odd;
	entry_two.out_streams = &streams[3];
	entry_two.out_cnt = 1;

	dsm_set_function ( dsm, &entry_two);

	// Define three entry
	stream_entry entry_three;
	entry_three.in_streams = &streams[2];
	entry_three.in_cnt = 2;
	entry_three.function = ds_union;
	entry_three.out_streams = &streams[4];
	entry_three.out_cnt = 1;

	dsm_set_function ( dsm, &entry_three);

	// Run it!
	run ( dsm );

	return 0;
}