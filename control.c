/*
 * control.c
 *
 *  Created for cs546: Programming Languages
 *  Author: Kenneth J. Loomis
 */

#include "control.h"

/*
 * control flow functions input/output test
 * 
 * This function uses the input and output pre-defined functions and the
 * split and union functions.
 * - Copies all tuples from the source file to the output file. Impliments
 * some simple stream direction functions. The output will have two duplicates
 * of the original.
 * 
 */ 
int main ( )
{
	data_stream_mgr* dsm = ( data_stream_mgr* ) malloc ( sizeof ( data_stream_mgr ) );
	datastream* streams[4];

	// Initialization
	int cnt = 0;
	while ( cnt < 4 )
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
	dsm_set_outputstream ( dsm, fout, streams[3] );

	// Define first entry
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
	entry_two.in_cnt = 2;
	entry_two.function = ds_union;
	entry_two.out_streams = &streams[3];
	entry_two.out_cnt = 1;

	dsm_set_function ( dsm, &entry_two);

	// Run it!
	run ( dsm );

	return 0;
}
