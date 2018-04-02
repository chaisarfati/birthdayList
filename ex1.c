/*
 * ex1.c
 * edited by chai sarfati
 * id: 000802632
 */

#include <sys/types.h>
#include <sys/stat.h>

#include <fcntl.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_BUFFER_SIZE 65536
#define DESTINATION_FILE_MODE S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH

extern int opterr, optind;

void exit_with_usage(const char *message) {
	fprintf (stderr, "%s\n", message);
	fprintf (stderr, "Usage:\n\tex1 [-f] SOURCE DEST BUFFER_SIZE\n");
	exit(EXIT_FAILURE);
}

void copy_file(const char *source_file, const char *dest_file, int buffer_size, int force_flag) {
	/*
	 * Copy source_file content to dest_file, buffer_size bytes at a time.
	 * If force_flag is true, then also overwrite dest_file. Otherwise print error ("File exists"), and exit.
	 *
	 * TODO:
	 * 	1. Open source_file for reading
	 * 	2. Open dest_file for writing (Hint: is force_flag true?)
	 * 	3. Loop reading from source and writing to the destination buffer_size bytes each time
	 * 	4. Close source_file and dest_file
	 *
	 *  ALWAYS check the return values of syscalls for errors!
	 *  If an error was found, use perror(3) to print it with a helpful message, and then exit(EXIT_FAILURE)
	 */

	 // OPEN THE SOURCE FILE
	 int fdSource = open(source_file, O_RDONLY);
	 if(fdSource == -1){
		 perror("Unable to open source file for reading");
		 exit(EXIT_FAILURE);
	 }

	 // OPEN DESTINATION FILE
	 int fdDest = open(dest_file, O_CREAT|O_EXCL|O_WRONLY, DESTINATION_FILE_MODE);
	 if(fdDest == -1){
		 if(force_flag){
			 fdDest = open(dest_file, O_CREAT|O_WRONLY|O_TRUNC, DESTINATION_FILE_MODE);
if(fdDest == -1){ perror("Unable to open destination file for writing"); }
		 }else{
			 perror("Unable to open destination file for writing");
			 exit(EXIT_FAILURE);
		 }
	 }

	 // CREATE A BUFFER
	 void *buffer = malloc(buffer_size);
	 int bytesRead = 0, bytesWritten = 0;

	 // COPY CONTENT OF SOURCE TO DESTINATION
	 while( (bytesRead = read(fdSource, buffer, buffer_size)) != 0){
		 bytesWritten = write(fdDest, buffer, buffer_size);

		 if(bytesWritten == -1 || bytesRead == -1){
			 perror("Error occured in copying the file");
			 exit(EXIT_FAILURE);
		 }
	 }

	 // CLOSES FILES
	 if (close(fdSource) == -1) {
		 perror("Unable to close source file for writing");
		 exit(EXIT_FAILURE);
	 }
	 if (close(fdDest) == -1) {
		 perror("Unable to close destination file for writing");
		 exit(EXIT_FAILURE);
	 }
	 
	 printf("%s%s%s%s\n", "File ", source_file, " was copied to ", dest_file);
	 exit(EXIT_SUCCESS);
}

void parse_arguments(
		int argc, char **argv,
		char **source_file, char **dest_file, int *buffer_size, int *force_flag) {
	/*
	 * parses command line arguments and set the arguments required for copy_file
	 */
	int option_character;

	opterr = 0; /* Prevent getopt() from printing an error message to stderr */

	while ((option_character = getopt(argc, argv, "f")) != -1) {
		switch (option_character) {
		case 'f':
			*force_flag = 1;
			break;
		default:  /* '?' */
			exit_with_usage("Unknown option specified");
		}
	}

	if (argc - optind != 3) {
		exit_with_usage("Invalid number of arguments");
	} else {
		*source_file = argv[argc-3];
		*dest_file = argv[argc-2];
		*buffer_size = atoi(argv[argc-1]);

		if (strlen(*source_file) == 0 || strlen(*dest_file) == 0) {
			exit_with_usage("Invalid source / destination file name");
		} else if (*buffer_size < 1 || *buffer_size > MAX_BUFFER_SIZE) {
			exit_with_usage("Invalid buffer size");
		}
	}
}

int main(int argc, char **argv) {
	int force_flag = 0; /* force flag default: false */
	char *source_file = NULL;
	char *dest_file = NULL;
	int buffer_size = MAX_BUFFER_SIZE;

	parse_arguments(argc, argv, &source_file, &dest_file, &buffer_size, &force_flag);

	copy_file(source_file, dest_file, buffer_size, force_flag);

	return EXIT_SUCCESS;
}
