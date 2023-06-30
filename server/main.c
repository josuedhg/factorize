#include <unistd.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

#include "server.h"

#ifdef TESTING_MODE
#define main test_main
#endif

#define DEFAULT_PORT "1024"
#define DEFAULT_NUMCONN 10

static struct option options[] = {
	{"port",		optional_argument, 0,  'p'},
	{"numconnections",	optional_argument, 0,  'c'},
	{"help",		no_argument	 , 0,  'h'},
	{0, 0, 0, 0},
};

static bool isnumber(char *s)
{
	while (*s != '\0')
		if (!isdigit(*s++)) return false;
	return true;
}

static void print_help(char *name)
{
	printf("%s [options]\n", name);
	printf("--port=N\t\tWhen N is the port number [default=%s]\n", DEFAULT_PORT);
	printf("--numconnections=N\tWhen N is the number of simultaneous connections to the server [default=%d]\n", DEFAULT_NUMCONN);
}

int main(int argc, char **argv)
{
	int arg = 0;
	int index = 0;
	char *port_num = DEFAULT_PORT;
	int numconn = DEFAULT_NUMCONN;
	while ( (arg = getopt_long(argc, argv, "hp:c:",
                        options, &index)) != -1) {

		switch(arg) {
			case 'p':
				port_num = optarg;
				if (!isnumber(port_num)) {
					fprintf(stderr, "port param should be a number: %s\n", port_num);
					exit(-1);
				}
			break;
			case 'c':
				if (!isnumber(optarg)) {
					fprintf(stderr, "numconnections param should be a number: %s\n", optarg);
					exit(-1);
				}
				numconn = strtoul(optarg, NULL, 0);
			break;
			case 'h':
				print_help(argv[0]);
				exit(-1);
			break;
		}
	}

	printf("port: %s\n", port_num);
	printf("max connections: %d\n", numconn);
	return server_run(port_num, numconn);
}
