/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * main.c
 * Copyright (C) 2014 olegartys <olegartys@gmail.com>
 * 
 * glocate is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * glocate is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <gtk/gtk.h>
#include "main.h"

WindowStruct *appWindow = NULL;

// SIGNALS
/******************************************************************************/

GtkWidget *
get_wg(const gchar *name, GtkBuilder *builder)
{
	GtkWidget *wg;
	if (!( wg = GTK_WIDGET(gtk_builder_get_object(builder, name)) )) {
		g_critical("%s %s", "Can't find widget!", name);
        return NULL;
    }
    return wg;
}
/******************************************************************************/
gint
main (gint argc, gchar *argv[])
{
	gtk_init (&argc, &argv);

	appWindow = create_window ();
	gtk_widget_show_all (appWindow->win);

	gtk_main ();
	return 0;
}

