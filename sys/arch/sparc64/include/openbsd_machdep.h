#ifndef _OPENBSD_MACHDEP_H
#define _OPENBSD_MACHDEP_H

/* sys/sparc64/include/exec.h */
#define OPENBSD___LDPGSZ	8192

/*
 * signal support
 */
 
/*struct openbsd_sigframe {
	int		sf_signo;
	int		sf_code;
	siginfo_t	*sf_sip;
	struct		sigcontext sf_sc;
	siginfo_t	sf_si;
};*/

struct  openbsd_sigcontext {
	int		sc_onstack;
	int		__sc_mask13;
	long		sc_sp;
	long		sc_pc;
	long		sc_npc;
	long		sc_tstate;
	long		sc_g1;
	long		sc_o0;
	int		sc_mask;
};

#ifdef _KERNEL
void openbsd_sendsig(sig_t, int, int, u_long, int, union sigval);
#endif

#endif /* _OPENBSD_MACHDEP_H */
