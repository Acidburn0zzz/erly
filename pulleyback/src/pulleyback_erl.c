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


#include <pulleyback.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static const char logger[] = "steamworks.pulleyback.erl";
static int num_instances = 0;

typedef struct {
	int instancenumber;
	int varc;
} handle_t;

void *pulleyback_open(int argc, char **argv, int varc)
{
	char ibuf[64];

	write_logger(logger, "erl backend opened.");
	snprintf(ibuf, sizeof(ibuf), " .. %d parameters, expect %d vars later", argc, varc);
	write_logger(logger, ibuf);
	// snprintf(ibuf, sizeof(ibuf), " .. %d variables", varc);

	for (unsigned int i=0; i<argc; i++)
	{
		snprintf(ibuf, sizeof(ibuf), "  .. parm %d=%s", i, argv[i]);
		write_logger(logger, ibuf);
	}

	handle_t* handle = malloc(sizeof(handle_t));
	if (handle == NULL)
	{
		snprintf(ibuf, sizeof(ibuf), "Could not allocate handle %ld (#%d)", sizeof(handle_t), num_instances+1);
		write_logger(logger, ibuf);
		/* assume malloc() has set errno */
		return NULL;
	}
	else
	{
		handle->instancenumber = ++num_instances;
		handle->varc = varc;

		snprintf(ibuf, sizeof(ibuf), "erl backend handle %p (#%d)", (void *)handle, handle->instancenumber);
		write_logger(logger, ibuf);
		return handle;
	}
}

void pulleyback_close(void *pbh)
{
	char ibuf[64];
	handle_t* handle = pbh;

	snprintf(ibuf, sizeof(ibuf), "erl backend close %p", (void *)handle);
	write_logger(logger, ibuf);
	snprintf(ibuf, sizeof(ibuf), " .. instance %d", handle->instancenumber);
	write_logger(logger, ibuf);

	free(pbh);
}

void dump_der(int argc, der_t der)
{
	char ibuf[64];

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
		dump_der(i, *p);
		p++;
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

