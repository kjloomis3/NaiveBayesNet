** README file for Project 5 **

Ken Loomis
CS 536
Project 5

This is the accompanying readme file for project five. I have includes some documentation in the code and header files, but the majority of the documentation can be found here. The purpose of this project was to generate an extension to the C language that is similar to a domain-specific programming language. 

For the purpose of this assignment I did make some assumptions that somewhat limit the capacity of this system. Most notible the limitation on the input types: only integers are used as input tuples. Also the only current input source is a file as opposed to some other input stream source. Another limitation of this system is that the arrival rate of the input tuples is controlled. In a traditional streaming environment this would not be the case, but since the tuples are being read from a file, the rate is controlled and can be adjusted by setting the sleep time between tuple reads.

In the most general sense, this pseudo-language seeks to ease the difficulties of implimenting a multi-threaded or traditional concurrent program that follows the streaming data paradigm. Programmers define functions (the operations nodes in the streams) and define how these functions are connected to each other through data streams. 

Organization
    
The library is organized into three main files (or source code, header file pairs):
	- datastream: The definition of a stream that would operate between functions. This stream is a implemented as a doubly-linked priority queue. The tuples are inserted in order of arriving time stamp. Alternately, the current time can be passed in instead to insert the tuples in arrival order.
	- dataStreamMgr: The method by which the data streams and funtions are organized into a meaningful graph. I had planned on implementing graph checking to ensure that every datastream has a starting or ending function, and that the graph was complete. However, this was not implemented.
	- stream_fncts: The "built-in" operations for streams. I wrote the input and output stream functions. Also included are two control functions: union and split. Other program specific functions have been written, and they are included in the respective testing files.
	- I also included a timeval library written by Jeongyeup Paek of the University of Southern California. This extends the functionality of the timeval struct datatype which I found useful.

Coding Methods

Since this is really a library for C instead of a real language, I found myself quite limited by the C language, but I was able to manage. The following is the general coding method employed in this pseudo-language. The methodology is organized into three main steps: definition and initialization of the streams (including the input and output streams), definition of the functions and the interconnectivity between functions, and finally running it.

    Definition and Initialization:
	Before they can be used, the streams must be initialized. Each of the streams must be declared and then initialized by a data_stream_manager. See the following example.

	*********************************************************************************
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
	*********************************************************************************

	Here a data stream manager is declared as well as 4 data streams (organized into an array). Then the data stream manager must be initialized by defining the number of functions (2), the number of input sources (1), and the number of output sinks (1). These definitions exclude the predefined error sink which is always written to error.dat.

	*********************************************************************************
	  // Define i/o streams
	  FILE* fin = fopen ( "source1.dat", "r" );
	  FILE* fout = fopen ( "output.dat", "w" );
	  if ( fin==NULL || fout == NULL )
		  exit(0);
	  dsm_set_inputstream ( dsm, fin, streams[0] );
	  dsm_set_outputstream ( dsm, fout, streams[3] );
	*********************************************************************************
	
	Next the input and output sources are defined. A file descriptor must be created for each then then passed to the data stream manager with the appropriate functions (see above) to set those as inputstream sources or outputstream sinks by also passing in one of the previously defined streams. In this example, stream 0 is the input stream the will produce tuples found in the file "source1.dat" using the fin file descriptor.

    Definition of functions and interconnectivity:

	Next the intermediate functions must be defined. First the members of a struct must be defined to represent this edge in the graph. Each must a set of input streams pointers (or just one) arranged in an array, the total count of these input streams, the function pointer, the set of output stream pointers for this function, and the total count of the output streams. In the example below, the function ds_split() has 1 input stream (0) and 2 output streams (1,2).

	*********************************************************************************
	  // Define first entry
	  stream_entry entry_one;
	  entry_one.in_streams = &streams[0];
	  entry_one.in_cnt = 1;
	  entry_one.function = ds_split;
	  entry_one.out_streams = &streams[1];
	  entry_one.out_cnt = 2;

	  dsm_set_function ( dsm, &entry_one);
	*********************************************************************************
	
	Once the struct is defined the struct must be added to the data stream manager by setting it with the dsm_set_function, which adds this entry to the graph.

    Running the code:

	The setup is the tricky part. Running the code is easy.

	*********************************************************************************
	  // Run it!
	  run ( dsm );
	*********************************************************************************
	
	Since no form of intercommunication has been implemented here, the functions operate idependently of one another in sepearte threads. Therefore, there is not way of determining when to stop execution. Future work could implement message passing or "puncuation" tuples to handle this.

Code examples:

Included in this package is some code examples described breifly here.

      Source ---> Output

    simple.c: This program use the simplist one can get. An input stream and output streams are defined. Essentially, it copies the tuples from one file to an output file.

		     ---------->
      Source ---> Split       Union ---> Output
		     ---------->

    control.c: This program uses the control functions to direct the flow of the data streams. Union conbines two streams into one, passing all arriving tuples from either stream into a single output stream. Split creates duplicate copies of the tuples and passes one to each of the output streams.

		     ----> Filter_Odd ---->
      Source ---> Split                  Union ---> Output
		     --------------------->

    filter_odd.C: Includes an additional function that filters out the odd tuples on one of the split streams. The output should contain twice the number of even tuples as the original.
	
      Source ---> Multiply ---> Output

    multiply3.c: This program simple multipies all the tuples by three.

      Source ---> Union --------------------> Split ---> Output	
		     <------- Fibonacci <-------

    fibonacci.c: This program produces the fibonacci sequence given a seed tuple. This will quickly exceed the maximum representable size of an integer however.

