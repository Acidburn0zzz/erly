/*
 *  Copyright 2017, Adriaan de Groot <groot@kde.org>
 *
 *  Redistribution and use is allowed according to the terms of the two-clause BSD license.
 *     https://opensource.org/licenses/BSD-2-Clause
 *     SPDX short identifier: BSD-2-Clause
 */

/*
 * A very basic Pulley backend plugin that talks to an Erlang node.
 */


#include <steamworks/pulleyback.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <erl_interface.h>
#include <ei.h>

static const char logger[] = "steamworks.pulleyback.erl";
static int num_instances = 0;

#define NAME_SIZE	(64)

typedef struct {
	int instancenumber;
	int varc;
	ETERM *atom_add;
	ETERM *atom_del;
	char nodename[NAME_SIZE];
	char servicename[NAME_SIZE];
	int sockfd;
	ETERM **terms;
} handle_t;

static ETERM *make_atom(const char *atom)
{
	char ibuf[64];

	ETERM *t = erl_mk_atom( atom );
	if (t == NULL)
	{
		ibuf[0] = 0;
		snprintf(ibuf, sizeof(ibuf), "Could not create Erlang atom '%s'", atom);
		ibuf[sizeof(ibuf)-1] = 0;
		write_logger(logger, ibuf);
		return NULL;
	}

	return t;
}

void *pulleyback_open(int argc, char **argv, int varc)
{
	char ibuf[64];

	write_logger(logger, "erl backend opened.");
	snprintf(ibuf, sizeof(ibuf), " .. %d parameters, expect %d vars later", argc, varc);
	write_logger(logger, ibuf);
	// snprintf(ibuf, sizeof(ibuf), " .. %d variables", varc);

	int servername_index = -1;
	int messagename_index = -1;
	for (unsigned int i=0; i<argc; i++)
	{
		snprintf(ibuf, sizeof(ibuf), "  .. parm %d=%s", i, argv[i]);
		write_logger(logger, ibuf);

		if (strncmp(argv[i], "tgt=\"", 5) == 0)
		{
			servername_index = i;
		}
		else if (strncmp(argv[i], "msg=\"", 5) == 0)
		{
			messagename_index = i;
		}
	}

	if ((servername_index >= 0) && (argv[servername_index][strlen(argv[servername_index])-1] != '"'))
	{
		write_logger(logger, "tgt= parameter is invalid, does not end in \"");
		servername_index = -1;
	}
	if ((messagename_index >= 0) && (argv[messagename_index][strlen(argv[messagename_index])-1] != '"'))
	{
		write_logger(logger, "msg= parameter is invalid, does not end in \"");
		messagename_index = -1;
	}
	if ((servername_index >= 0) && (strlen(argv[servername_index]) >= NAME_SIZE))
	{
		write_logger(logger, "tgt= parameter is too long");
		servername_index = -1;
	}
	if ((messagename_index >= 0) && (strlen(argv[messagename_index]) >= NAME_SIZE))
	{
		write_logger(logger, "msg= parameter is too long");
		messagename_index = -1;
	}

	if (servername_index < 0)
	{
		write_logger(logger, "No tgt= parameter given");
	}
	if (messagename_index < 0)
	{
		write_logger(logger, "No msg= parameter given");
	}
	if ((messagename_index < 0) || (servername_index < 0))
	{
		return NULL;
	}

	erl_init(NULL, 0);
	if (!erl_connect_init(3800 + num_instances, NULL, 0))
	{
		write_logger(logger, "Could not initialize Erlang connection");
		return NULL;
	}

	handle_t* handle = malloc(sizeof(handle_t));
	if (handle == NULL)
	{
		snprintf(ibuf, sizeof(ibuf), "Could not allocate handle %ld (#%d)", sizeof(handle_t), num_instances+1);
		write_logger(logger, ibuf);
		/* assume malloc() has set errno */
		return NULL;
	}

	handle->terms = calloc(varc + 2, sizeof(ETERM *));
	if (handle->terms == NULL)
	{
		write_logger(logger, "Could not allocate ETERM space");
		free(handle);
		return NULL;
	}

	if ((handle->atom_add = make_atom("add")) == NULL)
	{
		free(handle->terms);
		free(handle);
		return NULL;
	}

	if ((handle->atom_del = make_atom("del")) == NULL)
	{
		erl_free_term(handle->atom_add);
		free(handle->terms);
		free(handle);
		return NULL;
	}

	int l;
	l = strlen(argv[servername_index]);
	strncpy(handle->nodename, argv[servername_index]+5, l-6);
	l = strlen(argv[messagename_index]);
	strncpy(handle->servicename, argv[messagename_index]+5, l-6);

	if ((handle->sockfd = erl_connect(handle->nodename)) < 0)
	{
		snprintf(ibuf, sizeof(ibuf), "Could not connect to '%s'", handle->nodename);
		write_logger(logger, ibuf);

		erl_free_term(handle->atom_add);
		erl_free_term(handle->atom_del);
		free(handle->terms);
		free(handle);
		return NULL;
	}

	handle->instancenumber = ++num_instances;
	handle->varc = varc;

	snprintf(ibuf, sizeof(ibuf), "erl backend handle %p (#%d)", (void *)handle, handle->instancenumber);
	write_logger(logger, ibuf);
	return handle;
}

void pulleyback_close(void *pbh)
{
	char ibuf[64];
	handle_t* handle = pbh;

	snprintf(ibuf, sizeof(ibuf), "erl backend close %p", (void *)handle);
	write_logger(logger, ibuf);
	snprintf(ibuf, sizeof(ibuf), " .. instance %d", handle->instancenumber);
	write_logger(logger, ibuf);

	erl_close_connection(handle->sockfd);
	erl_free_term(handle->atom_add);
	erl_free_term(handle->atom_del);
	free(handle>terms);
	free(pbh);
}

void dump_der(ETERM **term, int argc, der_t der)
{
	char ibuf[64];
	*term = NULL;

	if (!der)
	{
		snprintf(ibuf, sizeof(ibuf), "  .. arg %d data=NULL", argc);
		write_logger(logger, ibuf);
		return;
	}

	if (der[0] != 0x04)
	{
		snprintf(ibuf, sizeof(ibuf), "  .. arg %d data=TAG(%02x)", argc, (int)(der[0]));
		write_logger(logger, ibuf);
		return;
	}


	size_t len = 0;
	if (der[1] < 0x80)
	{
		len = der[1];
		der += 2; // 0=tag 1=short len
	}
	else
	{
		len = 0x80;  // length > 128, cut down later, and der[1] is the length of the length
		uint8_t len_len = der[1] & 0x7f;
		der += 2 + len_len;
	}

	snprintf(ibuf, sizeof(ibuf), "  .. arg %d data=", argc);
	int offset = strlen(ibuf);
	len = len >= sizeof(ibuf) - offset  ? sizeof(ibuf) - offset - 1 : len;
	memcpy(ibuf+offset, der, len);
	ibuf[offset+len] = 0;
	write_logger(logger, ibuf);

	*term = erl_mk_estring(der, len);
}

int pulleyback_add(void *pbh, der_t *forkdata)
{
	char ibuf[64];
	handle_t* handle = pbh;

	snprintf(ibuf, sizeof(ibuf), "erl backend add %p", (void *)handle);
	write_logger(logger, ibuf);
	snprintf(ibuf, sizeof(ibuf), "  .. instance #%d add data @%p", handle->instancenumber, (void *)forkdata);
	write_logger(logger, ibuf);

	der_t* p = forkdata;
	for (unsigned int i = 0; i < handle->varc; i++)
	{
		snprintf(ibuf, sizeof(ibuf), "  .. arg %d  der@%p", i, *p);
		write_logger(logger, ibuf);
		dump_der(&(handle->terms[i]), i, *p);
		p++;
	}

	ETERM *msg_inner[2];
	msg_inner[0] = handle->atom_add;
	msg_inner[1] = erl_mk_tuple(handle->terms, handle->varc);

	ETERM *msg = erl_mk_tuple(msg_inner, 2);
	erl_send_reg(handle->sockfd, handle->servicename, msg);
	erl_free_term(msg);
	erl_free_term(msg_inner[1]);
	for (unsigned int i = 0; i<handle->varc; i++)
	{
		erl_free_term(handle->terms[i]);
	}

	return 1;
}

int pulleyback_del(void *pbh, der_t *forkdata)
{
	char ibuf[64];
	handle_t* handle = pbh;

	snprintf(ibuf, sizeof(ibuf), "erl backend del %p", (void *)handle);
	write_logger(logger, ibuf);
	snprintf(ibuf, sizeof(ibuf), "  .. instance #%d del data @%p", handle->instancenumber, (void *)forkdata);
	write_logger(logger, ibuf);

	return 1;
}

int pulleyback_reset(void *pbh)
{
	char ibuf[64];
	handle_t* handle = pbh;

	snprintf(ibuf, sizeof(ibuf), "erl backend reset %p", (void *)handle);
	write_logger(logger, ibuf);

	return 1;
}

int pulleyback_prepare(void *pbh)
{
	char ibuf[64];
	handle_t* handle = pbh;

	snprintf(ibuf, sizeof(ibuf), "erl backend prepare %p", (void *)handle);
	write_logger(logger, ibuf);

	return 1;
}

int pulleyback_commit(void *pbh)
{
	char ibuf[64];
	handle_t* handle = pbh;

	snprintf(ibuf, sizeof(ibuf), "erl backend commit %p", (void *)handle);
	write_logger(logger, ibuf);

	return 1;
}

void pulleyback_rollback(void *pbh)
{
	char ibuf[64];
	handle_t* handle = pbh;

	snprintf(ibuf, sizeof(ibuf), "erl backend rollback %p", (void *)handle);
	write_logger(logger, ibuf);
}

int pulleyback_collaborate(void *pbh1, void *pbh2)
{
	char ibuf[64];
	handle_t* handle = pbh1;

	snprintf(ibuf, sizeof(ibuf), "erl backend collaborate %p", (void *)handle);
	write_logger(logger, ibuf);

	snprintf(ibuf, sizeof(ibuf), "  .. joining @%p and @%p", (void *)handle, pbh2);
	write_logger(logger, ibuf);

	return 0;
}

