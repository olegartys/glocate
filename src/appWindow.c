/*
 * appWindow.c
 *
 * Copyright (C) 2014 - olegartys
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "main.h"
#include <stdlib.h>
#include <pthread.h>

#define SEARCH_COMMAND "locate"

extern WindowStruct *appWindow;
gchar *search_text;
GtkWidget *get_wg(const gchar *name, GtkBuilder *builder);	

//SIGNALS
/******************************************************************************/
static void 
search_button_clicked (GtkWidget *wg, gpointer data)
{
	gtk_list_store_clear (appWindow->tree_struct.tree_model);
	const search_text = gtk_entry_get_text (appWindow->entry1);
	gchar *search_string = g_strconcat(SEARCH_COMMAND, " ", search_text, NULL);
	
	guint t = gtk_statusbar_get_context_id(GTK_STATUSBAR(appWindow->statusbar1), "WAITING");
	gchar *s = g_strdup_printf ("Поиск...");
	gtk_statusbar_push(GTK_STATUSBAR(appWindow->statusbar1), t, s);
	g_free(s);
	
	//append elements to list from openede pipe
	FILE *out_fp = popen(search_string, "r");
	guint i = 0;
	while (!feof(out_fp)) {
		gchar buf[256];
		fgets(buf, sizeof(buf), out_fp);
		gtk_list_store_append(appWindow->tree_struct.tree_model, &appWindow->tree_struct.iter);
		gtk_list_store_set(appWindow->tree_struct.tree_model, &appWindow->tree_struct.iter, 
							0, i+1, 
							1, buf, 
							-1);
		i++;
	}
	close (out_fp);
	
	gchar *s1 = g_strdup_printf("Найдено файлов: %d", i);
	gtk_statusbar_push(GTK_STATUSBAR(appWindow->statusbar1), t, s1);
	g_free(s1);
}

static gboolean 
on_ENTER_press(GtkWidget *w, GdkEventKey *event, gpointer data)
{
	//g_print("%s\n",gdk_keyval_name (event->keyval));
	//g_print("%s\n", event->string);
	if (event->keyval == GDK_KEY_Return || event->keyval == GDK_KEY_KP_Enter) {
		pthread_t thr;
		// creating new thread to update treeView
		pthread_create (&thr, NULL, search_button_clicked, NULL);
    }
    return FALSE;
}
/******************************************************************************/

WindowStruct *
create_window (void)
{
	// Initializing builder
	GtkBuilder *builder;
	GError* error = NULL;
	builder = gtk_builder_new ();
	if (!gtk_builder_add_from_file (builder, UI_FILE, &error)) {
		g_critical ("Couldn't load builder file: %s", error->message);
		g_error_free (error);
		exit(EXIT_FAILURE);
	}

	// Getting widgets from ui file
	WindowStruct *new_appWindow = g_malloc(sizeof(WindowStruct));
	{
		new_appWindow->win = get_wg("window1", builder);
		new_appWindow->scrolled_window1 = get_wg("scrolledwindow1", builder);
		new_appWindow->entry1 = get_wg("entry1", builder);
		new_appWindow->button1 = get_wg("button1", builder);
		new_appWindow->statusbar1 = get_wg("statusbar1", builder);

		g_object_unref (builder);
	}

	// Connecting signals to widgets
	{
		g_signal_connect(new_appWindow->win, "destroy", gtk_main_quit, NULL);
		g_signal_connect(new_appWindow->button1, "clicked", search_button_clicked, NULL);
		g_signal_connect(new_appWindow->win, "key-press-event", G_CALLBACK(on_ENTER_press), NULL);
	}

	// Creating tree_view with files
	{
		new_appWindow->tree_struct.tree_model = gtk_list_store_new(2, G_TYPE_INT, G_TYPE_STRING);
		new_appWindow->tree_struct.tree_view = gtk_tree_view_new_with_model(GTK_TREE_MODEL(new_appWindow->tree_struct.tree_model));
		// first column (with number)
		GtkCellRenderer *renderer;
		GtkTreeViewColumn *column;
		gint column_number = 0;
		renderer = gtk_cell_renderer_text_new();
		column = gtk_tree_view_column_new_with_attributes("№", renderer, "text", column_number, NULL);
		gtk_tree_view_append_column (new_appWindow->tree_struct.tree_view, column);// вставляем колонку в список;
		gtk_tree_view_column_set_sort_column_id (column, column_number);// колонка сортируется;
		column_number++;
		// second column (with the name of the file)
		renderer = gtk_cell_renderer_text_new();
		column = gtk_tree_view_column_new_with_attributes("Файл", renderer, "text", column_number, NULL);
		gtk_tree_view_append_column (new_appWindow->tree_struct.tree_view, column);// вставляем колонку в список;
		gtk_tree_view_column_set_sort_column_id (column, column_number);// колонка сортируется;
		column_number++;
		gtk_container_add(GTK_CONTAINER(new_appWindow->scrolled_window1), new_appWindow->tree_struct.tree_view);
	}
	return new_appWindow;
}
