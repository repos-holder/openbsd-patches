#!/bin/sh

DESTDIR=/
TASKS=2

#LDFLAGS=-L${DESTDIR}/usr/lib # limits to single pass build only
MAKEFLAGS="-m ${DESTDIR}/usr/share/mk"
export DESTDIR #LDFLAGS

#test -d ${DESTDIR} && mv ${DESTDIR} ${DESTDIR}.old && rm -rf ${DESTDIR}.old &
#mkdir -p ${DESTDIR}

# Exclude MD bits
sed_func ()
{
    cp ${path} ${path}.orig && \
        sed "$@" \
        ${path}.orig > ${path}
} 
path=/usr/src/usr.bin/Makefile
sed_func -e "s/\ ld\ / /" -e "s/pcc//" -e "s/cpp//"
if [ `sysctl -n hw.byteorder` -eq "4321" ]; then
	sed_func "s/[nt]roff//"
fi
#

#rm -rf /usr/obj/*
cd /usr/src
make obj

cd /usr/src/etc && make distrib-dirs

export MAKEFLAGS

for i in `ls /usr/src/share/mk`
do
    cp -f /usr/src/share/mk/${i} ${DESTDIR}/usr/share/mk/
done
# for microcode/atmel
ln -s /usr/lib/libc.so* ${DESTDIR}/usr/lib/

cd /usr/src
make build -j${TASKS}
