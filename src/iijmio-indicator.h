/*
 * IIJmio application indicator
 * Copyright (c) 2013, HAYASHI Kentaro <kenhys@gmail.com>
 * All rights reserved.
 *
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#ifndef IIJMIO_INDICATOR_H
#define IIJMIO_INDICATOR_H

#define INDICATOR_BOX_SPACE 6

#define INDICATOR_ATTENTION_ICON "iijmio-on"
#define INDICATOR_ICON "iijmio-off"

#define IIJMIO_INDICATOR_WEBSITE_LABEL \
  "GiGo I.C.E."
#define IIJMIO_INDICATOR_WEBSITE_URL \
  "http://gigo-ice.org/tech/iijmio/application-indicator/index.html"

#define ENABLE_IIJMIO_COUPON_ACTION "enable-coupon"
#define ENABLE_IIJMIO_COUPON_ID "enable-coupon"
#define ENABLE_IIJMIO_COUPON_LABEL N_("_Enable IIJmio coupon")
#define ENABLE_IIJMIO_COUPON_TOOLTIP N_("Enable IIJmio coupon")

#define DISABLE_IIJMIO_COUPON_ACTION "disable-coupon"
#define DISABLE_IIJMIO_COUPON_ID "disable-coupon"
#define DISABLE_IIJMIO_COUPON_LABEL N_("Disable IIJmio coupon")
#define DISABLE_IIJMIO_COUPON_TOOLTIP N_("Disable IIJmio coupon")

struct _Widgets {
  GtkWidget *access_token;
};
typedef struct _Widgets Widgets;

struct _IIJmioIndicatorOption {
  gchar *rcpath;
  GKeyFile *rcfile;

  Widgets widgets;
  gchar *access_token;
};
typedef struct _IIJmioIndicatorOption IIJmioIndicatorOption;

static void indicator_activate_cb(GtkAction *action);
static void indicator_enable_coupon(void);
static void indicator_disable_coupon(void);
static void indicator_preference(void);
static void indicator_about(void);

static void indicator_rest_coupon_cb(GtkWidget *widget, gpointer data);

static GtkWidget *create_config_main_page(GtkWidget *dialog, GtkWidget *notebook,
                                          IIJmioIndicatorOption *option);

#endif

