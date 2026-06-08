# Nom du programme (doit correspondre à GETTEXT_PACKAGE dans configure.ac)
PACKAGE = atomes
atomes_locale = $(DESTDIR)$(localedir)

# Liste des fichiers de langue à compiler
POFILES = en/atomes-messages.po \
					fr/atomes-messages.po

MOFILES = $(patsubst %.po,%.mo,$(POFILES))

# Règle de compilation .po -> .mo
.po.mo:
	msgfmt -o $@ $<

all-local: $(MOFILES)

# Installation des fichiers .mo dans le dossier locale de destination
install-data-local:
	for mo in $(MOFILES); do \
		lang=`echo $$mo |sed 's/\/atomes-messages.mo//'`; \
		$(mkinstalldirs) $(atomes_locale)/$$lang/LC_MESSAGES; \
		$(INSTALL_DATA) $$mo $(atomes_locale)/$$lang/LC_MESSAGES/$(PACKAGE).mo; \
	done

# Nettoyage lors de la désinstallation
uninstall-local:
	for po in $(POFILES); do \
		lang=`echo $$po |sed 's/\/atomes-messages.po//'`; \
		rm -f $(atomes_locale)/$$lang/LC_MESSAGES/$(PACKAGE).mo; \
	done

# Fichiers à nettoyer
CLEANFILES = $(MOFILES)

# Fichiers à inclure dans la distribution
EXTRA_DIST = $(POFILES)
