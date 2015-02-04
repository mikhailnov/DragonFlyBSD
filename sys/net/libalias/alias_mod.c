/*-
 * Copyright (c) 2005 Paolo Pisati <piso@FreeBSD.org>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *	notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *	notice, this list of conditions and the following disclaimer in the
 *	documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 */
#include <sys/cdefs.h>
__FBSDID("$FreeBSD: src/sys/netinet/libalias/alias_mod.c,v 1.3.6.1 2008/11/25 02:59:29 kensmith Exp $");

#ifdef _KERNEL
#include <sys/libkern.h>
#include <sys/param.h>
#include <sys/lock.h>
#include <sys/idr.h>
#include <sys/queue.h>
#else
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <errno.h>
#endif

#include <netinet/in_systm.h>
#include <netinet/in.h>
#include <netinet/ip.h>


#include "alias_local.h"
#include "alias_mod.h"

/* Protocol and userland module handlers chains. */
LIST_HEAD(handler_chain, proto_handler) handler_chain = LIST_HEAD_INITIALIZER(foo);

SLIST_HEAD(dll_chain, dll) dll_chain = SLIST_HEAD_INITIALIZER(foo);


#define	LIBALIAS_RWLOCK_INIT() ;
#define	LIBALIAS_RWLOCK_DESTROY()	;
#define	LIBALIAS_WLOCK_ASSERT()	;
#define	LIBALIAS_RLOCK() ;
#define	LIBALIAS_RUNLOCK() ;
#define	LIBALIAS_WLOCK() ;
#define	LIBALIAS_WUNLOCK() ;
#define _handler_chain_init() ;
#define _handler_chain_destroy() ;

void
handler_chain_init(void)
{
	_handler_chain_init();
}

void
handler_chain_destroy(void)
{
	_handler_chain_destroy();
}

static int
_attach_handler(struct proto_handler *p)
{
	struct proto_handler *b = NULL;

	LIBALIAS_WLOCK_ASSERT();
	LIST_FOREACH(b, &handler_chain, entries) {
		if ((b->pri == p->pri) &&
			(b->dir == p->dir) &&
			(b->proto == p->proto))
			return (EEXIST); /* Priority conflict. */
		if (b->pri > p->pri) {
			LIST_INSERT_BEFORE(b, p, entries);
			return (0);
		}
	}
	/* End of list or found right position, inserts here. */
	if (b)
		LIST_INSERT_AFTER(b, p, entries);
	else
		LIST_INSERT_HEAD(&handler_chain, p, entries);
	return (0);
}

static int
_detach_handler(struct proto_handler *p)
{
	struct proto_handler *b, *b_tmp;;

	LIBALIAS_WLOCK_ASSERT();

	LIST_FOREACH_MUTABLE(b, &handler_chain, entries, b_tmp) {
		if (b == p) {
			LIST_REMOVE(b, entries);
			return (0);
		}
	}


	return (ENOENT); /* Handler not found. */
}

int
LibAliasAttachHandlers(struct proto_handler *_p)
{
	int i, error = -1;

	LIBALIAS_WLOCK();
	for (i=0; 1; i++) {
		if (*((int *)&_p[i]) == EOH)
			break;
		error = _attach_handler(&_p[i]);
		if (error != 0)
			break;
	}
	LIBALIAS_WUNLOCK();
	return (error);
}

int
LibAliasDetachHandlers(struct proto_handler *_p)
{
	int i, error = -1;

	LIBALIAS_WLOCK();
	for (i=0; 1; i++) {
		if (*((int *)&_p[i]) == EOH)
			break;
		error = _detach_handler(&_p[i]);
		if (error != 0)
			break;
	}
	LIBALIAS_WUNLOCK();
	return (error);
}

int
detach_handler(struct proto_handler *_p)
{
	int error = -1;

	LIBALIAS_WLOCK();
	error = _detach_handler(_p);
	LIBALIAS_WUNLOCK();
	return (error);
}

int
find_handler(int8_t dir, int8_t proto, struct libalias *la, struct ip *pip,
		 struct alias_data *ad)
{
	struct proto_handler *p;
	int error = ENOENT;

	LIBALIAS_RLOCK();

	LIST_FOREACH(p, &handler_chain, entries) {
		if ((p->dir & dir) && (p->proto & proto))
			if (p->fingerprint(la, pip, ad) == 0) {
				error = p->protohandler(la, pip, ad);
				break;
			}
	}
	LIBALIAS_RUNLOCK();
	return (error);
}

struct proto_handler *
first_handler(void)
{

	return (LIST_FIRST(&handler_chain));
}

/* Dll manipulation code - this code is not thread safe... */



struct dll *
walk_dll_chain(void)
{
	struct dll *t;

	t = SLIST_FIRST(&dll_chain);
	if (t == NULL)
		return (NULL);
	SLIST_REMOVE_HEAD(&dll_chain, next);
	return (t);
}
