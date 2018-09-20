/*-
 * Copyright (c) 2004 The DragonFly Project.  All rights reserved.
 *
 * Copyright (c) 2002 Thomas Moestl <tmm@FreeBSD.org>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
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
 * $FreeBSD: src/sys/sys/endian.h,v 1.2.2.1 2002/09/09 05:45:04 imp Exp $
 */

#ifndef _SYS_ENDIAN_H_
#define _SYS_ENDIAN_H_

#ifndef _SYS_TYPES_H_
#include <sys/types.h>
#endif
#include <machine/endian.h>

/*
 * General byte order swapping functions.
 */
#define	bswap16(x)	__bswap16(x)
#define	bswap32(x)	__bswap32(x)
#define	bswap64(x)	__bswap64(x)

/*
 * Host to big endian, host to little endian, big endian to host, and little
 * endian to host byte order functions as detailed in byteorder(9).
 */
#if _BYTE_ORDER == _LITTLE_ENDIAN
#define	htobe16(x)	bswap16((x))
#define	htobe32(x)	bswap32((x))
#define	htobe64(x)	bswap64((x))
#define	htole16(x)	((__uint16_t)(x))
#define	htole32(x)	((__uint32_t)(x))
#define	htole64(x)	((__uint64_t)(x))

#define	be16toh(x)	bswap16((x))
#define	be32toh(x)	bswap32((x))
#define	be64toh(x)	bswap64((x))
#define	le16toh(x)	((__uint16_t)(x))
#define	le32toh(x)	((__uint32_t)(x))
#define	le64toh(x)	((__uint64_t)(x))
#else /* _BYTE_ORDER != _LITTLE_ENDIAN */
#define	htobe16(x)	((__uint16_t)(x))
#define	htobe32(x)	((__uint32_t)(x))
#define	htobe64(x)	((__uint64_t)(x))
#define	htole16(x)	bswap16((x))
#define	htole32(x)	bswap32((x))
#define	htole64(x)	bswap64((x))

#define	be16toh(x)	((__uint16_t)(x))
#define	be32toh(x)	((__uint32_t)(x))
#define	be64toh(x)	((__uint64_t)(x))
#define	le16toh(x)	bswap16((x))
#define	le32toh(x)	bswap32((x))
#define	le64toh(x)	bswap64((x))
#endif /* _BYTE_ORDER == _LITTLE_ENDIAN */

/* Alignment-agnostic encode/decode bytestream to/from little/big endian. */

static __inline __uint16_t
be16dec(const void *pp)
{
	const __uint8_t *p = (const __uint8_t *)pp;

	return ((p[0] << 8) | p[1]);
}

static __inline __uint32_t
be32dec(const void *pp)
{
	const __uint8_t *p = (const __uint8_t *)pp;

	return ((p[0] << 24) | (p[1] << 16) | (p[2] << 8) | p[3]);
}

static __inline __uint64_t
be64dec(const void *pp)
{
	const __uint8_t *p = (const __uint8_t *)pp;

	return (((__uint64_t)be32dec(p) << 32) | be32dec(p + 4));
}

static __inline __uint16_t
le16dec(const void *pp)
{
	const __uint8_t *p = (const __uint8_t *)pp;

	return ((p[1] << 8) | p[0]);
}

static __inline __uint32_t
le32dec(const void *pp)
{
	const __uint8_t *p = (const __uint8_t *)pp;

	return ((p[3] << 24) | (p[2] << 16) | (p[1] << 8) | p[0]);
}

static __inline __uint64_t
le64dec(const void *pp)
{
	const __uint8_t *p = (const __uint8_t *)pp;

	return (((__uint64_t)le32dec(p + 4) << 32) | le32dec(p));
}

static __inline void
be16enc(void *pp, __uint16_t u)
{
	__uint8_t *p = (__uint8_t *)pp;

	p[0] = (u >> 8) & 0xff;
	p[1] = u & 0xff;
}

static __inline void
be32enc(void *pp, __uint32_t u)
{
	__uint8_t *p = (__uint8_t *)pp;

	p[0] = (u >> 24) & 0xff;
	p[1] = (u >> 16) & 0xff;
	p[2] = (u >> 8) & 0xff;
	p[3] = u & 0xff;
}

static __inline void
be64enc(void *pp, __uint64_t u)
{
	__uint8_t *p = (__uint8_t *)pp;

	be32enc(p, u >> 32);
	be32enc(p + 4, u & 0xffffffff);
}

static __inline void
le16enc(void *pp, __uint16_t u)
{
	__uint8_t *p = (__uint8_t *)pp;

	p[0] = u & 0xff;
	p[1] = (u >> 8) & 0xff;
}

static __inline void
le32enc(void *pp, __uint32_t u)
{
	__uint8_t *p = (__uint8_t *)pp;

	p[0] = u & 0xff;
	p[1] = (u >> 8) & 0xff;
	p[2] = (u >> 16) & 0xff;
	p[3] = (u >> 24) & 0xff;
}

static __inline void
le64enc(void *pp, __uint64_t u)
{
	__uint8_t *p = (__uint8_t *)pp;

	le32enc(p, u & 0xffffffff);
	le32enc(p + 4, u >> 32);
}

#endif	/* _SYS_ENDIAN_H_ */
