/*
 * stream_fncts.c
 *
 *  Created for cs546: Programming Languages
 *  Author: Kenneth J. Loomis
 */

#include "stream_fncts.h"

/*
 * Takes input from a single source file and enters it
 * into a new input stream.
 */
void *read_source ( input_stream* is )
{
	while ( ! feof ( is->fd ) )
	{
		tuple* tup = (tuple*) malloc ( sizeof ( tuple ) );
		tup->ts = ( tv* ) malloc ( sizeof ( tv ) );
		fscanf ( is->fd, "%d", &( tup->num ) );
		if (DEBUG)
		{
			printf ( "Input: %d\n", tup->num );
			fflush(NULL);
		}
		gettimeofday ( tup->ts, NULL );
		dstream_push ( is->stream, tup, tup->ts );
		usleep ( IN_DELAY );
	}
	if (DEBUG)
	{
		printf ( "Killing Input:\n");
		fflush(NULL);
	}
	return NULL;
}

/*
 * Takes input from a stream and writes it to a file.
 */
void *write_output ( output_stream* os )
{
	while ( 1 )
	{
		tv outTime, calcTime;
		tuple* tup;
		if ( dstream_pop ( os->stream, &tup ) )
		{
			if ( tup != NULL ) // Just in case
			{
				gettimeofday ( &( outTime ), NULL );
				subtract_timeval ( &outTime, tup->ts, &calcTime);
				if (DEBUG)
				{
					printf ( "Output: %d (%d)\n", tup->num, (int) tv2ms ( &calcTime ) );
					fflush (NULL);
				}
				fprintf ( os->fd, "%d (%d)\n", tup->num, (int) tv2ms ( &calcTime ) );
				fflush (os->fd);
				free (tup->ts);
				free (tup);
			}
		}
		usleep ( OUT_DELAY );
	}
	if (DEBUG)
	{
		printf ( "Killing Output:\n");
		fflush(NULL);
	}
	return NULL;
}

/*
 * Takes tuples for all streams and puts them in one
 * stream.
 */
void *ds_union ( stream_entry* se )
{
	while ( se->out_cnt ) // will exit if no output streams set
	{
		int iter=0;
		while ( iter < se->in_cnt )
		{
			tuple* tmp;
			if ( dstream_pop ( se->in_streams[iter], &tmp ) )
			{
				if (DEBUG)
				{
					printf ( "Union: %d\n", tmp->num );
					fflush(NULL);
				}
				// Always output to the first stream
				dstream_push ( se->out_streams[0], tmp, tmp->ts );
			}
			iter++;
		}
	}
	if (DEBUG)
	{
		printf ( "Killing Union:\n");
		fflush(NULL);
	}
	return NULL;
}

/*
 * Takes tuples for all streams and puts them in one
 * stream.
 */
void *ds_split ( stream_entry* se )
{
	while ( se->in_cnt ) // will exit if no input streams set
	{
		tuple* tmp;
		tuple* new_tup;
		int iter;
		// Always input from the first stream
		if ( dstream_pop ( se->in_streams[0], &tmp ) )
		{
			// Send copies
			iter=0;
			while ( iter < (se->out_cnt -1) )
			{
				new_tup = (tuple*) malloc ( sizeof ( tuple ) );
				new_tup->ts = ( tv* ) malloc ( sizeof ( tv ) );
				new_tup->num = tmp->num;
				copy_timeval ( new_tup->ts, tmp->ts);
				if (DEBUG)
				{
					printf ( "Split (c): %d\n", tmp->num );
					fflush(NULL);
				}
				dstream_push ( se->out_streams[iter], new_tup, new_tup->ts );
				iter++;
			}
			// Send original
			if (DEBUG)
			{
				printf ( "Split (o): %d\n", tmp->num );
				fflush(NULL);
			}
			dstream_push ( se->out_streams[iter], tmp, tmp->ts );
		}
	}
	if (DEBUG)
	{
		printf ( "Killing Split:\n");
		fflush(NULL);
	}
	return NULL;
}

/*
 * Experimental after this point
 */
/*
void *ds_filter_greater_n( stream_entry* se )
{
	int n = 3;
	int last = 1;
	while ( se->out_cnt && se->in_cnt ) // will exit if no streams set
	{
		tuple* tmp;
		// Always input from the first stream
		if ( dstream_pop ( se->in_streams[0], &tmp ) )
		{
			if (DEBUG)
			{
				printf ( "Filter>N: %d\n", tmp->num );
				fflush(NULL);
			}
			if ( last > tmp->num ) //we've come full circle
			{
				n++;
				last = tmp->num;
			}
			// Always output to the first stream
			if ( tmp->num < n )
				dstream_push ( se->out_streams[0], tmp, tmp->ts );
			else
				dstream_push ( se->out_streams[1], tmp, tmp->ts );
		}
	}
	if (DEBUG)
	{
		printf ( "Killing Filter>N:\n");
		fflush(NULL);
	}
	return NULL;
}

/*
 * Takes tuples from an input stream and puts it in the output
 * stream if the value is odd.
 */
/*
void *ds_filter_nth( stream_entry* se )
{
	int n = 2;
	int cnt = 0;
	int last = 0;
	while ( se->out_cnt && se->in_cnt ) // will exit if no streams set
	{
		tuple* tmp;
		// Always input from the first stream
		if ( dstream_pop ( se->in_streams[0], &tmp ) )
		{
			if (DEBUG)
			{
				printf ( "FilterNth: %d\n", tmp->num );
				fflush(NULL);
			}
			if ( last > tmp->num ) //we've come full circle
			{
				n++;
				cnt=0;
				last = tmp->num;
			}
			// Always output to the first stream
			if ( cnt != n )
			{
				dstream_push ( se->out_streams[0], tmp, tmp->ts );
				cnt++;
			}
			else
			{
				dstream_push ( se->err_stream, tmp, tmp->ts );
				cnt = 0;
			}
		}
	}
	if (DEBUG)
	{
		printf ( "Killing FilterNth:\n");
		fflush(NULL);
	}
	return NULL;
}
*/
