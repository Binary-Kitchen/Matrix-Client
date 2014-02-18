#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <matrix-client-config.h>

#include "app-common.h"

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)
#define DEFAULT_PORT 1337

const char usagestring[] = "[options] server-address\n"
    "\n"
    "Options:\n"
    "  -p port       Specify Port (Default: " STR(DEFAULT_PORT) ")\n"
    "  -s            \"silence\": No local animation\n"
    "  -d            \"develop\": No remote connection\n"
    "\n"
    "Version:\n"
    "  libmatrix " MATRIX_CLIENT_VERSION_MAJOR "."
    MATRIX_CLIENT_VERSION_MINOR " (sources: " MATRIX_GIT_BRANCH "-"
    MATRIX_GIT_COMMIT_HASH ")\n";

void usage(const char *n)
{
	printf("%s: %s", n, usagestring);
}

void app_close()
{
	matrix_close();
}

int app_init(int argc, char **argv)
{
	int c;

	int port = DEFAULT_PORT;
	char *server = "";

	int silence = 0;
	int develop = 0;

	while ((c = getopt(argc, argv, "sdhp:")) != -1) {
		switch (c) {
		case 'h':
			usage(argv[0]);
			return 1;
			break;
		case 'p':
			port = atoi(optarg);
			break;
		case 's':
			silence = 1;
			break;
		case 'd':
			develop = 1;
			break;
		default:
			return 1;
		}
	}

	if (silence && develop) {
		puts("You can't use both, silence and develop mode");
		return -1;
	}

	if (optind < argc) {
		server = argv[optind];
	} else {
		if (!develop) {
			puts("You must specify the server!");
			return -1;
		}
	}

	if (matrix_init(silence, develop, server, port)) {
		printf("Unable to connect to %s:%d\nExiting\n", server, port);
		return -1;
	}

	printf("Successfully connected to %s:%d\n", server, port);

	return 0;
}
