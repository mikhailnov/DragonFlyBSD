# $FreeBSD: src/secure/usr.sbin/sshd/Makefile,v 1.5.2.9 2003/02/03 17:31:12 des Exp $

PROG=	sshd
SRCS=	sshd.c auth-rhosts.c auth-passwd.c \
	audit.c audit-bsm.c platform.c \
	sshpty.c sshlogin.c servconf.c serverloop.c \
	auth.c auth2.c auth-options.c session.c \
	auth2-chall.c groupaccess.c \
	auth-skey.c auth-bsdauth.c auth2-hostbased.c auth2-kbdint.c \
	auth2-none.c auth2-passwd.c auth2-pubkey.c \
	monitor.c monitor_wrap.c auth-krb5.c \
	auth2-gss.c gss-serv.c gss-serv-krb5.c \
	loginrec.c auth-pam.c auth-shadow.c auth-sia.c md5crypt.c \
	sftp-server.c sftp-common.c \
	sandbox-rlimit.c

WARNS?=	3

# Portability layer
SRCS+=	fmt_scaled.c
MAN=	sshd.8 sshd_config.5

# Passwd routine
SRCS+=	auth-passwd-freebsd.c
CFLAGS+=-DCUSTOM_SYS_AUTH_PASSWD

# Flags for PAM
CFLAGS+=-DUSE_PAM -DHAVE_SECURITY_PAM_APPL_H \
	-DHAVE_PAM_GETENVLIST -DHAVE_PAM_PUTENV

LDADD+=	-lopie
DPADD+=	${LIBOPIE}

LDADD+=	-lcrypt -lwrap ${MINUSLPAM}
DPADD+=	${LIBCRYPT} ${LIBWRAP} ${LIBPAM}

.include "../../Makefile.ssh.common"
.include <bsd.prog.mk>
