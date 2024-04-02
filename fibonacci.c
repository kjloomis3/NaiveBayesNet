/*
 * fabonacci.c
 *
 *  Created for cs546: Programming Languages
 *  Author: Kenneth J. Loomis
 */

#include "fibonacci.h"

/*
 * Takes tuples from an input stream and puts it in the output
 * stream with the number added to the last number seen in the stream.
 */
void *ds_fibonacci( stream_entry* se )
{
	int last = 0;
	while ( se->out_cnt && se->in_cnt ) // will exit if no streams set
	{
		tuple* tmp;
		// Always input from the first stream
		if ( dstream_pop ( se->in_streams[0], &tmp ) )
		{
			int i = tmp->num;
			tmp->num = tmp->num + last;
			last = i;
			if (DEBUG)
			{
				printf ( "Fibonacci: %d\n", tmp->num );
				fflush(NULL);
			}
			// Always output to the first stream
			dstream_push ( se->out_streams[0], tmp, tmp->ts );
		}
	}
	if (DEBUG)
	{
		printf ( "Killing Fibonacci:\n");
		fflush(NULL);
	}
	return NULL;
} 

/*
 * fibonacci input/output test
 * 
 * This function shows the recursive capabilites of the streams. Generates all the 
 * fibonacci numbers starting from the seed value located in the fib_seed file. Quickly
 * exceeds the bounds of an int.
 * 
 */ 
int main ( )
{
	data_stream_mgr* dsm = ( data_stream_mgr* ) malloc ( sizeof ( data_stream_mgr ) );
	datastream* streams[6];

	// Initialization
	int cnt = 0;
	while ( cnt < 5 )
	{
		streams[cnt] = ( datastream* ) malloc ( sizeof ( datastream ) );
		create_dstream ( streams[cnt] );
		cnt++;
	}
	dsm_init ( dsm, 3, 1, 1 );

	// Define i/o streams
	FILE* fin = fopen ( "fib_seed.dat", "r" );
	FILE* fout = fopen ( "output.dat", "w" );
	if ( fin==NULL || fout == NULL )
		exit(0);
	dsm_set_inputstream ( dsm, fin, streams[0] );
	dsm_set_outputstream ( dsm, fout, streams[3] );

	// Define the first function entry
	stream_entry entry_one;
	entry_one.in_streams = &streams[2];
	entry_one.in_cnt = 1;
	entry_one.function = ds_split;
	entry_one.out_streams = &streams[3];
	entry_one.out_cnt = 2;

	dsm_set_function ( dsm, &entry_one);

	// Define second entry
	stream_entry entry_two;
	entry_two.in_streams = &streams[4];
	entry_two.in_cnt = 1;
	entry_two.function = ds_fibonacci;
	entry_two.out_streams = &streams[1];
	entry_two.out_cnt = 1;

	dsm_set_function ( dsm, &entry_two);

	// Define three entry
	stream_entry entry_three;
	entry_three.in_streams = &streams[0];
	entry_three.in_cnt = 2;
	entry_three.function = ds_union;
	entry_three.out_streams = &streams[2];
	entry_three.out_cnt = 1;

	dsm_set_function ( dsm, &entry_three);


	// Run it!
	run ( dsm );

	return 0;
}