
bin_PROGRAMS = iijmio-indicator

iijmio_indicator_SOURCES = \
	iijmio-indicator.c

APPINDICATOR_CFLAGS = `pkg-config --cflags appindicator-0.1`
APPINDICATOR_LDFLAGS = `pkg-config --libs appindicator-0.1`
JSON_GLIB_CFLAGS = `pkg-config --cflags json-glib-1.0`
JSON_GLIB_LDFLAGS = `pkg-config --libs json-glib-1.0`
CURL_CFLAGS = `pkg-config --cflags libcurl`
CURL_LDFLAGS = `pkg-config --libs libcurl`

AM_CFLAGS =			\
	$(GTK_CFLAGS)		\
	$(APPINDICATOR_CFLAGS)	\
	$(JSON_GLIB_CFLAGS)	\
	$(CURL_CFLAGS)		\
	-I$(includedir)

iijmio_indicator_LDFLAGS =	\
	$(APPINDICATOR_LDFLAGS)	\
	$(JSON_GLIB_LDFLAGS)	\
	$(CURL_LDFLAGS)

iijmio_indicator_LDADD = $(GTK_LIBS)

