#include <string.h>

#include "erl_interface.h"
#include "ei.h"

int main(int argc, char **argv)
{
	int sockfd;
	char servername[128];
	const char *hostname = "localhost";
	int offset;

	strcpy(servername, "messenger@");
	offset = strlen(servername);

	if ((argc > 1) && (argv[1]))
	{
		hostname = argv[1];
	}
	strncpy(servername + offset, hostname, sizeof(servername) - offset);
	servername[sizeof(servername)-1] = 0;

	erl_init(NULL, 0);

	if (!erl_connect_init(177, NULL, 0))
	{
		erl_err_quit("ERROR: erl_connect_init failed.");
		return 1;  /* NOTREACHED */
	}

	if ((sockfd = erl_connect(servername)) < 0)
	{
		erl_err_quit("ERROR: erl_connect failed for %s", servername);
		return 1;  /* NOTREACHED */
	}

	return 0;
}
