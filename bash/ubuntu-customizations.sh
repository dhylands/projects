#!/bin/sh

echo "Moving min/max/close over to the right..."

gconftool-2 --set "/apps/metacity/general/button_layout" --type string "menu:minimize,maximize,close"

# The following is especially useful if you use VNC
echo "Make the background be a plain color (no image)..."

gconftool-2 --set "/desktop/gnome/background/primary_color"         --type string "#2c2c00001e1e"
gconftool-2 --set "/desktop/gnome/background/secondary_color"       --type string "#2c2c00001e1e"
gconftool-2 --set "/desktop/gnome/background/color_shading_type"    --type string "solid"
gconftool-2 --set "/desktop/gnome/background/picture_options"       --type string "none"

echo "Make the terminal have 9999 lines of scroll back..."

gconftool-2 --set "/apps/gnome-terminal/profiles/Default/scrollback-lines" --type int 9999

echo "Changing terminal to be gray on black..."

gconftool-2 --set "/apps/gnome-terminal/profiles/Default/use_theme_colors" --type bool false
gconftool-2 --set "/apps/gnome-terminal/profiles/Default/background_color" --type string "#000000000000"
gconftool-2 --set "/apps/gnome-terminal/profiles/Default/foreground_color" --type string "#AAAAAAAAAAAA"

echo "Customizing Keybindings for next/prev tab..."

gconftool-2 --set "/apps/gnome-terminal/keybindings/prev_tab" --type string "<Shift>Left"
gconftool-2 --set "/apps/gnome-terminal/keybindings/next_tab" --type string "<Shift>Right"

echo "Customizing gnome-terminal to start larger..."

mkdir -p ${HOME}/.local/share/applications
cat > ${HOME}/.local/share/applications/gnome-terminal.desktop << EOF
#!/usr/bin/env xdg-open

[Desktop Entry]
Name=Terminal
Comment=Use the command line
TryExec=gnome-terminal
Exec=gnome-terminal --geometry=160x50
Icon=utilities-terminal
Type=Application
X-GNOME-DocPath=gnome-terminal/index.html
X-GNOME-Bugzilla-Bugzilla=GNOME
X-GNOME-Bugzilla-Product=gnome-terminal
X-GNOME-Bugzilla-Component=BugBuddyBugs
X-GNOME-Bugzilla-Version=2.32.0
Categories=GNOME;GTK;Utility;TerminalEmulator;
StartupNotify=true
OnlyShowIn=GNOME;
X-Ubuntu-Gettext-Domain=gnome-terminal
Terminal=false
EOF

echo "Making a gnome-terminal autostart whenever you login..."

mkdir -p ${HOME}/.config/autostart
cat > ${HOME}/.config/autostart/gnome-terminal.desktop << EOF

[Desktop Entry]
Type=Application
Exec=gnome-terminal --geometry=160x50
Hidden=false
NoDisplay=false
X-GNOME-Autostart-enabled=true
Name[en_CA]=gnome-terminal
Name=gnome-terminal
Comment[en_CA]=gnome-terminal
Comment=gnome-terminal
EOF

