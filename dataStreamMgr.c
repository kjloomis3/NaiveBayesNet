/*
 * dataStreamFramework.c
 *
 *  Created for cs546: Programming Languages
 *  Author: Kenneth J. Loomis
 */

#include "dataStreamMgr.h"
#include "stream_fncts.h"

void dsm_init ( data_stream_mgr *dsm, int function_cnt, int input_cnt, int output_cnt )
{
	dsm->fnct_cnt = 0;
	dsm->in_cnt = 0;
	dsm->out_cnt = 0;

	// Set all the function entries
	dsm->s_entries = (stream_entry**) malloc ( sizeof (stream_entry*) * function_cnt );

	// Set all the input streams
	int cnt = 0;
	dsm->in_streams = (input_stream**) malloc ( sizeof (input_stream*) * input_cnt );
	while ( cnt < input_cnt )
	{
		dsm->in_streams[cnt] = (input_stream*) malloc ( sizeof (input_stream) );
		cnt++;
	}
	// Set all the output streams
	cnt = 0;
	dsm->out_streams = (output_stream**) malloc ( sizeof (output_stream*) * output_cnt );
	while ( cnt < input_cnt )
	{
		dsm->out_streams[cnt] = (output_stream*) malloc ( sizeof (output_stream) );
		cnt++;
	}
	// Set the error sink stream
	dsm->err_stream = (output_stream*) malloc ( sizeof (output_stream) );
	dsm->err_stream->stream = ( datastream* ) malloc ( sizeof ( datastream ) );
	dsm->err_stream->fd = fopen ( "error.dat", "w" );

}

void dsm_set_inputstream ( data_stream_mgr *dsm, FILE *fd, datastream* ds )
{
	dsm->in_streams[dsm->in_cnt]->stream = ds;
	dsm->in_streams[dsm->in_cnt]->fd = fd;
	dsm->in_cnt++;
}

void dsm_set_outputstream ( data_stream_mgr *dsm, FILE *fd, datastream* ds )
{
	dsm->out_streams[dsm->out_cnt]->stream = ds;
	dsm->out_streams[dsm->out_cnt]->fd = fd;
	dsm->out_cnt++;
}

void dsm_set_function ( data_stream_mgr *dsm, stream_entry* new_se )
{
	dsm->s_entries[dsm->fnct_cnt] = new_se;
	new_se->err_stream = dsm->err_stream->stream;
	dsm->fnct_cnt++;
}

void run ( data_stream_mgr *dsm )
{
	pthread_t threads[ dsm->in_cnt + dsm->out_cnt + dsm->fnct_cnt +1];
	int cnt = 0;
	int cnt2 = 0;
	// Start all the input streams
	while ( cnt < dsm->in_cnt )
	{
		pthread_create ( &threads[cnt2], (pthread_attr_t*) NULL,
				         ( void* ) read_source, dsm->in_streams[cnt] );
		cnt2++;
		cnt++;
	}
	// Start all the function threads
	cnt = 0;
	while ( cnt < dsm->fnct_cnt )
	{
		pthread_create ( &threads[cnt2], (pthread_attr_t*) NULL,
						 ( void* ) dsm->s_entries[cnt]->function,
				         dsm->s_entries[cnt] );
		cnt2++;
		cnt++;
	}
	// Start all the output streams
	cnt = 0;
	while ( cnt < dsm->out_cnt )
	{
		pthread_create ( &threads[cnt2], (pthread_attr_t*) NULL,
				         ( void* ) write_output, dsm->out_streams[cnt] );
		cnt2++;
		cnt++;
	}
	// Start the error sink
	pthread_create ( &threads[cnt2], (pthread_attr_t*) NULL,
					 ( void* ) write_output, dsm->err_stream );
	cnt2++;

	cnt = 0;
	while ( cnt < cnt2 )
		pthread_join( threads[cnt], NULL);

}

/*

int main ( )
{
	data_stream_mgr* dsm = ( data_stream_mgr* ) malloc ( sizeof ( data_stream_mgr ) );
	datastream* streams[6];

	// Initialization
	int cnt = 0;
	while ( cnt < 6 )
	{
		streams[cnt] = ( datastream* ) malloc ( sizeof ( datastream ) );
		create_dstream ( streams[cnt] );
		cnt++;
	}
	dsm_init ( dsm, 4, 1, 1 );

	// Define i/o streams
	FILE* fin = fopen ( "sieve.dat", "r" );
	FILE* fout = fopen ( "output.dat", "w" );
	if ( fin == NULL || fout == NULL )
		exit(0);
	dsm_set_inputstream ( dsm, fin, streams[0] );
	dsm_set_outputstream ( dsm, fout, streams[3] );

	// Define the first function entry
	stream_entry entry_one;
	entry_one.in_streams = &streams[2];
	entry_one.in_cnt = 1;
	entry_one.function = ds_filter_greater_n;
	entry_one.out_streams = &streams[3];
	entry_one.out_cnt = 2;

	dsm_set_function ( dsm, &entry_one);

	// Define second entry
	stream_entry entry_two;
	entry_two.in_streams = &streams[4];
	entry_two.in_cnt = 1;
	entry_two.function = ds_filter_nth;
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

*/


