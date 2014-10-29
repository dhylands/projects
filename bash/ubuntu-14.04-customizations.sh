Partially disable aut-maximize:
gconftool --set --type=bool /apps/metacity/general/auto_maximize_windows False

To turn off Shopping Suggestions
System Settings -> Security & Privacy -> Search -> Set "Include online search results" to off


Other tips may be on:
http://www.techsupportalert.com/content/tips-and-tricks-ubuntu-after-installation-ubuntu-1404.htm

To change the color of the tabs in the terminal window:

Create ~/.config/gtk-3.0/gtk.css with the contents:

	TerminalWindow .notebook tab {
	    background-color: #d2d0cd;
	}
	TerminalWindow .notebook tab:active {
	    background-color: #f2f1f0;
	}

