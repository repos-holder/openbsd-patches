/*
 * System call names.
 *
 * DO NOT EDIT-- this file is automatically generated.
 *
 * created from
 *	ABSD: syscalls.master,v 1.7 2010/02/15 09:42:19 mickey Exp 
 */

const char *syscallnames[] = {
	"syscall",			/* 0 = syscall */
	"sched_yield",			/* 1 = sched_yield */
	"__sysctl",			/* 2 = __sysctl */
	"exit",			/* 3 = exit */
	"fork",			/* 4 = fork */
	"vfork",			/* 5 = vfork */
	"rfork",			/* 6 = rfork */
	"wait4",			/* 7 = wait4 */
	"fchdir",			/* 8 = fchdir */
	"__getcwd",			/* 9 = __getcwd */
	"closefrom",			/* 10 = closefrom */
	"#11 (unimplemented { int sys_pstat ( int which , id_t who , struct kinfo_proc * kip , size_t * len ) ; })",		/* 11 = unimplemented { int sys_pstat ( int which , id_t who , struct kinfo_proc * kip , size_t * len ) ; } */
	"mount",			/* 12 = mount */
	"unmount",			/* 13 = unmount */
	"setuid",			/* 14 = setuid */
	"getpgid",			/* 15 = getpgid */
	"mmap",			/* 16 = mmap */
	"munmap",			/* 17 = munmap */
	"mprotect",			/* 18 = mprotect */
	"madvise",			/* 19 = madvise */
	"mincore",			/* 20 = mincore */
	"mlock",			/* 21 = mlock */
	"munlock",			/* 22 = munlock */
	"mlockall",			/* 23 = mlockall */
	"munlockall",			/* 24 = munlockall */
	"minherit",			/* 25 = minherit */
	"msync",			/* 26 = msync */
	"mquery",			/* 27 = mquery */
	"chroot",			/* 28 = chroot */
	"umask",			/* 29 = umask */
	"execve",			/* 30 = execve */
	"kill",			/* 31 = kill */
	"profil",			/* 32 = profil */
	"sigaction",			/* 33 = sigaction */
	"sigprocmask",			/* 34 = sigprocmask */
	"nanosleep",			/* 35 = nanosleep */
	"poll",			/* 36 = poll */
	"select",			/* 37 = select */
	"setlogin",			/* 38 = setlogin */
	"sigaltstack",			/* 39 = sigaltstack */
	"ioctl",			/* 40 = ioctl */
	"reboot",			/* 41 = reboot */
	"revoke",			/* 42 = revoke */
	"setpriority",			/* 43 = setpriority */
	"getpriority",			/* 44 = getpriority */
	"sigreturn",			/* 45 = sigreturn */
	"adjfreq",			/* 46 = adjfreq */
	"adjtime",			/* 47 = adjtime */
	"gettimeofday",			/* 48 = gettimeofday */
	"sigsuspend",			/* 49 = sigsuspend */
	"setgroups",			/* 50 = setgroups */
	"setpgid",			/* 51 = setpgid */
	"setitimer",			/* 52 = setitimer */
	"getitimer",			/* 53 = getitimer */
	"settimeofday",			/* 54 = settimeofday */
	"setreuid",			/* 55 = setreuid */
	"setregid",			/* 56 = setregid */
	"sendto",			/* 57 = sendto */
	"setsid",			/* 58 = setsid */
	"quotactl",			/* 59 = quotactl */
	"#60 (unimplemented aio_open)",		/* 60 = unimplemented aio_open */
	"#61 (unimplemented aio_read)",		/* 61 = unimplemented aio_read */
	"#62 (unimplemented aio_write)",		/* 62 = unimplemented aio_write */
	"#63 (unimplemented aio_fsync)",		/* 63 = unimplemented aio_fsync */
	"#64 (unimplemented aio_stat)",		/* 64 = unimplemented aio_stat */
	"#65 (unimplemented aio_wstat)",		/* 65 = unimplemented aio_wstat */
	"#66 (unimplemented aio_close)",		/* 66 = unimplemented aio_close */
	"#67 (unimplemented aio_return)",		/* 67 = unimplemented aio_return */
	"#68 (unimplemented aio_cancel)",		/* 68 = unimplemented aio_cancel */
	"#69 (unimplemented)",		/* 69 = unimplemented */
	"sync",			/* 70 = sync */
	"sysarch",			/* 71 = sysarch */
	"setgid",			/* 72 = setgid */
	"setegid",			/* 73 = setegid */
	"seteuid",			/* 74 = seteuid */
	"setresuid",			/* 75 = setresuid */
	"setresgid",			/* 76 = setresgid */
	"getfsstat",			/* 77 = getfsstat */
	"rmdir",			/* 78 = rmdir */
	"rename",			/* 79 = rename */
	"unlink",			/* 80 = unlink */
	"#81 (unimplemented sys_undelete)",		/* 81 = unimplemented sys_undelete */
	"swapctl",			/* 82 = swapctl */
	"setrlimit",			/* 83 = setrlimit */
	"getdirentries",			/* 84 = getdirentries */
	"clock_gettime",			/* 85 = clock_gettime */
	"clock_settime",			/* 86 = clock_settime */
	"clock_getres",			/* 87 = clock_getres */
	"#88 (unimplemented timer_create)",		/* 88 = unimplemented timer_create */
	"#89 (unimplemented timer_delete)",		/* 89 = unimplemented timer_delete */
	"#90 (unimplemented timer_settime)",		/* 90 = unimplemented timer_settime */
	"#91 (unimplemented timer_gettime)",		/* 91 = unimplemented timer_gettime */
	"#92 (unimplemented timer_getoverrun)",		/* 92 = unimplemented timer_getoverrun */
	"#93 (unimplemented)",		/* 93 = unimplemented */
	"#94 (unimplemented)",		/* 94 = unimplemented */
	"#95 (unimplemented)",		/* 95 = unimplemented */
	"#96 (unimplemented)",		/* 96 = unimplemented */
	"#97 (unimplemented)",		/* 97 = unimplemented */
	"#98 (unimplemented)",		/* 98 = unimplemented */
	"#99 (unimplemented)",		/* 99 = unimplemented */
	"#100 (unimplemented)",		/* 100 = unimplemented */
	"#101 (unimplemented)",		/* 101 = unimplemented */
	"#102 (unimplemented)",		/* 102 = unimplemented */
	"#103 (unimplemented)",		/* 103 = unimplemented */
	"#104 (unimplemented)",		/* 104 = unimplemented */
	"#105 (unimplemented)",		/* 105 = unimplemented */
	"#106 (unimplemented)",		/* 106 = unimplemented */
	"#107 (unimplemented)",		/* 107 = unimplemented */
	"#108 (unimplemented)",		/* 108 = unimplemented */
	"#109 (unimplemented)",		/* 109 = unimplemented */
	"#110 (unimplemented)",		/* 110 = unimplemented */
	"#111 (unimplemented)",		/* 111 = unimplemented */
	"#112 (unimplemented)",		/* 112 = unimplemented */
	"#113 (unimplemented)",		/* 113 = unimplemented */
	"#114 (unimplemented)",		/* 114 = unimplemented */
	"#115 (unimplemented)",		/* 115 = unimplemented */
	"#116 (unimplemented)",		/* 116 = unimplemented */
	"#117 (unimplemented)",		/* 117 = unimplemented */
	"#118 (unimplemented)",		/* 118 = unimplemented */
	"#119 (unimplemented)",		/* 119 = unimplemented */
	"#120 (unimplemented)",		/* 120 = unimplemented */
	"#121 (unimplemented)",		/* 121 = unimplemented */
	"#122 (unimplemented)",		/* 122 = unimplemented */
	"#123 (unimplemented)",		/* 123 = unimplemented */
	"#124 (unimplemented)",		/* 124 = unimplemented */
	"#125 (unimplemented)",		/* 125 = unimplemented */
	"#126 (unimplemented)",		/* 126 = unimplemented */
	"#127 (unimplemented)",		/* 127 = unimplemented */
	"#128 (unimplemented)",		/* 128 = unimplemented */
	"#129 (unimplemented)",		/* 129 = unimplemented */
	"#130 (unimplemented)",		/* 130 = unimplemented */
	"#131 (unimplemented)",		/* 131 = unimplemented */
	"#132 (unimplemented)",		/* 132 = unimplemented */
	"#133 (unimplemented)",		/* 133 = unimplemented */
	"#134 (unimplemented)",		/* 134 = unimplemented */
	"#135 (unimplemented)",		/* 135 = unimplemented */
	"#136 (unimplemented)",		/* 136 = unimplemented */
	"#137 (unimplemented)",		/* 137 = unimplemented */
	"#138 (unimplemented)",		/* 138 = unimplemented */
	"#139 (unimplemented)",		/* 139 = unimplemented */
	"#140 (unimplemented sys_extattrctl)",		/* 140 = unimplemented sys_extattrctl */
	"#141 (unimplemented sys_extattr_set_file)",		/* 141 = unimplemented sys_extattr_set_file */
	"#142 (unimplemented sys_extattr_get_file)",		/* 142 = unimplemented sys_extattr_get_file */
	"#143 (unimplemented sys_extattr_delete_file)",		/* 143 = unimplemented sys_extattr_delete_file */
	"#144 (unimplemented sys_extattr_set_fd)",		/* 144 = unimplemented sys_extattr_set_fd */
	"#145 (unimplemented sys_extattr_get_fd)",		/* 145 = unimplemented sys_extattr_get_fd */
	"#146 (unimplemented sys_extattr_delete_fd)",		/* 146 = unimplemented sys_extattr_delete_fd */
#ifdef XFS
	"xfspioctl",			/* 147 = xfspioctl */
#else
	"#147 (unimplemented xfspioctl)",		/* 147 = unimplemented xfspioctl */
#endif
#if defined(NFSCLIENT) || defined(NFSSERVER)
	"nfssvc",			/* 148 = nfssvc */
#else
	"#148 (unimplemented nfssvc)",		/* 148 = unimplemented nfssvc */
#endif
#ifdef ACCOUNTING
	"acct",			/* 149 = acct */
#else
	"#149 (unimplemented acct)",		/* 149 = unimplemented acct */
#endif
#ifdef PTRACE
	"ptrace",			/* 150 = ptrace */
#else
	"#150 (unimplemented ptrace)",		/* 150 = unimplemented ptrace */
#endif
#ifdef KTRACE
	"ktrace",			/* 151 = ktrace */
#else
	"#151 (unimplemented ktrace)",		/* 151 = unimplemented ktrace */
#endif
#ifdef SYSVSEM
	"semget",			/* 152 = semget */
	"semop",			/* 153 = semop */
	"__semctl",			/* 154 = __semctl */
#else
	"#152 (unimplemented semget)",		/* 152 = unimplemented semget */
	"#153 (unimplemented semop)",		/* 153 = unimplemented semop */
	"#154 (unimplemented ___semctl)",		/* 154 = unimplemented ___semctl */
#endif
#ifdef SYSVSHM
	"shmget",			/* 155 = shmget */
	"shmctl",			/* 156 = shmctl */
	"shmat",			/* 157 = shmat */
	"shmdt",			/* 158 = shmdt */
#else
	"#155 (unimplemented shmget)",		/* 155 = unimplemented shmget */
	"#156 (unimplemented shmctl)",		/* 156 = unimplemented shmctl */
	"#157 (unimplemented shmat)",		/* 157 = unimplemented shmat */
	"#158 (unimplemented shmdt)",		/* 158 = unimplemented shmdt */
#endif
#ifdef SYSVMSG
	"msgget",			/* 159 = msgget */
	"msgsnd",			/* 160 = msgsnd */
	"msgrcv",			/* 161 = msgrcv */
	"msgctl",			/* 162 = msgctl */
#else
	"#159 (unimplemented msgget)",		/* 159 = unimplemented msgget */
	"#160 (unimplemented msgsnd)",		/* 160 = unimplemented msgsnd */
	"#161 (unimplemented msgrcv)",		/* 161 = unimplemented msgrcv */
	"#162 (unimplemented msgctl)",		/* 162 = unimplemented msgctl */
#endif
#ifdef RTHREADS
	"getthrid",			/* 163 = getthrid */
	"thrsleep",			/* 164 = thrsleep */
	"thrwakeup",			/* 165 = thrwakeup */
	"threxit",			/* 166 = threxit */
	"thrsigdivert",			/* 167 = thrsigdivert */
#else
	"#163 (unimplemented getthrid)",		/* 163 = unimplemented getthrid */
	"#164 (unimplemented thrsleep)",		/* 164 = unimplemented thrsleep */
	"#165 (unimplemented thrwakeup)",		/* 165 = unimplemented thrwakeup */
	"#166 (unimplemented threxit)",		/* 166 = unimplemented threxit */
	"#167 (unimplemented thrsigdivert)",		/* 167 = unimplemented thrsigdivert */
#endif
	"#168 (unimplemented)",		/* 168 = unimplemented */
	"#169 (unimplemented)",		/* 169 = unimplemented */
#ifdef LKM
	"lkmnosys",			/* 170 = lkmnosys */
	"lkmnosys",			/* 171 = lkmnosys */
	"lkmnosys",			/* 172 = lkmnosys */
	"lkmnosys",			/* 173 = lkmnosys */
	"lkmnosys",			/* 174 = lkmnosys */
	"lkmnosys",			/* 175 = lkmnosys */
	"lkmnosys",			/* 176 = lkmnosys */
	"lkmnosys",			/* 177 = lkmnosys */
	"lkmnosys",			/* 178 = lkmnosys */
	"lkmnosys",			/* 179 = lkmnosys */
#else	/* !LKM */
	"#170 (unimplemented)",		/* 170 = unimplemented */
	"#171 (unimplemented)",		/* 171 = unimplemented */
	"#172 (unimplemented)",		/* 172 = unimplemented */
	"#173 (unimplemented)",		/* 173 = unimplemented */
	"#174 (unimplemented)",		/* 174 = unimplemented */
	"#175 (unimplemented)",		/* 175 = unimplemented */
	"#176 (unimplemented)",		/* 176 = unimplemented */
	"#177 (unimplemented)",		/* 177 = unimplemented */
	"#178 (unimplemented)",		/* 178 = unimplemented */
	"#179 (unimplemented)",		/* 179 = unimplemented */
#endif	/* !LKM */
	"#180 (unimplemented)",		/* 180 = unimplemented */
	"#181 (unimplemented)",		/* 181 = unimplemented */
	"#182 (unimplemented)",		/* 182 = unimplemented */
	"#183 (unimplemented)",		/* 183 = unimplemented */
	"#184 (unimplemented)",		/* 184 = unimplemented */
	"#185 (unimplemented)",		/* 185 = unimplemented */
	"#186 (unimplemented)",		/* 186 = unimplemented */
	"#187 (unimplemented)",		/* 187 = unimplemented */
	"#188 (unimplemented)",		/* 188 = unimplemented */
	"#189 (unimplemented)",		/* 189 = unimplemented */
	"#190 (unimplemented)",		/* 190 = unimplemented */
	"#191 (unimplemented)",		/* 191 = unimplemented */
	"#192 (unimplemented)",		/* 192 = unimplemented */
	"#193 (unimplemented)",		/* 193 = unimplemented */
	"#194 (unimplemented)",		/* 194 = unimplemented */
	"#195 (unimplemented)",		/* 195 = unimplemented */
	"#196 (unimplemented)",		/* 196 = unimplemented */
	"#197 (unimplemented)",		/* 197 = unimplemented */
	"__syscall",			/* 198 = __syscall */
	"getpeereid",			/* 199 = getpeereid */
	"setsockopt",			/* 200 = setsockopt */
	"getsockopt",			/* 201 = getsockopt */
	"getpeername",			/* 202 = getpeername */
	"getsockname",			/* 203 = getsockname */
	"access",			/* 204 = access */
	"accept",			/* 205 = accept */
	"listen",			/* 206 = listen */
	"connect",			/* 207 = connect */
	"bind",			/* 208 = bind */
	"fstatfs",			/* 209 = fstatfs */
	"kqueue",			/* 210 = kqueue */
	"kevent",			/* 211 = kevent */
	"pipe",			/* 212 = pipe */
	"socketpair",			/* 213 = socketpair */
	"socket",			/* 214 = socket */
	"link",			/* 215 = link */
	"symlink",			/* 216 = symlink */
	"readlink",			/* 217 = readlink */
	"mkfifo",			/* 218 = mkfifo */
	"mkdir",			/* 219 = mkdir */
	"mknod",			/* 220 = mknod */
	"fhstatfs",			/* 221 = fhstatfs */
	"getfh",			/* 222 = getfh */
	"fhopen",			/* 223 = fhopen */
	"fhstat",			/* 224 = fhstat */
	"recvmsg",			/* 225 = recvmsg */
	"sendmsg",			/* 226 = sendmsg */
	"recvfrom",			/* 227 = recvfrom */
	"open",			/* 228 = open */
	"read",			/* 229 = read */
	"write",			/* 230 = write */
	"readv",			/* 231 = readv */
	"writev",			/* 232 = writev */
	"pread",			/* 233 = pread */
	"pwrite",			/* 234 = pwrite */
	"preadv",			/* 235 = preadv */
	"pwritev",			/* 236 = pwritev */
	"fsync",			/* 237 = fsync */
	"fstat",			/* 238 = fstat */
	"ftruncate",			/* 239 = ftruncate */
	"futimes",			/* 240 = futimes */
	"fchmod",			/* 241 = fchmod */
	"fchown",			/* 242 = fchown */
	"fchflags",			/* 243 = fchflags */
	"close",			/* 244 = close */
	"chdir",			/* 245 = chdir */
	"pathconf",			/* 246 = pathconf */
	"stat",			/* 247 = stat */
	"lstat",			/* 248 = lstat */
	"statfs",			/* 249 = statfs */
	"truncate",			/* 250 = truncate */
	"utimes",			/* 251 = utimes */
	"chmod",			/* 252 = chmod */
	"chown",			/* 253 = chown */
	"lchown",			/* 254 = lchown */
	"chflags",			/* 255 = chflags */
	"fcntl",			/* 256 = fcntl */
	"dup",			/* 257 = dup */
	"dup2",			/* 258 = dup2 */
	"flock",			/* 259 = flock */
	"fpathconf",			/* 260 = fpathconf */
	"lseek",			/* 261 = lseek */
	"shutdown",			/* 262 = shutdown */
	"getpid",			/* 263 = getpid */
	"getuid",			/* 264 = getuid */
	"geteuid",			/* 265 = geteuid */
	"getppid",			/* 266 = getppid */
	"getegid",			/* 267 = getegid */
	"getgid",			/* 268 = getgid */
	"getlogin",			/* 269 = getlogin */
	"sigpending",			/* 270 = sigpending */
	"issetugid",			/* 271 = issetugid */
	"getgroups",			/* 272 = getgroups */
	"getpgrp",			/* 273 = getpgrp */
	"getrusage",			/* 274 = getrusage */
	"getsid",			/* 275 = getsid */
	"getrlimit",			/* 276 = getrlimit */
	"getresuid",			/* 277 = getresuid */
	"getresgid",			/* 278 = getresgid */
};
