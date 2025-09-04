## Process this file with automake to produce Makefile.in

SUBDIRS = src data

atomes_datadir = $(DESTDIR)$(datadir)
atomes_pkgdatadir = $(DESTDIR)$(pkgdatadir)
atomes_iconsdir = $(atomes_datadir)/pixmaps
atomes_desktopdir = $(atomes_datadir)/applications
atomes_mimedir = $(atomes_datadir)/mime/packages/
atomes_metadir = $(atomes_datadir)/metainfo
atomes_bashdir = $(atomes_datadir)/bash-completion/completions
atomes_docdir = $(docdir)
atomes_doc_DATA = \
	README.md \
	AUTHORS.md \
	ChangeLog

atomes_mandir = $(mandir)/man1/
atomes_man_DATA = \
	atomes.1.gz 

install-data-local:
# Bash completion
	if [ ! -d $(atomes_bashdir) ]; then \
		$(mkinstalldirs) $(atomes_bashdir); \
	fi
	$(INSTALL_DATA) $(srcdir)/atomes-completion.sh $(atomes_bashdir)/atomes
# Program's icons
	if [ ! -d $(atomes_iconsdir) ]; then \
		$(mkinstalldirs) $(atomes_iconsdir); \
	fi
	$(INSTALL_DATA) $(srcdir)/metadata/icons/atomes.svg $(atomes_iconsdir)
	$(INSTALL_DATA) $(srcdir)/metadata/icons/atomes-workspace.svg $(atomes_iconsdir)
	$(INSTALL_DATA) $(srcdir)/metadata/icons/atomes-project.svg $(atomes_iconsdir)
	$(INSTALL_DATA) $(srcdir)/metadata/icons/atomes-coordinates.svg $(atomes_iconsdir)
# Custom MIME types
	if [ ! -d $(atomes_mimedir) ]; then \
		$(mkinstalldirs) $(atomes_mimedir); \
	fi
	$(INSTALL_DATA) $(srcdir)/metadata/atomes-mime.xml $(atomes_mimedir)
# Meta info
	if [ ! -d $(atomes_metadir) ]; then \
		$(mkinstalldirs) $(atomes_metadir); \
	fi
	$(INSTALL_DATA) $(srcdir)/metadata/fr.ipcms.atomes.appdata.xml $(atomes_metadir)
	appstream-util validate-relax --nonet $(atomes_metadir)/fr.ipcms.atomes.appdata.xml
# Desktop file
	if [ ! -d $(atomes_desktopdir) ]; then \
		$(mkinstalldirs) $(atomes_desktopdir); \
	fi
	$(INSTALL_DATA) $(srcdir)/metadata/atomes.desktop  $(atomes_desktopdir)/atomes.desktop
# Finalize
	touch -c $(atomes_iconsdir)
	if [ -u `which gtk-update-icon-cache` ]; then \
	  	gtk-update-icon-cache -q $(atomes_iconsdir); \
	fi
	desktop-file-install --vendor="" \
		--dir=$(atomes_desktopdir) -m 644 \
		$(atomes_desktopdir)/atomes.desktop
	if [ -z "$(DESTDIR)" ]; then \
		update-desktop-database $(atomes_desktopdir) &> /dev/null || :; \
		update-mime-database $(atomes_datadir)/mime &> /dev/null || :; \
	fi

# Remove doc directordy on uninstall
uninstall-local:
	-rm -f $(atomes_iconsdir)/atomes.svg
	-rm -f $(atomes_iconsdir)/atomes-workspace.svg
	-rm -f $(atomes_iconsdir)/atomes-project.svg
	-rm -f $(atomes_iconsdir)/atomes-coordinates.svg
	-rm -f $(atomes_desktopdir)/atomes.desktop
	-rm -f $(atomes_metadir)/fr.ipcms.atomes.appdata.xml
	-rm -f $(atomes_mimedir)/atomes-mime.xml
	-rm -f $(atomes_bashdir)/atomes
	-rmdir $(atomes_docdir)
	touch -c $(atomes_iconsdir)
	if [ -u `which gtk-update-icon-cache` ]; then \
		gtk-update-icon-cache -q $(atomes_iconsdir); \
	fi
	if [ -z "$(DESTDIR)" ]; then \
		update-desktop-database $(atomes_desktopdir) &> /dev/null || :; \
		update-mime-database $(atomes_datadir)/mime &> /dev/null || :; \
	fi
