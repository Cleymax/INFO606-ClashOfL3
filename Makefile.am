## Process this file with automake to produce Makefile.in

SUBDIRS = col3-bibtp

AM_CPPFLAGS = \
	-DPACKAGE_DATA_DIR=\""$(pkgdatadir)"\"

AM_CFLAGS =\
	-Wall -Wextra\
	-fdiagnostics-show-option\
	-g\
	-pthread -Icol3-bibtp
 
bin_PROGRAMS = col3_client_etu

col3_client_etu_SOURCES = \
	main.c \
	clientCOL3.c \
	clientCOL3.h

col3_client_etu_LDADD =  \
	-lncurses \
	-lrt \
	col3-bibtp/libcol3tp.a
