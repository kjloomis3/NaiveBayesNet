/*
 * datastream.c
 *
 *  Created for cs546: Programming Languages
 *  Author: Kenneth J. Loomis
 */

#include "datastream.h"

void create_dstream ( datastream* ds )
{
	ds->head = NULL;
	ds->tail = NULL;
	ds->count = 0;
	pthread_mutex_init ( &(ds->protect_ds), NULL );
}

void dstream_lock ( datastream* ds )
{
	//printf ("getting lock\n");
	//fflush (NULL);
	pthread_mutex_lock ( &(ds->protect_ds) );
	//printf ("locked\n");
	//fflush (NULL);
}

void dstream_unlock ( datastream* ds )
{
	//printf ("unlocking\n");
	//fflush (NULL);
	pthread_mutex_unlock ( &(ds->protect_ds) );
	//printf ("unlocked\n");
	//fflush (NULL);
}

short dstream_push ( datastream* ds, tuple* tup, struct timeval* t )
{
	stream_node* new_node = NULL;
	stream_node* cur_node = NULL;

	// Allocate new node and set the timestamp
	new_node = ( stream_node* ) malloc ( sizeof( stream_node ) );

	if ( new_node == NULL )
	{
		printf ("Something bad happened!\n");
		fflush(NULL);
		return 0;
	}

	dstream_lock ( ds );
	new_node->ts = (tv*) malloc ( sizeof (tv) );
	if ( t != NULL)
		copy_timeval ( new_node->ts, t);
	else
		copy_timeval ( new_node->ts, tup->ts);
	new_node->next = NULL;
	new_node->prev = NULL;
	new_node->info = tup;

	// Handle the case of an empty queue
	if ( ds->head == NULL )
	{
		ds->head = new_node;
		ds->tail = new_node;
	}
	// Handle the case of one or more items in the queue
	else
	{
		cur_node = ds->tail; // More likely to be adding item to the end
		while ( cur_node != ds->head && // cur_node >= new_node
				compare_timeval ( cur_node->ts, new_node->ts ) > 0 )
				cur_node = cur_node->prev;

		if ( cur_node == ds->head && // cur_node >= head
			 compare_timeval ( ds->head->ts, new_node->ts ) > 0 )
		{
			ds->head->prev = new_node;
			new_node->next = ds->head;
			ds->head = new_node;
			ds->count++;
			dstream_unlock ( ds );
			return 1;
		}
		else if ( cur_node == ds->tail && // cur_node >= head
				 compare_timeval ( ds->tail->ts, new_node->ts ) <= 0 )
		{
			ds->tail->next = new_node;
			new_node->prev = ds->tail;
			ds->tail = new_node;
			ds->count++;
			dstream_unlock ( ds );
			return 1;
		}
		else
		{
			new_node->next = cur_node->next;
			new_node->prev = cur_node;
			cur_node->next->prev = new_node;
			cur_node->next = new_node;
			ds->count++;
			dstream_unlock ( ds );
			return 1;
		}
		dstream_unlock ( ds );
		return 0;
	}
	ds->count++;
	dstream_unlock ( ds );
	return 1;
}

short dstream_pop ( datastream* ds, tuple** tup )
{
	stream_node* tmp;

	dstream_lock ( ds );
	// Handle the case of no items to pop
	if ( ds->head == NULL )
	{
		dstream_unlock ( ds );
		return 0;
	}
	else
	{
		// Move head pointer to next item
		tmp = ds->head;
		ds->head = ds->head->next;
		if ( ds->head != NULL ) // If other items in queue
		{
			ds->head->prev = NULL;
		}
		else
			ds->tail = NULL;
		// Extract data
		ds->count--;
		*tup = tmp->info;
	}
	dstream_unlock( ds );
	return 1;
}

short dstream_peek ( datastream* ds, tuple** tup )
{
	dstream_lock ( ds );
	// Handle the case of no items to peek
	if ( ds->head == NULL )
	{
		dstream_unlock ( ds );
		return 0;
	}
	else
	{
		// Extract data
		*tup = ds->head->info;
	}
	dstream_unlock( ds );
	return 1;
}

int dstream_length ( datastream *ds )
{
	return ds->count;
}

/*
 * Testing function

int main ()
{
	datastream d1;

	int cnt = 0;
	int* num[ 10 ];
	struct timeval* ts[10];
	while ( cnt < 10 )
	{
		num[cnt] = (int*) malloc ( sizeof (int) );
		cnt++;
	}
	create_dstream ( &d1 );

	cnt = 0;
	printf ( "Pushing:\t" );
	while ( cnt < 10 )
	{
		printf ( "%i\t", cnt );
		fflush( NULL );
		*num[cnt] = cnt;
		gettimeofday ( &( ts[cnt] ), NULL );
		dstream_push ( &d1, (void*)num[cnt], &( ts[cnt] ) );
		cnt++;
	}

	printf ( "\nPopping:\t" );
	cnt = 0;
	while ( cnt < 10 )
	{
		dstream_pop ( &d1, (void**)&num[cnt] );
		printf ( "%i\t", *num[cnt] );
		fflush( NULL );
		cnt++;
	}



	getchar ( );

	return 0;
}
 */
