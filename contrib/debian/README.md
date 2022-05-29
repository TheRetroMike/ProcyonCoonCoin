
Debian
====================
This directory contains files used to package procyond/procyon-qt
for Debian-based Linux systems. If you compile procyond/procyon-qt yourself, there are some useful files here.

## procyon: URI support ##


procyon-qt.desktop  (Gnome / Open Desktop)
To install:

	sudo desktop-file-install procyon-qt.desktop
	sudo update-desktop-database

If you build yourself, you will either need to modify the paths in
the .desktop file or copy or symlink your procyon-qt binary to `/usr/bin`
and the `../../share/pixmaps/procyon128.png` to `/usr/share/pixmaps`

procyon-qt.protocol (KDE)

