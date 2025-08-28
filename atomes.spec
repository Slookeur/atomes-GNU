Name:           atomes
%global upname atomes-GNU
Version:        1.2.0
Release:        1%{?dist}
Summary:        An atomistic toolbox
License:        AGPL-3.0-or-later
Source0:        https://github.com/Slookeur/%{upname}/archive/refs/tags/v%{version}.tar.gz
# Source1:        ./v%%{version}.tar.gz.asc
# Source2:        %%{name}.gpg
URL:            https://%{name}.ipcms.fr/

BuildRequires: gnupg2
BuildRequires: make
BuildRequires: automake
BuildRequires: autoconf
BuildRequires: pkgconf-pkg-config
BuildRequires: gcc
BuildRequires: gcc-gfortran
BuildRequires: libgfortran
BuildRequires: desktop-file-utils
BuildRequires: libappstream-glib

# pkg-config 
BuildRequires: pkgconfig(gtk+-3.0)
BuildRequires: pkgconfig(libxml-2.0)
BuildRequires: pkgconfig(pangoft2)
BuildRequires: pkgconfig(glu)
BuildRequires: pkgconfig(epoxy)
BuildRequires: pkgconfig(libavutil)
BuildRequires: pkgconfig(libavcodec)
BuildRequires: pkgconfig(libavformat)
BuildRequires: pkgconfig(libswscale)

# Runtime requirements
Requires: gtk3
Requires: mesa-libGLU

Provides: %{name} = %{version}-%{release}

%description
atomes: a toolbox to analyze, to visualize 
and to create/edit three-dimensional atomistic models.
It offers a workspace that allows to have many projects opened simultaneously.
The different projects in the workspace can exchange data: 
analysis results, atomic coordinates...
atomes also provides an advanced input preparation system 
for further calculations using well known molecular dynamics codes:

    Classical MD: DLPOLY and LAMMPS
    ab-initio MD: CPMD and CP2K
    QM-MM MD: CPMD and CP2K

To prepare the input files for these calculations is likely to be the key, 
and most complicated step towards MD simulations.
atomes offers a user-friendly assistant to help and guide the scientist
step by step to achieve this crucial step.

%prep
# %%{gpgverify} --keyring='%%{SOURCE2}' --signature='%%{SOURCE1}' --data='%%{SOURCE0}'
%autosetup -n %{upname}-%{version}

%build
%configure
#make %%{?_smp_mflags}
%make_build

%install
%make_install

%check
desktop-file-validate %{buildroot}/%{_datadir}/applications/%{name}.desktop
appstream-util validate-relax --nonet %{buildroot}%{_metainfodir}/fr.ipcms.%{name}.appdata.xml

%files
%license COPYING
%{_bindir}/%{name}
%{_libexecdir}/%{name}_startup_testing
%{_datadir}/doc/%{name}/
%{_mandir}/man1/%{name}.1*
%{_datadir}/%{name}/
%{_datadir}/bash-completion/completions/%{name}
%{_datadir}/applications/%{name}.desktop
%{_datadir}/mime/packages/%{name}-mime.xml
%{_datadir}/pixmaps/%{name}.svg
%{_datadir}/pixmaps/%{name}-workspace.svg
%{_datadir}/pixmaps/%{name}-project.svg
%{_datadir}/pixmaps/%{name}-coordinates.svg
%{_metainfodir}/fr.ipcms.%{name}.appdata.xml

%changelog
* Fri Sep 05 2025 Sébastien Le Roux <sebastien.leroux@ipcms.unistra.fr> - 1.2.0-1
- New features (see:  https://github.com/Slookeur/atomes-GNU/releases/tag/v1.2.0)

* Thu May 15 2025 Sébastien Le Roux <sebastien.leroux@ipcms.unistra.fr> - 1.1.18-1
- Bug corrections (see: https://github.com/Slookeur/atomes-GNU/releases/tag/v1.1.18)

* Thu Feb 20 2025 Sébastien Le Roux <sebastien.leroux@ipcms.unistra.fr> - 1.1.17-1
- Bug corrections and improvements (see: https://github.com/Slookeur/atomes-GNU/releases/tag/v1.1.17)

* Wed Dec 18 2024 Sébastien Le Roux <sebastien.leroux@ipcms.unistra.fr> - 1.1.16-1
- Bug corrections

* Mon Oct 14 2024 Sébastien Le Roux <sebastien.leroux@ipcms.unistra.fr> - 1.1.15-3
- Bug correction

* Thu Oct 10 2024 Sébastien Le Roux <sebastien.leroux@ipcms.unistra.fr> - 1.1.15-2
- Package correction

* Fri Oct 04 2024 Sébastien Le Roux <sebastien.leroux@ipcms.unistra.fr> - 1.1.15-1
- Bug corrections and improvements (see: https://github.com/Slookeur/atomes-GNU/releases/tag/v1.1.15)

* Mon Sep 23 2024 Fabio Valentini <decathorpe@gmail.com> - 1.1.14-4
- Rebuild for ffmpeg 7

* Wed Jul 17 2024 Fedora Release Engineering <releng@fedoraproject.org> - 1.1.14-3
- Rebuilt for https://fedoraproject.org/wiki/Fedora_41_Mass_Rebuild

* Wed Mar 20 2024 Sébastien Le Roux <sebastien.leroux@ipcms.unistra.fr> - 1.1.14-2
- Bug corrections and improvements (see: https://github.com/Slookeur/atomes-GNU/releases/tag/v1.1.14)

* Wed Mar 20 2024 Sébastien Le Roux <sebastien.leroux@ipcms.unistra.fr> - 1.1.14-1
- Bug corrections and improvements (see: https://github.com/Slookeur/atomes-GNU/releases/tag/v1.1.14)

* Wed Feb 28 2024 Sébastien Le Roux <sebastien.leroux@ipcms.unistra.fr> - 1.1.13-1
- Bug corrections and improvements (see: https://github.com/Slookeur/atomes-GNU/releases/tag/v1.1.13)

* Mon Sep 18 2023 Sébastien Le Roux <sebastien.leroux@ipcms.unistra.fr> - 1.1.12-2
- Package metadata update

* Mon Sep 11 2023 Sébastien Le Roux <sebastien.leroux@ipcms.unistra.fr> - 1.1.12-1
- Several bug corrections and improvements (see: https://github.com/Slookeur/atomes-GNU/releases/tag/v1.1.12)

* Wed Jul 19 2023 Fedora Release Engineering <releng@fedoraproject.org> - 1.1.11-9
- Rebuilt for https://fedoraproject.org/wiki/Fedora_39_Mass_Rebuild

* Fri Apr 14 2023 Sébastien Le Roux <sebastien.leroux@ipcms.unistra.fr> - 1.1.11-8
- Initial Fedora commit
