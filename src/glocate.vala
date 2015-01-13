/*
 * glocate.vala
 * 
 * Copyright 2014 olegartys <olegartys@olegartys-ubuntu>
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
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */

using Gtk;

class AppWindow : Window 
{
	private Entry entry;
	public Button search_button;
	public AppWindow (ref Builder builder) {
		this.entry = builder.get_object ("entry1") as Entry;
		this.search_button = builder.get_object ("button1") as Button;
	}
}

class glocate 
{
	public static void BuilderInit (ref Builder builder) {
		try {
			builder.add_from_file ("glocate.ui");
		} catch (Error e) {
			stderr.printf ("Could not load UI: %s\n", e.message);
		}
	}
	
    static int main(string[] args) {
		Gtk.init (ref args);
		var builder = new Builder ();
		glocate.BuilderInit (ref builder);
		
		/*var window = new AppWindow (ref builder);
		window = (AppWindow) builder.get_object ("window1");
		window.show_all();
		window.destroy.connect (Gtk.main_quit);
		window.search_button.clicked.connect (() => {stdout.printf("Hello");});*/
		Gtk.main ();
        return 0;
    }
}

