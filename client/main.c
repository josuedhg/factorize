#include <stdint.h>
#include <unistd.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

#include "client.h"

#ifdef TESTING_MODE
#define main test_main
#endif

#define DEFAULT_PORT "1024"
#define DEFAULT_HOST "127.0.0.1"

static struct option options[] = {
	{"port",	optional_argument, 0,  'p'},
	{"host",	optional_argument, 0,  's'},
	{"nums",	required_argument, 0,  'n'},
	{"help",	no_argument,	   0,  'h'},
	{0, 0, 0, 0},
};

static bool isnumber(char *s, char limiter)
{
	if (*s == limiter || *s == '\0') return false;
	while (*s != limiter && *s != '\0')
		if (!isdigit(*s++)) return false;
	return true;
}

static uint8_t comacounter(char *str)
{
	uint8_t counter = 0;
	while (*str != '\0')
		if (*str++ == ',') counter++;
	return counter;
}

static void print_help(char *name)
{
	printf("%s [options] --nums=N1,N2,...\n", name);
	printf("--port=N\t\tWhen N is the port number [default=%s]\n", DEFAULT_PORT);
	printf("--host=IP\t\tWhen IP is the server ip [default=%s]\n", DEFAULT_HOST);
	printf("--nums=N1,N2,...\tThis param gets the list of numberrs to be factorized [required]\n");
}

static int16_t parsenums(char *comanumbers, uint16_t **nums)
{
	int16_t size = comacounter(comanumbers) + 1;
	uint16_t *numbers = calloc(1, sizeof(uint16_t) * size);
	char *current = comanumbers;
	char *nextcoma = NULL;

	for (int i = 0; i < size; i++) {
		nextcoma = strchr(current, ',');
		if (!isnumber(current, ',')) {
			free(numbers);
			return -1;
		}
		uint16_t tmpnum = strtoul(current, NULL, 0);
		numbers[i] = tmpnum;
		current = nextcoma + 1;
	}
	*nums = numbers;
	return size;
}

int main(int argc, char **argv)
{
	int arg = 0;
	int index = 0;
	uint16_t *nums = NULL;
	int16_t nums_size = 0;
	char *portnum = DEFAULT_PORT;
	char *hostip = DEFAULT_HOST;
	int ret = 0;
	
	while ( (arg = getopt_long(argc, argv, "hp:s:n:",
                        options, &index)) != -1) {

		switch(arg) {
			case 'p':
				portnum = optarg;
				if (!isnumber(portnum, '\0')) {
					fprintf(stderr, "port param should be a number: %s\n", portnum);
					exit(-1);
				}
			break;
			case 's':
				hostip = optarg;
			break;
			case 'n':
				nums_size = parsenums(optarg, &nums);
				if (nums_size == -1) {
					fprintf(stderr, "nums params should contain coma serparated numbers: %s\n", optarg);
					exit(-1);
				}
			break;
			case 'h':
				print_help(argv[0]);
				exit(-1);
			break;
		}
	}

	if (nums == NULL) {
		fputs("nums is a required parameter\n", stderr);
		exit(-1);
	}

	ret = client_run(hostip, portnum, nums, nums_size);

	free(nums);
	return ret;
}
