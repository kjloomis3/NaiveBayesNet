/*
 * simple.c
 *
 *  Created for cs546: Programming Languages
 *  Author: Kenneth J. Loomis
 */

#include "simple.h"

/*
 * simple input/output test
 * 
 * This function uses only the input and output pre-defined functions.
 * - Copies all tuples from the source file to the output file.
 * 
 */ 
int main ( )
{
	data_stream_mgr* dsm = ( data_stream_mgr* ) malloc ( sizeof ( data_stream_mgr ) );
	datastream* ds = ( datastream* ) malloc ( sizeof ( datastream ) );

	// Initialization
	create_dstream ( ds );
	dsm_init ( dsm, 0, 1, 1 );

	// Define i/o streams
	FILE* fin = fopen ( "source1.dat", "r" );
	FILE* fout = fopen ( "output.dat", "w" );
	dsm_set_inputstream ( dsm, fin, ds );
	dsm_set_outputstream ( dsm, fout, ds );

	run ( dsm );

	return 0;
}
