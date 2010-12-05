#
%global mydocs __tmp_docdir
#
Name:           simcrs
Version:        99.99.99
Release:        1%{?dist}

Summary:        C++ library providing a clean API for a Travel-oriented Distribution System

Group:          System Environment/Libraries 
License:        LGPLv2
URL:            http://%{name}.sourceforge.net
Source0:        http://downloads.sourceforge.net/%{name}/%{name}-%{version}.tar.gz
%{?el5:BuildRoot: %(mktemp -ud %{_tmppath}/%{name}-%{version}-%{release}-XXXXXX)}

BuildRequires:  boost-devel
BuildRequires:  mysql-devel
BuildRequires:  soci-mysql-devel

%description
%{name} aims at providing a clean API and a simple implementation, as a C++
library, of a Travel-oriented Distribution System. It corresponds to the
simulated version of the real-world Computerized Reservation Systems (CRS).

Install the %{name} package if you need a library for simulated CRS C++
C++ library.

%package        devel
Summary:        Header files, libraries and development documentation for %{name}
Group:          Development/Libraries
Requires:       %{name} = %{version}-%{release}
Requires:       pkgconfig

%description    devel
This package contains the header files, static libraries and
development documentation for %{name}. If you would like to develop
programs using %{name}, you will need to install %{name}-devel.

%package doc
Summary:        HTML documentation for the %{name} library
Group:          Documentation
%{?fedora:BuildArch:      noarch}
BuildRequires:  tex(latex)
BuildRequires:  doxygen, ghostscript

%description doc
This package contains the documentation in the HTML format of the %{name}
library. The documentation is the same as at the %{name} web page.


%prep
%setup -q
# The INSTALL package is not relevant for RPM package users
# (e.g., see https://bugzilla.redhat.com/show_bug.cgi?id=489233#c4)
rm -f INSTALL
# Fix some permissions and formats
chmod -x ABOUT-NLS AUTHORS ChangeLog COPYING NEWS README
find . -type f -name '*.[hc]pp' -exec chmod 644 {} \;


%build
%configure --disable-static
make %{?_smp_mflags}

%install
# On Fedora, the BuildRoot is automatically cleaned. Which is not the case for
# RedHat. See: https://fedoraproject.org/wiki/Packaging/Guidelines#BuildRoot_tag
%{?rhel:rm -rf $RPM_BUILD_ROOT}

make install DESTDIR=$RPM_BUILD_ROOT INSTALL="install -p"
%find_lang %{name}
# Remove unpackaged files from the buildroot
rm -f $RPM_BUILD_ROOT%{_includedir}/%{name}/config.h
rm -f $RPM_BUILD_ROOT%{_infodir}/dir
rm -f $RPM_BUILD_ROOT%{_libdir}/lib%{name}.la

mkdir -p %{mydocs}
mv $RPM_BUILD_ROOT%{_docdir}/%{name}-%{version}/html %{mydocs}

%if 0%{?rhel}
%clean
rm -rf $RPM_BUILD_ROOT
%endif

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig


%files -f %{name}.lang
%defattr(-,root,root,-)
%doc AUTHORS ChangeLog COPYING NEWS README
%{_bindir}/%{name}_indexer
%{_bindir}/%{name}_searcher
%{_libdir}/lib*.so.*
%{_mandir}/man3/%{name}.3.*

%files devel
%defattr(-,root,root,-)
%{_includedir}/%{name}
%{_bindir}/%{name}-config
%{_libdir}/lib%{name}.so
%{_libdir}/pkgconfig/%{name}.pc
%{_datadir}/aclocal/%{name}.m4
%{_infodir}/%{name}-ref.info.*
%{_mandir}/man1/%{name}-config.1.*

%files doc
%defattr(-,root,root,-)
%doc %{mydocs}/html
%doc COPYING


%changelog
* Mon Sep 06 2010 Denis Arnaud <denis.arnaud_fedora@m4x.org> 99.99.99-1
- Upstream integration

* Sun Mar 22 2009 Denis Arnaud <denis.arnaud_fedora@m4x.org> 0.2.0-1
- RPM release for Fedora 10

