#!/bin/sh
# $gate: get.sh,v 1.1 2010/12/14 02:43:05 dukzcry Exp $

set -e

# External settings passed as arguments
src_dir=$1
orig_dir=$2
patch_dir=/usr.bin/tmux
revision=`cat revision.txt`

# Getting tmux
git clone -n git://anoncvs.estpak.ee/openbsd-src src
cd src
git config core.sparsecheckout true
echo usr.bin/tmux/ > .git/info/sparse-checkout
echo lib/libutil/ >> .git/info/sparse-checkout
git checkout ${revision}

# Moving stuff into the tree
mv usr.bin/tmux ${src_dir}/usr.bin/
mv lib/libutil ${src_dir}/lib/libutil.tmux
cd .. && rm -rf src

# Required cause of estpak sync glitch, where signal.c is too old
rm ${src_dir}/usr.bin/tmux/signal.c
cvs -qd anoncvs@anoncvs.openbsd.org:/cvs get -P -r1.6 src/usr.bin/tmux/signal.c
mv src/usr.bin/tmux/signal.c ${src_dir}/usr.bin/tmux/ && rm -rf src

cd ${orig_dir}
# Imsg was moved into libutil, bring it back
cp lib/libutil.tmux/Makefile ${src_dir}/lib/libutil.tmux/

# Makefile for project
cp usr.bin/tmux/Makefile ${src_dir}/Makefile.tmux

# Add missing libevent function
cp usr.bin/tmux/event.h ${src_dir}/usr.bin/tmux

# Patching tmux
patch -su < .${patch_dir}/patch-Makefile.tmux ${src_dir}${patch_dir}/Makefile
patch -su < .${patch_dir}/patch-tmux.h ${src_dir}${patch_dir}/tmux.h
patch -su < .${patch_dir}/patch-server.c ${src_dir}${patch_dir}/server.c
patch -su < .${patch_dir}/patch-Makefile ${src_dir}/Makefile
