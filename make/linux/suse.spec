#
# spec file for package libebml for (at least) SuSE Linux 9.0, 9.1
#
# Copyright (c) 2004 SUSE LINUX AG, Nuernberg, Germany.
# This file and all modifications and additions to the pristine
# package are under the same license as the package itself.
#
# Please submit bugfixes or comments via http://www.suse.de/feedback/
#

# neededforbuild  gcc-c++ libstdc++-devel

BuildRequires: bzip2 cpp libebml make patch tar zlib zlib-devel binutils gcc gcc-c++ libstdc++-devel perl rpm

Name:         libebml
URL:          http://sourceforge.net/projects/ebml
Version: 0.7.8
Release: 1
Summary:      libary to parse EBML files.
License:      LGPL
Group:        Development/Libraries/Other
Source:       %{name}-%{version}.tar.bz2
Patch:        suse-libebml-makefile.diff
Summary:      libary to parse EBML files.
BuildRoot:    %{_tmppath}/%{name}-%{version}-build
Prefix:       /usr

%description
libebml is a C++ libary to parse EBML files. See the EBML RFV at
http://www.matroska.org/technical/specs/rfc/



Authors:
--------
    Steve Lhomme <steve.lhomme@free.fr>
    Moritz Bunkus <moritz@bunkus.org>

%prep
rm -rf $RPM_BUILD_ROOT
%setup
%patch -p1

%build
export CFLAGS="$RPM_OPT_FLAGS"
cd make/linux
make PREFIX=$RPM_BUILD_ROOT/usr LIBDIR=%{_libdir} staticlib

%install
cd make/linux
make install_staticlib install_headers PREFIX=$RPM_BUILD_ROOT/usr LIBDIR=$RPM_BUILD_ROOT/%{_libdir}

%clean
rm -rf $RPM_BUILD_ROOT

%post
%run_ldconfig

%postun
%run_ldconfig

%files
%defattr (-,root,root)
%{_libdir}/libebml.a
/usr/include/ebml

%changelog -n libebml
* Sat Apr 16 2005 - moritz@bunkus.org
- modified for the new libebml build targets
* Wed Sep 01 2004 - seife@suse.de
- initial submission
