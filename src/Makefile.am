sysconfdir=@sysconfdir@
nemdir=@nemdir@

SUBDIRS = debug nfcconf modules

INCLUDES = $(all_includes)

AM_CFLAGS = -DNEMDIR=\"${nemdir}\" -DSYSCONFDIR=\"${sysconfdir}\" @LIBNFC_CFLAGS@
AM_LDFLAGS = @LIBNFC_LIBS@

bin_PROGRAMS = gatesrvd

gatesrvd_SOURCES = types.h initdata.h adrport.h gatectl.h adrport.c gatectl.c gatesrvd.c
gatesrvd_LDADD = $(top_builddir)/src/debug/libdebug.la \
	$(top_builddir)/src/nfcconf/libnfcconf.la @LIBNFCCONF@
noinst_HEADERS = types.h initdata.h gatectl.h adrport.h