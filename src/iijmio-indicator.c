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

#include "config.h"
#include <glib.h>
#include <glib/gprintf.h>
#include <glib/gi18n-lib.h>
#include <locale.h>
#include <gtk/gtk.h>

#include <libappindicator/app-indicator.h>
#include <json-glib/json-glib.h>
#include <curl/curl.h>
#include "iijmio-indicator.h"

static GtkActionEntry entries[] = {
  { "PopupMenu", NULL, "_PopupMenu" },
  { ENABLE_IIJMIO_COUPON_ACTION,
    ENABLE_IIJMIO_COUPON_ID,
    ENABLE_IIJMIO_COUPON_LABEL, NULL,
    ENABLE_IIJMIO_COUPON_TOOLTIP, G_CALLBACK(indicator_activate_cb) },
  { DISABLE_IIJMIO_COUPON_ACTION,
    DISABLE_IIJMIO_COUPON_ID,
    DISABLE_IIJMIO_COUPON_LABEL, NULL,
    DISABLE_IIJMIO_COUPON_TOOLTIP, G_CALLBACK(indicator_activate_cb) },
  { "preference", "preference-open", "_Preference", NULL,
    "Preference", G_CALLBACK(indicator_activate_cb) },
  { "about", "about-open", "_About", NULL,
    "About", G_CALLBACK(indicator_activate_cb) },
  { "quit",     "application-exit", "_Quit", "<control>Q",
    "Exit the application", G_CALLBACK (gtk_main_quit) },
};
static guint n_entries = G_N_ELEMENTS(entries);

static IIJmioIndicatorOption option;

static void indicator_activate_cb(GtkAction *action)
{
  const gchar *name = gtk_action_get_name(action);

  g_debug("%s %s called\n", G_STRFUNC, name);

  if (g_strcmp0(name, "enable-coupon") == 0) {
    indicator_enable_coupon();
  } else if (g_strcmp0(name, "disable-coupon") == 0) {
    indicator_disable_coupon();
  } else if (g_strcmp0(name, "preference") == 0) {
    indicator_preference();
  } else if (g_strcmp0(name, "about") == 0) {
    indicator_about();
  } else {
  }
}

static void indicator_enable_coupon()
{
  GtkWidget *dialog;

  dialog = gtk_message_dialog_new(NULL,
                                  GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                                  GTK_MESSAGE_WARNING,
                                  GTK_BUTTONS_OK,
                                  "%s", N_("Enable coupon is not supported yet."),
                                  NULL);
  gtk_dialog_run(GTK_DIALOG(dialog));
  gtk_widget_destroy(dialog);
}

static void indicator_disable_coupon()
{
  GtkWidget *dialog;
  dialog = gtk_message_dialog_new(NULL,
                                  GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                                  GTK_MESSAGE_WARNING,
                                  GTK_BUTTONS_OK,
                                  "%s", N_("Disable coupon is not supported yet."),
                                  NULL);
  gtk_dialog_run(GTK_DIALOG(dialog));
  gtk_widget_destroy(dialog);
}

static void indicator_about()
{
  GtkWidget *dialog;
  gint response;

  dialog = gtk_about_dialog_new();

  gtk_about_dialog_set_program_name(GTK_ABOUT_DIALOG(dialog),
                                    "IIJmio indicator");
  gtk_about_dialog_set_version(GTK_ABOUT_DIALOG(dialog),
                               PACKAGE_VERSION);
  gtk_about_dialog_set_copyright(GTK_ABOUT_DIALOG(dialog),
                                 "Copyright 2013 HAYASHI Kentaro");
  gtk_about_dialog_set_wrap_license(GTK_ABOUT_DIALOG(dialog), TRUE);
  gtk_about_dialog_set_website(GTK_ABOUT_DIALOG(dialog),
                               IIJMIO_INDICATOR_WEBSITE_URL);
  gtk_about_dialog_set_website_label(GTK_ABOUT_DIALOG(dialog),
                                     IIJMIO_INDICATOR_WEBSITE_LABEL);

  gtk_widget_show_all(dialog);
  response = gtk_dialog_run(GTK_DIALOG(dialog));
  gtk_widget_destroy(dialog);
}

static GtkWidget *create_preference_dialog()
{
  GtkWidget *dialog;
  GtkWidget *vbox, *hbox;
  GtkWidget *notebook;
  GtkWidget *page;
  GtkWidget *label;

  dialog = gtk_dialog_new_with_buttons(_("IIJmio indicator"),
                                       NULL,
                                       GTK_DIALOG_MODAL,
                                       GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                       GTK_STOCK_OK, GTK_RESPONSE_OK,
                                       NULL);

  vbox = gtk_vbox_new(FALSE, INDICATOR_BOX_SPACE);
  hbox = gtk_hbox_new(TRUE, INDICATOR_BOX_SPACE);

  gtk_container_add(GTK_CONTAINER(hbox), vbox);
  gtk_container_add(GTK_CONTAINER(GTK_DIALOG(dialog)->vbox), hbox);

  notebook = gtk_notebook_new();
  page = create_config_main_page(dialog, notebook, (gpointer)&option);
  label = gtk_label_new(_("General"));
  gtk_notebook_append_page(GTK_NOTEBOOK(notebook), page, label);

  gtk_box_pack_start(GTK_BOX(vbox), notebook, TRUE, TRUE, 0);

  gtk_box_set_spacing(GTK_BOX(GTK_DIALOG(dialog)->vbox), INDICATOR_BOX_SPACE);

  return dialog;
}

static GtkWidget *create_config_main_page(GtkWidget *dialog,
                                          GtkWidget *notebook,
                                          IIJmioIndicatorOption *option)
{
  GtkWidget *align, *frame, *frame_align;
  GtkWidget *page;
  GtkWidget *vbox, *hbox;
  GtkWidget *label;
  GtkWidget *text;
  GtkWidget *rest_coupon;

  label = gtk_label_new(_("Access Token:"));
  text = gtk_entry_new();

  option->widgets.access_token = text;

  hbox = gtk_hbox_new(FALSE, INDICATOR_BOX_SPACE);
  gtk_box_pack_start(GTK_BOX(hbox), label, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(hbox), text, TRUE, TRUE, 0);

  align = gtk_alignment_new(0, 0, 1, 1);
  gtk_alignment_set_padding(GTK_ALIGNMENT(align),
                            INDICATOR_BOX_SPACE,
                            INDICATOR_BOX_SPACE,
                            INDICATOR_BOX_SPACE,
                            INDICATOR_BOX_SPACE);

  frame = gtk_frame_new(_("IIJmio Access Token"));
  frame_align = gtk_alignment_new(0, 0, 1, 1);
  gtk_alignment_set_padding(GTK_ALIGNMENT(frame_align),
                            INDICATOR_BOX_SPACE,
                            INDICATOR_BOX_SPACE,
                            INDICATOR_BOX_SPACE,
                            INDICATOR_BOX_SPACE);

  gtk_container_add(GTK_CONTAINER(frame_align), hbox);
  gtk_container_add(GTK_CONTAINER(frame), frame_align);
  gtk_container_add(GTK_CONTAINER(align), frame);

  vbox = gtk_vbox_new(FALSE, INDICATOR_BOX_SPACE);

  gtk_box_pack_start(GTK_BOX(vbox), align, FALSE, FALSE, 0);

  rest_coupon = gtk_button_new_with_label(_("Check rest coupon"));
  g_signal_connect(G_OBJECT(rest_coupon), "clicked",
                   G_CALLBACK(indicator_rest_coupon_cb), option);

  gtk_box_pack_start(GTK_BOX(vbox), rest_coupon, FALSE, FALSE, 0);

  return vbox;
}

static void indicator_preference(void)
{
  GtkWidget *dialog;
  gint response;

  dialog = create_preference_dialog();

  gtk_widget_show_all(dialog);
  response = gtk_dialog_run(GTK_DIALOG(dialog));

  switch (response) {
  case GTK_RESPONSE_OK:
    break;
  case GTK_RESPONSE_CANCEL:
  default:
    break;
  }

  gtk_widget_destroy(dialog);
}

static void indicator_rest_coupon_cb(GtkWidget *widget, gpointer data)
{
  IIJmioIndicatorOption *option = data;
  const gchar *access_token;
  gboolean enable_debug;
  CURL *curl;
  CURLcode res;
  struct curl_slist *chunk = NULL;
  FILE *json_file;
  gchar *json_path;
  gchar *json_dir;
  gchar *token_header;
  const gchar *home_dir;
  gchar *basename;
  GDateTime *datetime;

  access_token = gtk_entry_get_text(GTK_ENTRY(option->widgets.access_token));

  curl_global_init(CURL_GLOBAL_DEFAULT);
  curl = curl_easy_init();
  if (curl) {
    curl_easy_setopt(curl,
                     CURLOPT_URL,
                     "https://api.iijmio.jp/mobile/d/v1/coupon/");
    chunk = curl_slist_append(chunk,
                              "X-IIJmio-Developer: XpVuCioCmLzAeewwiDs");
    token_header = g_strdup_printf("%s: %s",
                                   "X-IIJmio-Authorization", access_token);
    chunk = curl_slist_append(chunk, token_header);
    res = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);

    home_dir = g_get_home_dir();
    json_dir = g_build_path(G_DIR_SEPARATOR_S,
                            home_dir,
                            ".iijmio-appindicator",
                            "json",
                            NULL);
    g_mkdir_with_parents(json_dir, 0700);

    datetime = g_date_time_new_now_local();
    basename = g_date_time_format(datetime, "%Y%m%d-%H%M%S");
    json_path = g_strdup_printf("%s%s%s.json",
                                json_dir,
                                G_DIR_SEPARATOR_S,
                                basename);

    json_file = fopen(json_path, "w+");
    if (json_file) {

      curl_easy_setopt(curl, CURLOPT_FILE, json_file);

      res = curl_easy_perform(curl);
      if (res != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform() failed: %s\n",
                curl_easy_strerror(res));
      }
      fclose(json_file);
    }
    g_free(token_header);
    curl_easy_cleanup(curl);
  }
  curl_global_cleanup();
}

static GtkWidget *create_iijmio_window(void)
{
  GtkWidget *window;

  window = gtk_window_new(GTK_WINDOW_POPUP);
  gtk_window_set_title(GTK_WINDOW(window), _("IIJmio indicator"));
  gtk_window_set_icon_name(GTK_WINDOW(window), INDICATOR_ICON);
  g_signal_connect(G_OBJECT (window),
                   "destroy",
                   G_CALLBACK (gtk_main_quit),
                   NULL);

  return window;
}

static void print_theme_path(GtkIconTheme *theme)

{
  gchar **paths;
  gint n_paths;
  gint i;

  gtk_icon_theme_get_search_path(theme,
                                 &paths,
                                 &n_paths);
  g_print("path elements:%d\n", n_paths);
  for (i = 0; i < n_paths; i++) {
    g_print("%s theme path[%d]: %s\n", G_STRFUNC, i, paths[i]);
  }
  g_strfreev(paths);
}

static void create_iijmio_indicator(GtkUIManager *ui)
{
  AppIndicator *indicator;
  GtkWidget *indicator_menu;
  GtkIconTheme *theme;
  gchar *current_dir;
  gchar *theme_path;

  indicator = app_indicator_new("iijmio-indicator-client",
                                INDICATOR_ICON,
                                APP_INDICATOR_CATEGORY_APPLICATION_STATUS);
  indicator_menu = gtk_ui_manager_get_widget (ui, "/ui/IndicatorPopup");

  current_dir = g_get_current_dir();
  g_print("%s current_dir:%s\n", G_STRFUNC, current_dir);
  theme_path = g_build_path(G_DIR_SEPARATOR_S,
                            current_dir,
                            "data",
                            NULL);
  g_print("%s path:%s\n", G_STRFUNC, theme_path);
  app_indicator_set_icon_theme_path(indicator,
                                    theme_path);
  g_free(current_dir);
  g_free(theme_path);


  app_indicator_set_status(indicator, APP_INDICATOR_STATUS_ACTIVE);
  app_indicator_set_attention_icon(indicator, INDICATOR_ATTENTION_ICON);

  app_indicator_set_menu(indicator, GTK_MENU(indicator_menu));
  gtk_widget_show_all(indicator_menu);
}

static const gchar *ui_info =
"<ui>"
"  <popup name='IndicatorPopup'>"
"    <menuitem action='enable-coupon' />"
"    <menuitem action='disable-coupon' />"
"    <separator/>"
"    <menuitem action='preference' />"
"    <separator/>"
"    <menuitem action='about' />"
"    <separator/>"
"    <menuitem action='quit' />"
"  </popup>"
"</ui>";

GtkUIManager *create_iijmio_uimanager(GtkWidget *window)
{
  GtkUIManager *ui;
  GtkActionGroup *action_group;
  GError *error;

  ui = gtk_ui_manager_new();
  g_object_set_data_full(G_OBJECT(window),
                         "ui-manager", ui,
                         g_object_unref);
  if (!gtk_ui_manager_add_ui_from_string(ui, ui_info, -1, &error)) {
    g_message("Failed to build menus: %s\n", error->message);
    g_error_free (error);
    error = NULL;
  }

  action_group = gtk_action_group_new("IIJmioActions");
  gtk_action_group_add_actions(action_group,
                               entries, n_entries,
                               window);
  gtk_ui_manager_insert_action_group(ui, action_group, 0);
  gtk_window_add_accel_group(GTK_WINDOW(window),
                             gtk_ui_manager_get_accel_group(ui));

  return ui;
}

int main(int argc, char *argv[])
{
  GtkWidget *window;
  GtkUIManager *ui;

  gtk_init(&argc, &argv);

  window = create_iijmio_window();

  gtk_widget_show_all(window);
  ui = create_iijmio_uimanager(window);

  create_iijmio_indicator(ui);

  gtk_widget_hide(window);
  gtk_main();

  return 0;
}
