## Process this file with automake to produce Makefile.in
## Created by Anjuta

SUBDIRS = include tests
	 

cppcontainersdocdir = ${prefix}/doc/cppcontainers
cppcontainersdoc_DATA = \
	README\
	COPYING\
	AUTHORS\
	ChangeLog\
	INSTALL\
	NEWS


EXTRA_DIST = $(cppcontainersdoc_DATA)


# Remove doc directory on uninstall
uninstall-local:
	-rm -r $(cppcontainersdocdir)
