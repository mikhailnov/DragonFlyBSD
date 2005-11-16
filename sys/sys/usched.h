/*
 * SYS/USCHED.H
 *
 *	Userland scheduler API
 * 
 * $DragonFly: src/sys/sys/usched.h,v 1.7 2005/11/16 02:24:33 dillon Exp $
 */

#ifndef _SYS_USCHED_H_
#define _SYS_USCHED_H_

#if defined(_KERNEL) || defined(_KERNEL_STRUCTURES)

#ifndef _SYS_QUEUE_H_
#include <sys/queue.h>
#endif

struct proc;
struct globaldata;

struct usched {
    TAILQ_ENTRY(usched) entry;
    const char *name;
    const char *desc;
    void (*usched_register)(void);
    void (*usched_unregister)(void);
    void (*acquire_curproc)(struct lwp *);
    void (*release_curproc)(struct lwp *);
    void (*select_curproc)(struct globaldata *);
    void (*setrunqueue)(struct lwp *);
    void (*remrunqueue)(struct lwp *);
    void (*schedulerclock)(struct lwp *, sysclock_t, sysclock_t);
    void (*recalculate)(struct lwp *);
    void (*resetpriority)(struct lwp *);
    void (*heuristic_forking)(struct lwp *, struct lwp *);
    void (*heuristic_exiting)(struct lwp *, struct lwp *);
    void (*setcpumask)(struct usched *, cpumask_t);
};

union usched_data {
    /*
     * BSD4 scheduler. 
     */
    struct {
	short	priority;	/* lower is better */
	char	interactive;	/* (currently not used) */
	char	rqindex;
	int	origcpu;
	int	estcpu;		/* dynamic priority modification */
    } bsd4;

    int		pad[4];		/* PAD for future expansion */
};

/*
 * Flags for usched_ctl()
 */
#define        USCH_ADD        0x00000001
#define        USCH_REM        0x00000010

#endif	/* _KERNEL || _KERNEL_STRUCTURES */

/*
 * Kernel variables and procedures, or user system calls.
 */
#ifdef _KERNEL

extern struct usched	usched_bsd4;

int usched_ctl(struct usched *, int);
struct usched *usched_init(void);

#else

int usched_set(const char *, int);

#endif

#endif

