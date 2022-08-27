# Linux more/less/cp commands

A custom C program that replicates the functionality of more, less, and cp Linux commands 

COMPILE
	
	gcc util.c -o util

NAME
	
	util - view and copy files
	
SYNOPSIS 

	./util SOURCE [OPTION]
	
DESCRIPTION

	view SOURCE file or copy SOURCE to DEST, copies/views first 10 lines as default
	
	-d		When desintation file is added, file will be copied
	
	-n		Specifiy the number of lines to displayed or printed
	
	-L		Specify viewing or copying the last n lines of a file
