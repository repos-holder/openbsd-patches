#!/bin/sh

set -e

# External settings passed as arguments
src_dir=$1
orig_dir=$2
patch_dir=$3
while [ $# -ne 5 ]; do
    shift
done
fetch_cmd=$*

# Bring missing imsg from portable version
${fetch_cmd} -o - http://opensmtpd.org/archives/opensmtpd-portable-latest.tar.gz | tar xzf -
mv opensmtpd/openbsd-compat/imsg* opensmtpd/openbsd-compat/sys-queue.h \
 ${src_dir}${patch_dir}/
rm -rf opensmtpd || ( echo "Warning! Alien files probably stayed in .${patch_dir}" && exit )

# Replace some crap to use OpenBSD compatibility functions
mv ${src_dir}${patch_dir}/smtp_session.c ${src_dir}${patch_dir}/smtp_session.c.orig && \
	echo "sed -e 's/OpenSMTPD/NekoSMTPd/' -e '/session_respond.*214.*bugs/{N; //d;g;}'" \
	"-e '/session_respond.*214-.*/d'" \
	"${src_dir}${patch_dir}/smtp_session.c.orig > ${src_dir}${patch_dir}/smtp_session.c" \
     > patch-smtp_session_c && sh ./patch-smtp_session_c && rm patch-smtp_session_c
cd ${orig_dir}

# Build fake lib with imsg stuff
cp lib/libutil.tmux/Makefile ${src_dir}${patch_dir}/Makefile.fakeutil && \
	echo ".include \"Makefile.fakeutil\"" >> ${src_dir}${patch_dir}/Makefile
patch -su < .${patch_dir}/patch-Makefile.smtpd ${src_dir}${patch_dir}/smtpd/Makefile
patch -su < .${patch_dir}/patch-Makefile.smtpctl ${src_dir}${patch_dir}/smtpctl/Makefile
cd ${src_dir}${patch_dir}
make -f Makefile.fakeutil clean 2>&1 >/dev/null

# Misc
echo "#define AI_FQDN AI_CANONNAME\n#define EAI_OVERFLOW -14\n" \
        "#define SS_LEN(x)(((struct sockaddr *)(x))->sa_family == AF_INET6 ? " \
        "sizeof(struct sockaddr_in6) : sizeof(struct sockaddr_in))\n" > includes.h 
echo "#define getdtablecount() 42" >> smtpd.h
echo "#include \"includes.h\"" >> asr/asr.h
mv queue_fsqueue.c queue_fsqueue.c.orig && \
   sed '/.*st_mtim.*/d' queue_fsqueue.c.orig > queue_fsqueue.c
