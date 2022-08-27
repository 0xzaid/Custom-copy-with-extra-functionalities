/*
Name: Zaid Al-Dirbashi
Start date: 20/8/2021
Last modified date: 27/8/2021


Program that contains some functionalties of common linux commands such as 
head, tail, and cp

*/


#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <libgen.h>
#include <stdbool.h>
#include <sys/stat.h>


void display_file(char file[], int n, int option);
void copy(char file_source[], char file_destination[], int n, int option);

char file_exists[100] = "File already exists\n";
char copy_success[100] = "Copy successful\n";
char no_path_error[100] = "Invalid argument: immediately after -d a directory path was expected\n";
char invalid_error[100] = "Invalid argument, no line numbers specified after the -n argument!\n";


int main(int argc, char *argv[]) 
{
	struct stat sb;
	char default_file[] = "sample.txt";
	bool copy_flag = false;
	bool num_flag = false;
	bool tail_flag = false;

	/*
	option indicates what mode to use is 0 which means head.
	if it is 1 that means tail
	*/
	int option = 0;
	int i = 0;
	int x = 0;
	int default_n = 10;


	//no arguements, just display the default file's content in terminal
	if(argc == 1){
		display_file(default_file, default_n, option);
	}
	else if(argc == 2){
		if(strcmp(argv[1], "-L") == 0){
			display_file(default_file, default_n, 1);
		} else {
			display_file(argv[1], default_n, option);
		}
	}
	//more than 2 args

	else {
		//figure out which argument
		for(i = 1; i < argc; i++){
			if(strcmp("-d", argv[i]) == 0){
				copy_flag = true;

			}else if(strcmp("-n", argv[i]) == 0){
				num_flag = true;
			}else if(strcmp("-L", argv[i]) == 0)
				tail_flag = true;
				option = 1;
		}

		if(num_flag == true){
			if(argc == 3){
				if(strcmp(argv[2], "-n") == 0){
					write(2, invalid_error, strlen(invalid_error));
					exit(1);
				}
				x = atoi(argv[2]);
				display_file(default_file, x, 0);
			}
			else if(argc == 4){
				x = atoi(argv[3]);
				display_file(argv[1], x, option);
			}

		} else if(num_flag == false && copy_flag == false && tail_flag == true){
			display_file(argv[1], default_n, option);

		} 
		else if(num_flag == true && copy_flag == false && tail_flag == true){

			if(argc == 3){
				if(strcmp(argv[1], "-n") == 0){
					x = atoi(argv[2]);
					display_file(default_file, x, option);
				} else if(strcmp(argv[1], "-L") ==  0 && strcmp(argv[2], "-n") == 0){
					x = atoi(argv[3]);
					display_file(default_file, x, option);
				}
			} else{
				if(strcmp(argv[2], "-n") == 0){
				x = atoi(argv[3]);
				display_file(argv[1], x, option);
			} else if(strcmp(argv[2], "-L") ==  0 && strcmp(argv[3], "-n") == 0){
				x = atoi(argv[4]);
				display_file(default_file, x, option);
				}
			}

		}
		else if(num_flag == false && copy_flag == true) {
			for(i = 1; i < argc; i++){
				if(strcmp("-d", argv[i]) == 0){
					if(stat(argv[i+1], &sb) == -1){
						write(2, no_path_error, strlen(no_path_error));
						exit(1);
					} else {
						if(argc == 1){
							copy(default_file, argv[i+1], default_n, option);
					} 	else {
							if(argc == 3){
								copy(default_file, argv[2], default_n, option);
							} else {
								copy(argv[1], argv[3], default_n, option);
								}
							}
					}
				}

			}

		} 
		else if(num_flag == true && copy_flag == true) {
			if(argc == 5){
				copy(default_file, argv[2], atoi(argv[4]), option);
			} else {
			copy(argv[1], argv[3], atoi(argv[5]), option);
				}
			}

		else if(num_flag == false && copy_flag == false && tail_flag == true){
			display_file(argv[1], default_n, option);
		}
	}

}

void display_file(char file[], int n, int option) {
	/* 
	When given a file, this function displays the content inside it.
	return: void
	*/
	if(option == 1){
		int mainfile = open(file, O_RDONLY);
		char c;
		int line_n = 0;
		int char_n = 0;
		int pos = 0;
		int state;


		while(line_n < n){
			if((state = read(mainfile, &c, 1)) < 0) {
				write(2, strerror(errno), strlen(strerror(errno)));
				exit(1);
			}
			else if(state == 0) break;
			else {
				if(pos == -1){
					pos = lseek(mainfile, 0, SEEK_END);
				}
				pos--;
				pos = lseek(mainfile, pos, SEEK_SET);
				if(c == '\n'){
					line_n++;
				}
				char_n++;
			}
			
		}
		if(line_n >= n){
			lseek(mainfile, 2, SEEK_CUR);
		} else {
			lseek(mainfile, 0, SEEK_SET);
		}
		char *lines = malloc(char_n - 1 * sizeof(char));
		read(mainfile, lines, char_n);
		lines[char_n - 1] = n;

		write(1, lines, char_n);
		close(mainfile);
		free(lines);
		exit(0);
	} 

		int bytes_read;

		// get size of file
		off_t file_lines = lseek(open(file, O_RDONLY),0, SEEK_END);
		// create suitable buffer
		char buffer[file_lines];
		lseek(open(file, O_RDONLY), 0, SEEK_SET);

		//if error while opening file, return error
		int mainfile = 0;
		if ((mainfile = open(file, O_RDONLY)) < 0) {
			write(2, strerror(errno), strlen(strerror(errno)));
			exit(1);
		}

		//no error, write content to standard output
		//using head
		while( (bytes_read = read(mainfile, buffer, sizeof(buffer)))) {
		for(int i = 0; n > 0; i++){
			if(bytes_read == 0){
				break;
			}
			if(buffer[i] == '\n') {
				n--;
			}
			char tmp = buffer[i];
			write(1, &tmp, 1);
			bytes_read--;
			}
		}
		close(mainfile);
		exit(0);
}

void copy(char file_source[], char file_destination[], int n, int option){
	/*
	takes in a file and a destination path and then copies the file 
	into the destination if file doesnt exist already
	*/
	int src, dst, in, out;

	// get size of file and create buffer
	off_t file_end = lseek(open(file_source, O_RDONLY), 0, SEEK_END);
	char buffer[file_end];
	lseek(open(file_source, O_RDONLY), 0, SEEK_SET);

	//if error while opening file
	if((src = open(file_source, O_RDONLY)) < 0) {
		write(2, strerror(errno), strlen(strerror(errno)));
		exit(2);
	}
	//file doesnt exist, create, else error
	if(access(strcat(file_destination, basename(file_source)), F_OK) == -1) {
		dst = creat(file_destination, 0666);
	} else {
		write(2, file_exists, sizeof(file_exists));
		exit(2);
	}
	while(1) {

		in = read(src, buffer, sizeof(buffer));
		
		if(in <= 0){
		break;
		}
		for(int i = 0; n > 0; i++){
			if(in == 0){
				break;
			}
			if(buffer[i] == '\n') {
				n--;
			}
			char tmp = buffer[i];
			write(dst, &tmp, 1);
			in--;
			}
	}
	//close file and exit
	close(src);
	close(dst);
	write(1, copy_success, sizeof(copy_success));
	exit(0);
}
