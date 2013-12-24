
bin_PROGRAMS = iijmio-indicator

iijmio_indicator_SOURCES = \
	iijmio-indicator.c

APPINDICATOR_CFLAGS = `pkg-config --cflags appindicator-0.1`
APPINDICATOR_LDFLAGS = `pkg-config --libs appindicator-0.1`

AM_CFLAGS = \
	$(GTK_CFLAGS) \
	$(APPINDICATOR_CFLAGS) \
	-I$(includedir)

iijmio_indicator_LDFLAGS = $(APPINDICATOR_LDFLAGS)

iijmio_indicator_LDADD = $(GTK_LIBS)

