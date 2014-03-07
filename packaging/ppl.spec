Name:           ppl
Version:        1.1
Release:        0
License:        GPL-3.0+
Summary:        The Parma Polyhedra Library
Url:            http://www.cs.unipr.it/ppl/
Group:          Development/Libraries/C and C++
Source:         ppl-%{version}.tar.bz2
Source1:        baselibs.conf
Source1001: 	ppl.manifest
BuildRequires:  gcc-c++
BuildRequires:  pkgconfig(gmp) >= 5.1

%description
The Parma Polyhedra Library (PPL) is a library for the manipulation of
(not necessarily closed) convex polyhedra and other numerical
abstractions.  The applications of convex polyhedra include program
analysis, optimized compilation, integer and combinatorial optimization
and statistical data-editing.  The Parma Polyhedra Library comes with
several user friendly interfaces, is fully dynamic (available virtual
memory is the only limitation to the dimension of anything), written in
accordance to all the applicable standards, exception-safe, rather
efficient, thoroughly documented, and free software.  This package
provides all what is necessary to run applications using the PPL
through its C and C++ interfaces.

%package devel
Summary:        Development tools for the Parma Polyhedra Library C and C++ interfaces
Group:          Development/Libraries/C and C++
Requires:       pkgconfig(gmp) >= 5.1
Requires:       libppl = %{version}
Requires:       libppl_c = %{version}

%description devel
The header files, Autoconf macro, and ppl-config tool for developing
applications using the Parma Polyhedra Library through its C and C++
interfaces.

%package devel-static
Summary:        Static libraries for the Parma Polyhedra Library C and C++ interfaces
Group:          Development/Libraries/C and C++
Requires:       ppl-devel = %{version}

%description devel-static
Static libraries for the Parma Polyhedra Library C and C++ interfaces.

%package -n libppl
Summary:        Shared library for the Parma Polyhedra Library
Group:          Development/Libraries/C and C++

%description -n libppl
Shared library for the Parma Polyhedra Library.

%package -n libppl_c
Summary:        Shared library for the Parma Polyhedra Library C bindings
Group:          Development/Libraries/C and C++

%description -n libppl_c
Shared library for the Parma Polyhedra Library C bindings.

%package doc
Summary:        Documentation for the Parma Polyhedra Library
Group:          Development/Libraries/C and C++
BuildArch:      noarch

%description doc
This package contains all the documentations required by programmers
using the Parma Polyhedra Library (PPL). Install this package if you
want to program with the PPL.

%prep
%setup -q -n ppl-%{version}
cp %{SOURCE1001} .

%build
%reconfigure --enable-shared --with-pic --disable-rpath \
	--disable-watchdog
#sed -i 's|^hardcode_libdir_flag_spec=.*|hardcode_libdir_flag_spec=""|g' libtool
#sed -i 's|^runpath_var=LD_RUN_PATH|runpath_var=DIE_RPATH_DIE|g' libtool
make %{?_smp_mflags}

#cd doc
#make user-html

%install
make DESTDIR=%{buildroot} INSTALL="install -p" install-exec

for DIR in utils src tests interfaces demos m4; do
 make -C ${DIR} DESTDIR=%{buildroot} INSTALL="install -p"  install-data
done

make -C doc DESTDIR=%{buildroot} INSTALL="install -p" install-txt
make -C doc DESTDIR=%{buildroot} INSTALL="install -p" install-man

# We don't build the binary, so delete its manpage
rm -f %{buildroot}%{_mandir}/man1/ppl_lpsol.1

rm -f %{buildroot}%{_datadir}/doc/%{name}/ChangeLog
rm -f %{buildroot}%{_datadir}/doc/%{name}/TODO
rm -f %{buildroot}%{_datadir}/doc/%{name}/gpl.*

%post -n libppl -p /sbin/ldconfig

%postun -n libppl -p /sbin/ldconfig

%post -n libppl_c -p /sbin/ldconfig

%postun -n libppl_c -p /sbin/ldconfig

%files
%manifest %{name}.manifest
%defattr(-,root,root,-)
%dir %{_datadir}/doc/%{name}
%doc %{_datadir}/doc/%{name}/BUGS
%doc %{_datadir}/doc/%{name}/COPYING
%doc %{_datadir}/doc/%{name}/CREDITS
%doc %{_datadir}/doc/%{name}/NEWS
%doc %{_datadir}/doc/%{name}/README
%{_bindir}/ppl_lcdd
%{_bindir}/ppl_pips
%{_mandir}/man1/ppl_lcdd.1.gz
%{_mandir}/man1/ppl_pips.1.gz

%files -n libppl
%manifest %{name}.manifest
%defattr(-,root,root,-)
%{_libdir}/libppl.so.*

%files -n libppl_c
%manifest %{name}.manifest
%defattr(-,root,root,-)
%{_libdir}/libppl_c.so.*

%files devel
%manifest %{name}.manifest
%defattr(-,root,root,-)
%doc %{_datadir}/doc/%{name}/README.configure
%{_includedir}/ppl.hh
%{_includedir}/ppl_c.h
%{_libdir}/libppl.so
%{_libdir}/libppl_c.so
%{_bindir}/ppl-config
%{_mandir}/man1/ppl-config.1.gz
%{_mandir}/man3/libppl.3.gz
%{_mandir}/man3/libppl_c.3.gz
%dir %{_datadir}/aclocal
%{_datadir}/aclocal/ppl.m4
%{_datadir}/aclocal/ppl_c.m4

%files devel-static
%manifest %{name}.manifest
%defattr(-,root,root,-)
%{_libdir}/libppl.a
%{_libdir}/libppl.la
%{_libdir}/libppl_c.a
%{_libdir}/libppl_c.la

%files doc
%manifest %{name}.manifest
%defattr(-,root,root,-)
%doc %{_datadir}/doc/%{name}/README.doc
%doc %{_datadir}/doc/%{name}/fdl.txt
