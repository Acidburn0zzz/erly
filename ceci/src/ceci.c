#include "erl_interface.h"
#include "ei.h"

int main(int argc, char *argv)
{
	int sockfd;
	char *nodename="xyz@chivas.du.etx.ericsson.se"; /* An example */
	if ((sockfd = erl_connect(nodename)) < 0)
	{
		erl_err_quit("ERROR: erl_connect failed");
		return 1;
	}
	return 0;
}
