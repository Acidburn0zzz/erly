/*
 *  Copyright 2017, Adriaan de Groot <groot@kde.org>
 *
 *  Redistribution and use is allowed according to the terms of the two-clause BSD license.
 *     https://opensource.org/licenses/BSD-2-Clause
 *     SPDX short identifier: BSD-2-Clause
 */

/*
 * A very, *very* basic Erlang node that sends a single "pong" message
 * to the pong service (see messenger.erl).
 */

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

	ETERM *term = erl_mk_atom( "helo" );
	if (!erl_reg_send(sockfd, "pong", term))
	{
		erl_err_sys("ERROR: erl_reg_send failed for %s", servername);
		return 1;  /* NOTREACHED */
	}

	erl_free_term(term);

	return 0;
}
