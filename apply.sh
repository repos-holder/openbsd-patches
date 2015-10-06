#!/bin/sh
# $gate: apply.sh,v 1.16 2011/10/09 22:17:09 dukzcry Exp $

echo "================================================"
echo " This script will apply custom modifications to"
echo " OpenBSD tree. Platforms of interest are sparc64"
echo " and macppc."
echo "================================================"
echo " Note for interactive mode."

orig_dir="`pwd`"
src_dir=/usr/src # dir name "src" is a requirement
sexec=/bin/sh
fetch_cmd="/usr/bin/ftp -V -m -k 0"
CVSROOT=anoncvs@anoncvs.openbsd.org:/cvs
OPENBSD_CVSROOT=anoncvs@anoncvs.estpak.ee:/OpenBSD

ask ()
{
	want=0
	while [ ${want} != "y" ] && [ ${want} != "n" ] &&
	      [ ${want} != "Y" ] && [ ${want} != "N" ];
	do
		echo $1 
		read want
	done
	if [ ${want} == "y" ] || [ ${want} == "Y" ]; then
		return 1
	fi
	return 0 
}
check ()
{
    if [ $1 -gt 1 ]; then
	return 1
    fi
    return 0
}
sed_func ()
{
    path=$1
    cp ${path} ${path}.orig && \
	sed "$2" \
	${path}.orig > ${path}
    check $?
    return $?
}
patch_func ()
{
    patch -CNu < .$1 $2
	ask "Ok for you? [Y/N]"
	if [ $? -eq 1 ]; then
	    patch -su < .$1 $2
	    if [ $? -gt 1 ]; then
		return 1
	    fi
	fi
    return 0
}
build ()
{
    make depend >/dev/null 2>&1
    make $@ >/dev/null
    check $?
    return $?
}
build_foreign ()
{
    make depend >/dev/null 2>&1
    make $@
    check $?
    return $?
}
setup ()
{
    sudo make $@ install >/dev/null
    check $?
    return $?
}
setup_foreign ()
{
    sudo make $@ install >/dev/null
    check $?
    return $?
}
cleanup ()
{
    make $@ clean >/dev/null 2>&1
}

echo " "
echo "------------"
echo "Main section"
echo "------------"
# BEGIN OF MAIN SECTION #
ask "Want it? [Y/N]"
if [ $? -eq 1 ]; then
#			#
ask "1. Check for old \"config\" utility and install new\n
    if required (fix few extern errors that brake building)? [Y/N]"
if [ $? -eq 1 ]; then
    diff_outcmd="diff /usr/sbin/config ./config"

    cd ${src_dir}/usr.sbin/config
    build
    diff_output=`diff /usr/sbin/config ./config`
    diff_status=`expr "$diff_output" : '.*'`
    
    if [ ${diff_status} -eq "49" ]; then
	strip ./config
	diff_output=`diff /usr/sbin/config ./config`
	diff_status=`expr "$diff_output" : '.*'`		
	if [ ${diff_status} -eq "49" ]; then
	    setup_foreign
	fi
    fi
    cleanup
    cd ${orig_dir}
fi

copy_custom_config="N"
while [ ${copy_custom_config} != "y" ] && [ ${copy_custom_config} != "n" ] &&
      [ ${copy_custom_config} != "Y" ] && [ ${copy_custom_config} != "N" ];
do
    echo " "
    echo "2. Set up custom kernel config file for Sun Fire V100 (EPILEPSY)? [Y/N]"
    read copy_custom_config
done
if [ ${copy_custom_config} == "y" ] || [ ${copy_custom_config} == "Y" ]; then
    cp ./sys/arch/sparc64/conf/EPILEPSY ${src_dir}/sys/arch/sparc64/conf
fi

ask " \n
    3. Hide external services versions like childs do it (ssh)? [Y/N]"
if [ $? -eq 1 ]; then
    patch_dir=/usr.bin/ssh
    sed_func ${src_dir}${patch_dir}/version.h \
	's/\(#define.*SSH_VERSION.*"OpenSSH\).*\("\)/\1\2/'
	cd ${src_dir}${patch_dir}
	build; setup; cleanup
	cd ${orig_dir}
fi

ask " \n
    4. Populate /emul/openbsd (required for successful bootstrap)? [Y/N]"
if [ $? -eq 1 ]; then
    emul_path=/emul/openbsd/usr
    sudo mkdir -p ${emul_path}/libexec/ && sudo cp /usr/libexec/ld.so \
        ${emul_path}/libexec/ 2>/dev/null
    ( test -d "${emul_path}/lib" || sudo mkdir ${emul_path}/lib/ ) && \
        #sudo cp /usr/lib/libc.so.* /usr/lib/lib[pr]thread.so.* ${emul_path}/lib/ 2>/dev/null
        sudo cp /usr/lib/*.so.* ${emul_path}/lib/ 2>/dev/null
fi

#                     #
fi
# END OF MAIN SECTION #

    echo " "
    echo "---------------------------------------------------"
    echo "In next part I'll show you changes, and you'll take"
    echo "decision should i patch one file. In another case you'll"
    echo "make it with your own hands." 
    echo " "
    echo "----------------"
    echo "Software section"
    echo "----------------"
ask "Want it? [Y/N]"
# BEGIN OF SOFTWARE SECTION #
if [ $? -eq 1 ]; then
#			    #
:<<obsoleted
while [ ${fix_elf} != "y" ] && [ ${fix_elf} != "n" ] &&
      [ ${fix_elf} != "Y" ] && [ ${fix_elf} != "N" ];
do
    echo "1. Fix libsa/loadfile.c error that brakes world compilation on 64-bit"
    echo "platforms that came from mistyping and fixed with ac3e7b6605 commit"
    echo "but came after b2e86f868e (on way to go to native bins) which is dead"
    echo "yet? [Y/N]"
    read fix_elf
done
if [ ${fix_elf} == "y" ] || [ ${fix_elf} == "Y" ]; then
    ok=0
    patch_dir=/sys/lib/libsa
    patch -CNu < .${patch_dir}/patch-loadfile.c ${src_dir}${patch_dir}/loadfile.c
    while [ ${ok} != "y" ] && [ ${ok} != "n" ] &&
          [ ${ok} != "Y" ] && [ ${ok} != "N" ];
    do
        echo "Ok for you? [Y/N]"
        read ok
    done
    if [ ${ok} == "y" ] || [ ${ok} == "Y" ]; then
        patch -su < .${patch_dir}/patch-loadfile.c ${src_dir}${patch_dir}/loadfile.c
        if [ $? -gt 1 ]; then
            exit
        fi
    fi
fi
obsoleted
ask "1. Fix current build breakers and include missed stuff? [Y/N]"
    ask_val=$?
if [ ${ask_val} -eq 1 ]; then
# cvs = opencvs
    sed_func ${src_dir}/usr.bin/Makefile \
        's/\( apply \)/\1aetouch icb cvs /'
# build configs with no "option XFS"
    echo "int sys_xfspioctl(struct proc *, void *, register_t *);" >> \
	${src_dir}/sys/xfs/xfs_locl.h 
#
# build configs with "option COMPAT_OPENBSD" && w/o "option NFSCLIENT" 
    patch_dir=/sys/compat/openbsd
	sed_func ${src_dir}${patch_dir}/Makefile \
	    's/\(openbsd_sysent.c\)/\1 openbsd_file.c/'
	sed_func ${src_dir}${patch_dir}/openbsd_file.c \
	    '/NFSCLIENT/d'
#
### rebuild breakers
	unamem=`uname -m`
	patch_dir=/usr.bin/pcc
	if [ $unamem == "sparc64" ]; then
		# gen files to be patched
		cd ${src_dir}${patch_dir}/ccom
		build
		cd ${orig_dir}
		# just disable -xtemps instead of adding new OPLTYPEs to the table
		patch_func ${patch_dir}/patch-pcc.2 ${src_dir}${patch_dir}/ccom/sparc64/cgram.c
		patch_func ${patch_dir}/patch-pcc.1 ${src_dir}${patch_dir}/sparc64/code.c
		echo "Ok. Please proceed with building pcc."
	elif [ $unamem == "amd64" ]; then
		# let it build
		echo "CPPFLAGS+=-Dmach_amd64" | sudo tee -a ${src_dir}${patch_dir}/Makefile.inc
	fi
#
###
### world breakers
# add pcc stubs
        patch_dir=/usr.bin/pcc
        tmpfile=`mktemp /tmp/pcc.XXXXXXXXXX`
        cat > ${tmpfile} << EOF
    
NODE *
builtin_cfa(const struct bitable *bt, NODE *a)
{
        uerror("missing builtin_cfa");
        return bcon(0);
}
NODE *
builtin_frame_address(const struct bitable *bt, NODE *a)
{
        uerror("missing builtin_frame_address");
        return bcon(0);
}
NODE *
builtin_return_address(const struct bitable *bt, NODE *a)
{      
        uerror("missing builtin_return_address");
        return bcon(0);
}
EOF
        archs="arm hppa mips pdp11 powerpc sparc64"
        for i in ${archs}; do
                cat ${tmpfile} >> ${src_dir}${patch_dir}/${i}/code.c
        done
#
    ask "Patch tree to make building possible with gnu tools? [Y/N]"
if [ $? -eq 1 ]; then
    patch_dir=/usr.bin/graph
    patch_func ${patch_dir}/patch-graph.c ${src_dir}${patch_dir}/graph.c
# don't interference with stdlib's decls
    patch_dir=/usr.bin/sudo
    sed_func ${src_dir}${patch_dir}/config.h \
	'/#define.*PUTENV_CONST.*/d'
    echo "#undef UNSETENV_VOID" >> ${src_dir}${patch_dir}/config.h
#
    patch_dir=/usr.sbin/icbd
    sed_func ${src_dir}${patch_dir}/Makefile 's/-Wall//'
# build shlibs
    cd ${src_dir}
    # bsd.lib.mk excises shlibs if NOPIC is set :\
    sed_func ./share/mk/bsd.own.mk 's/NOPIC=//'
    git revert 48183a955c
    git revert 930db82bba
    echo "major=51\nminor=0" > ./lib/libc/shlib_version
    cd ${orig_dir}
#
# don't install lintlibs for these
    patch_dir=/lib/libelf
    cp ${src_dir}${patch_dir}/Makefile ${src_dir}${patch_dir}/Makefile.orig && \
	sed '1i\
	    WANTLINT=no' ${src_dir}${patch_dir}/Makefile.orig > \
	    ${src_dir}${patch_dir}/Makefile
:<<obsoleted
    # for i386
    patch_dir=/lib/libm
    cp ${src_dir}${patch_dir}/Makefile ${src_dir}${patch_dir}/Makefile.orig && \
	sed '1i\
	    WANTLINT=no' ${src_dir}${patch_dir}/Makefile.orig > \
	    ${src_dir}${patch_dir}/Makefile
obsoleted
#
    echo "int tlen(NODE *);" >> ${src_dir}/usr.bin/pcc/ccom/pass1.h
fi
if [ `uname -m` == "sparc64" ]; then
	# don't let gnu tools use this unsupported optimization
        echo "CFLAGS+=-mno-app-regs" | sudo tee -a /etc/mk.conf
	echo "added to /etc/mk.conf"
fi
###
fi
:<<obsoleted
fix_ar="N"
while [ ${fix_ar} != "y" ] && [ ${fix_ar} != "n" ] &&
      [ ${fix_ar} != "Y" ] && [ ${fix_ar} != "N" ];
do
    #echo " "
    echo "2. Update ar(1) to the latest version where mickey@ fixed annoying"
    echo "'Corrupted nametab' error during ranlib which broke build of any software"
    echo "which built library archives" 
    echo "[Y/N]"
    read fix_ar 
done
if [ ${fix_ar} == "y" ] || [ ${fix_ar} == "Y" ]; then
    update_dir=/usr.bin/
    ar_dir=src${update_dir}ar
    cd ${src_dir}${update_dir}
    rm -rf ar/
    cd ${src_dir}/..
    cvs -qd ${CVSROOT} get -P ${ar_dir}
    cd ${ar_dir}
    build; setup; cleanup
    cd ${orig_dir}
fi
obsoleted
:<<junk
    ask " \n
    3. Add tmux(1) terminal multiplexer to source tree. Last checked revision that\n
    sucessfully passed building tests should be set up via revision.txt. Note that\n
    in current version of OpenBSD you should set EVENT_NOKQUEUE=1 to avoid tmux\n
    hangs in scroll mode which are caused by kqueue bugs.\n
    Note that git still doesn't support sparse fetching, so cloning all the tree\n
    may take a lot of time and space. [Y/N]"
if [ $? -eq 1 ]; then
    non_tree=/usr.bin/tmux/NON_TREE
    cd .${non_tree} && ${sexec} ./get.sh ${src_dir} ${orig_dir}
    cd ${src_dir} && build_foreign -f Makefile.tmux && setup_foreign -f Makefile.tmux
    cd ${orig_dir} 
fi
junk
    ask " \n
    4. Bring Nginx from OpenBSD tree and patch it's mail proxy module for ssl backends
    support? [Y/N]"
if [ $? -eq 1 ]; then
    patch_dir=/usr.sbin/nginx
    nginx_dir=src${patch_dir}
    cd ${src_dir}/..
    if [ ! -d ${nginx_dir} ]; then
	cvs -z3 -qd ${OPENBSD_CVSROOT} get -rOPENBSD_5_4 -P ${nginx_dir}
    fi
    cd ${orig_dir}
    sed_func ${src_dir}${patch_dir}/src/core/nginx.h \
	's#\(NGINX_VER.*"\)nginx/\("\).*NGINX_VERSION#\1WWW\2#'
    patch_func ${patch_dir}/patch-auto_lib_pcre_conf ${src_dir}${patch_dir}/auto/lib/pcre/conf
    cd ${src_dir}${patch_dir}
    patch -CNu < ${orig_dir}${patch_dir}/patch-nginx_mail_proxy_ssl_backends.diff
	ask "OK for you? [Y/N]"
    if [ $? -eq 1 ]; then
	patch -su < ${orig_dir}${patch_dir}/patch-nginx_mail_proxy_ssl_backends.diff
	if [ $? -gt 1 ]; then
	    exit
	fi
    fi
    cd ${orig_dir}
    makefile=${src_dir}${patch_dir}/Makefile.bsd-wrapper
    sed_expr="sed -e '/http_gzip_static_module/d' -e '/http_stub_status_module/d' -e '/without-mail_pop3/d' \
	    -e 's/without-mail_imap_module/with-mail --with-mail_ssl_module --with-http_perl_module --with-debug/' ${makefile}"
    eval ${sed_expr} | diff -up ${makefile} -
	ask "Ok for you? [Y/N]"
    if [ $? -eq 1 ]; then
	cp ${makefile} ${makefile}.orig && eval "${sed_expr}.orig" > ${makefile}
	cd ${src_dir}${patch_dir}
	build_foreign -f Makefile.bsd-wrapper; setup_foreign; cleanup
    fi
fi
:<<junk
	ask " \n
	5. OpenBSD team wrote OpenSMTPD and added it to OpenBSD 4.6 as sendmail\n
	replacement. Although it's still a WIP and is not enabled by default on current\n
	OpenBSD distribution. Let's bring this quite secure and GNU-free thing. [Y/N]"
if [ $? -eq 1 ]; then
	patch_dir=/usr.sbin/smtpd
	non_tree=${patch_dir}/NON_TREE
	smtpd_dir=src${patch_dir}
	cd ${src_dir}/..
	if [ -d ${smtpd_dir} ]; then
	    # -C - Overwrite locally modified files
	    cd ${smtpd_dir} && cvs -q up -Pd -C && cd asr && cvs -q up -Pd -C
	else
		cvs -qd ${OPENBSD_CVSROOT} get -P ${smtpd_dir}
		# Bring async resolver
		cd ${smtpd_dir} && cvs -qd ${OPENBSD_CVSROOT} get -d asr -P src/lib/libc/asr
	fi
	cd ${orig_dir}
	mv ${src_dir}${patch_dir}/smtpd.h ${src_dir}${patch_dir}/smtpd.h.orig && \
	    sed 's/OpenSMTPD/SMTPD/' ${src_dir}${patch_dir}/smtpd.h.orig > ${src_dir}${patch_dir}/smtpd.h
	cd ${orig_dir}${non_tree} && ${sexec} ./do.sh ${src_dir} ${orig_dir} ${patch_dir} ${fetch_cmd}
	cd ${src_dir}${patch_dir}
	build_foreign
	# Looks like 'install' doesn't create directory for me...
	sudo mkdir -p /usr/libexec/smtpd
	setup_foreign; cleanup -f Makefile.fakeutil; cleanup
	sudo useradd -u 95 -g=uid -c "SMTP Daemon" -d /var/empty -s /sbin/nologin _smtpd
	cd ${orig_dir}
fi
junk
    ask " \n
    6. Work on dhclient(8): 1) Add option for forceful logging to stderr\n
    instead of syslog, so it will behave like dhcpd(8). Old '-d' meaning\n
    is '-f' now. 2) New statement for overriding timeout before next\n
    renewal process. 3) Bring multiple default gateway support. Configuration\n
    will be shown. 4) Support for dynDNS updates using nsupdate(8).\n
    You need BIND installed for this. [Y/N]"
if [ $? -eq 1 ]; then
    patch_dir=/sys/net
    cd ${src_dir}${patch_dir}
    ask "Routing priorities were somehow broken. I'll fix them,\n
	but you'll need to recompile your kernel. OK for you? [Y/N]"
    if [ $? -eq 1 ]; then
	patch -CNu < ${orig_dir}${patch_dir}/patch-priorities
	    ask "OK for you? [Y/N]"
	if [ $? -eq 1 ]; then
	    patch -su < ${orig_dir}${patch_dir}/patch-priorities
	    if [ $? -gt 1 ]; then
		exit
	    fi
	fi
    fi
    patch_dir=/sys/netinet
    cd ${orig_dir}
    patch_func ${patch_dir}/patch-if_ether.c ${src_dir}${patch_dir}/if_ether.c
    patch_dir=/sbin/dhclient
    cd ${src_dir}${patch_dir}
    patch -CNu < ${orig_dir}${patch_dir}/patch-dhclient
	ask "OK for you? [Y/N]"
    if [ $? -eq 1 ]; then
	patch -su < ${orig_dir}${patch_dir}/patch-dhclient
	if [ $? -gt 1 ]; then
	    exit
	fi
    fi
    patch_dir=${patch_dir}
    patch -CNu < ${orig_dir}${patch_dir}/patch-dhclient.2
	ask "OK for you? [Y/N]"
    if [ $? -eq 1 ]; then
	patch -su < ${orig_dir}${patch_dir}/patch-dhclient.2
	if [ $? -gt 1 ]; then
	    exit
	fi
    fi
    patch_dir=${patch_dir}
    patch -CNu < ${orig_dir}${patch_dir}/patch-dhclient.3
	ask "OK for you? [Y/N]"
    if [ $? -eq 1 ]; then
	patch -su < ${orig_dir}${patch_dir}/patch-dhclient.3
	if [ $? -gt 1 ]; then
	    exit
	fi
    fi
    patch_dir=${patch_dir}
    patch -CNu < ${orig_dir}${patch_dir}/patch-dhclient.4
        ask "OK for you? [Y/N]"
    if [ $? -eq 1 ]; then
        patch -su < ${orig_dir}${patch_dir}/patch-dhclient.4
        if [ $? -gt 1 ]; then
            exit
        fi
    fi
    #
    build; setup; cleanup
    echo "Here is the sample configuration, showing how to route with multiple"
    echo "default gateways"
    cat ${orig_dir}/etc/dhclient-multiple-gateways.txt
    #
fi
    ask " \n
    7. Handle multiple 'hardware ethernet' per host for static leases\n
    in dhcpd(8). [Y/N]"
if [ $? -eq 1 ]; then
    patch_dir=/usr.sbin/dhcpd
    cd ${src_dir}${patch_dir}
    patch -CNu < ${orig_dir}${patch_dir}/patch-dhcpd
	ask "OK for you? [Y/N]"
    if [ $? -eq 1 ]; then
	patch -su < ${orig_dir}${patch_dir}/patch-dhcpd
	if [ $? -gt 1 ]; then
	    exit
	fi
    fi
    build; setup; cleanup
    cd ${orig_dir}
fi
    ask " \n
    8. Add compat_openbsd for sparc64 target? [Y/N]"
if [ $? -eq 1 ]; then
    # Just copying files
    echo "Adding openbsd_machdep.h"
    cp ./sys/arch/sparc64/include/openbsd_machdep.h ${src_dir}/sys/arch/sparc64/include/
    
    # Patching
    echo " "
    echo "Now patching... Look at diff output and make decision"
    echo "apply it or not."
    patch_dir=/sys/arch/sparc64/sparc64
    patch_func ${patch_dir}/patch-locore.s ${src_dir}${patch_dir}/locore.s
    patch_func ${patch_dir}/patch-trap.c ${src_dir}${patch_dir}/trap.c
fi
    ask " \n
    9. Fix LKM work on sparc64 target? [Y/N]"
if [ $? -eq 1 ]; then
    # Solution from NetBSD. Do map to put LKMs close to kernel's .text & .data, fix is similar
    # to what we done for amd64. Fixes 'reloc truncated to fit' on module load
    patch_dir=/sys/arch/sparc64/sparc64
    patch_func ${patch_dir}/patch-pmap.c ${src_dir}${patch_dir}/pmap.c
    patch_func ${patch_dir}/patch-machdep.c ${src_dir}${patch_dir}/machdep.c
fi
# END OF SOFTWARE SECTION #
fi
#			  #
    echo " "
    echo "----------------"
    echo "Hardware section"
    echo "----------------"
    ask "Want it? [Y/N]"
# BEGIN OF HARDWARE SECTION #
if [ $? -eq 1 ]; then
#                           #
    ask "1. Add lom(4) (from 4.7-current) device? Don't forget about ddb.panic=0\n
    for watchdog ;) (sparc64) [Y/N]"	
if [ $? -eq 1 ]; then
    # Just copying files
    echo "Adding lom.4 man page."
    cp ./share/man/man4/man4.sparc64/lom.4 ${src_dir}/share/man/man4/man4.sparc64/
    echo "Adding lom.c device."
    cp ./sys/arch/sparc64/dev/lom.c ${src_dir}/sys/arch/sparc64/dev/

    # Patching    
    echo " "
    echo "Now patching... Look at diff output and make decision"
    echo "apply it or not."
    patch_dir=/share/man/man4/man4.sparc64
    patch_func ${patch_dir}/patch-Makefile ${src_dir}${patch_dir}/Makefile
    patch_dir=${patch_dir}
    patch_func ${patch_dir}/patch-ebus.4 ${src_dir}${patch_dir}/ebus.4
    patch_dir=/sys/arch/sparc64/conf
    patch_func ${patch_dir}/patch-files.sparc64 ${src_dir}${patch_dir}/files.sparc64
    patch_dir=/sys/kern
    patch_func ${patch_dir}/patch-kern_timeout.c ${src_dir}${patch_dir}/kern_timeout.c
    patch_dir=/sys/sys
    patch_func ${patch_dir}/patch-timeout.h ${src_dir}${patch_dir}/timeout.h
fi

    ask " \n
    2. Finally fix long-unknown kernel panic where Davicom network chip is doing\n
    DMA to memory which was previosly unmapped by dc(4) driver. Bug fix was made\n
    by kettenis@ in \"obsd 4.6 on Netra X1\" mail list topic. By 12 Dec 2009.\n
    Also fix some little dc(4) messings like preventing to halt and \"fail\n
    to force ... to idle state\" message on init. OK? [Y/N]" 
if [ $? -eq 1 ]; then
    patch_dir=/sys/dev/ic
    # You should set pathes for multidiff, no default backup by using posix flag 
    patch -CNu -d ${src_dir}${patch_dir} --posix < .${patch_dir}/patch-dc.c dc.c
    	    ask "Ok for you? [Y/N]"
	    if [ $? -eq 1 ]; then
		# Backup it before doing multidiff
		cp ${src_dir}${patch_dir}/dc.c ${src_dir}${patch_dir}/dc.c.old || 
		    exit
		patch -su -d ${src_dir}${patch_dir} --posix < .${patch_dir}/patch-dc.c dc.c
		if [ $? -gt 1 ]; then
		    exit
		fi
	    fi	
fi
:<<junk 
    ask " \n
    3. kettenis@ says: \"Add some missing splx()'s (and missing simple_unlock()\n
    no-ops) in pmap error paths.  Spotted by Mike Belopuhov. (sparc64)\"\n
    Fix? [Y/N]"
if [ $? -eq 1 ]; then
    patch_dir=/sys/arch/sparc64/sparc64
    patch_func ${patch_dir}/patch-pmap.c ${src_dir}${patch_dir}/pmap.c
fi
    ask " \n
    4. Update cpu.c in order to add hw.cpuspeed and add support of CPU freq\n
    scaling via hw.setperf for US IIe \"Hummingbird\" (sparc64)? [Y/N]"
if [ $? -eq 1 ]; then
    update_dir=src/sys/arch/sparc64/sparc64
    cd ${src_dir}/..
    mkdir ${update_dir}/CVS 2>/dev/null; touch ${update_dir}/CVS/Entries 2>/dev/null;
    echo ${update_dir} > ${update_dir}/CVS/Repository
    rm ${update_dir}/cpu.c && \
    cvs -d ${OPENBSD_CVSROOT} -q get -r1.49 ${update_dir}/cpu.c
    cd ${orig_dir}
fi
junk
    ask " \n
    5. Fix macppc and bring initial powerbook5,8 machine support? [Y/N]"
if [ $? -eq 1 ]; then
    patch_dir=/sys/arch/macppc/macppc
    patch_func ${patch_dir}/machdep.diff ${src_dir}${patch_dir}/machdep.c
    patch_dir=/sys/arch/macppc/dev
    patch_func ${patch_dir}/adb.diff ${src_dir}${patch_dir}/adb.c
    patch_dir=${patch_dir}
    patch_func ${patch_dir}/wdc_obio.diff ${src_dir}${patch_dir}/wdc_obio.c
    patch_dir=${patch_dir}
    patch_func ${patch_dir}/tpms.diff ${src_dir}${patch_dir}/tpms.c
    # Fix interrupt storming
    patch_dir=${patch_dir}
    patch_func ${patch_dir}/patch-macgpio.c ${src_dir}${patch_dir}/macgpio.c
    update_dir=src/sys/arch/macppc/dev
    cd ${src_dir}/..
    mkdir ${update_dir}/CVS 2>/dev/null; touch ${update_dir}/CVS/Entries 2>/dev/null;
    echo ${update_dir} > ${update_dir}/CVS/Repository
    rm ${update_dir}/openpic.c && \
    cvs -d ${OPENBSD_CVSROOT} -q get -r1.43 ${update_dir}/openpic.c
    cd ${orig_dir}
    #
fi
# END OF HARDWARE SECTION #
fi
#			  #
