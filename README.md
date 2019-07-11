
Summary:
--------
The Travel Market Simulator project aims at providing reference implementation,
mainly in C++, of a travel market simulator, focusing on revenue management (RM)
for airlines. It is intended to be used for applied research activities only:
it is by no way intended to be used by production systems. It is a new breed of
software and aims to become the new generation PODS (http://podsresearch.com/),
which was instrumental in the inception of the Travel Market Simulator project.

Over a dozen components have been implemented and are fully functional,
encompassing for instance (but not limited to) traveller demand generation
(booking requests), travel distribution (GDS/CRS), low fare search (LFS),
price calculation and inventory availability calculation), customer choice
modelling (CCM), revenue management (RM), schedule and inventory management,
revenue accounting (RA).

The Travel Market Simulator can used in either batch or hosted mode. It is
the main component of the Travel Market Simulator:
http://www.travel-market-simulator

That project makes an extensive use of existing open-source libraries for
increased functionality, speed and accuracy. In particular the Boost (C++
Standard Extensions: http://www.boost.org) library is used.

The TvlSim component itself aims at providing a clean API and a simple
implementation, as a C++ library, of a travel market simulator, focusing
on revenue management (RM) for airlines. That library uses the Standard
Airline IT C++ object model (http://sf.net/projects/stdair).


Getting and installing from the Fedora/CentOS/RedHat distribution:
------------------------------------------------------------------
Just use DNF:
```bash
dnf -y install tvlsim-devel tvlsim-doc
```

You can also get the RPM packages (which may work on Linux
distributions like Novel Suse and Mandriva) from the Fedora repository
(_e.g._, for Fedora 30, 
http://fr2.rpmfind.net/linux/fedora/releases/30/Everything/)


Building the library and test binary from Git repository:
----------------------------------------------------------------
The Git repository may be cloned as following:
```bash
$ git clone git@github.com:airsim/tvlsim.git tvlsimgit # through SSH
$ git clone https://github.com/airsim/tvlsim.git # if the firewall filters SSH
cd tvlsimgit
git checkout trunk
```

Then, you need the following packages (Fedora/RedHat/CentOS names here, 
but names may vary according to distributions):
* cmake
* gcc-c++
* boost-devel / libboost-dev
* python-devel / python-dev
* gettext-devel / gettext-dev
* sqlite3-devel / libsqlite3-dev
* readline-devel / readline-dev
* ncurses-devel
* soci-mysql-devel, soci-sqlite3-devel
* stdair-devel / libstdair-dev
* sevmgr-devel / libsevmgr-dev
* airrac-devel / libairrac-dev
* rmol-devel / librmol-dev
* airinv-devel / libairinv-dev
* simfqt-devel / libsimfqt-dev
* airtsp-devel / libairtsp-dev
* doxygen, ghostscript, graphviz
* tetex-latex (optional)
* rpm-build (optional)

Building the library and test binary from the tarball:
------------------------------------------------------
The latest stable source tarball (`tvlsim*.tar.gz` or `.bz2`)
can be found here:
https://github.com/airsim/tvlsim/releases

To customise the following to your environment, you can alter the path
to the installation directory:
```bash
export INSTALL_BASEDIR=/home/user/dev/deliveries
export TVLSIM_VER=99.99.99
if [ -d /usr/lib64 ]; then LIBSUFFIX=64; fi
export LIBSUFFIX_4_CMAKE="-DLIB_SUFFIX=$LIBSUFFIX"
```

Then, as usual:
* To configure the project, type something like:
```bash
  mkdir build && cd build
  cmake -DCMAKE_INSTALL_PREFIX=${INSTALL_BASEDIR}/tvlsim-$TVLSIM_VER \
   -DWITH_STDAIR_PREFIX=${INSTALL_BASEDIR}/stdair-stable \
   -DWITH_TRADEMGEN_PREFIX=${INSTALL_BASEDIR}/trademgen-stable \
   -DWITH_SEVMGR_PREFIX=${INSTALL_BASEDIR}/sevmgr-stable \
   -DWITH_TRAVELCCM_PREFIX=${INSTALL_BASEDIR}/travelccm-stable \
   -DWITH_AIRTSP_PREFIX=${INSTALL_BASEDIR}/airtsp-stable \
   -DWITH_AIRRAC_PREFIX=${INSTALL_BASEDIR}/airrac-stable \
   -DWITH_RMOL_PREFIX=${INSTALL_BASEDIR}/rmol-stable \
   -DWITH_AIRINV_PREFIX=${INSTALL_BASEDIR}/airinv-stable \
   -DWITH_SIMFQT_PREFIX=${INSTALL_BASEDIR}/simfqt-stable \
   -DWITH_SIMCRS_PREFIX=${INSTALL_BASEDIR}/simcrs-stable \
   -DCMAKE_BUILD_TYPE:STRING=Debug -DENABLE_TEST:BOOL=ON -DINSTALL_DOC:BOOL=ON \
   -DRUN_GCOV:BOOL=OFF ${LIBSUFFIX_4_CMAKE} ..
```
* To build the project, type:
```bash
  make
```
* To test the project, type:
```bash 
 make check
```
* To install the library (`libtvlsim*.so*`) and the binary (`tvlsim`),
  just type:
```bash
  make install
```
* To package the source files, type:
```bash
  make dist
```
* To package the binary and the (HTML and PDF) documentation:
```bash
  make package
```
* To browse the (just installed, if enabled) HTML documentation:
```bash
  midori file://${INSTALL_BASEDIR}/tvlsim-$TVLSIM_VER/share/doc/tvlsim/html/index.html
```
* To browse the (just installed, if enabled) PDF documentation:
```bash
  evince ${INSTALL_BASEDIR}/tvlsim-$TVLSIM_VER/share/doc/tvlsim/html/refman.pdf
```
* To run the local binary version:
```bash
  ./tvlsim/tvlsim -b
```
* To run the installed version:
```bash
  ${INSTALL_BASEDIR}/tvlsim-$TVLSIM_VER/bin/tvlsim -b
```

`simulate` binary:
------------------
The `simulate` executable requires a MySQL/MariaDB relational database.
Also, the StdAir package contains helper scripts in order to create:
* a database user, namely `tvlsim`;
* as well as a database, namely `sim_tvlsim`.
Some data need to be loaded into a specific table, namely `airlines`.

Following is a sequence of commands allowing to initialise the MySQL database
correctly:
```bash
$ /usr/libexec/stdair/create_tvlsim_user.sh
# Then, enter the credentials of an admin MySQL account.
# Launch with the '-h' option to see how to change the hostname and/or port.
$ /usr/libexec/stdair/create_tvlsim_db.sh
# Then, enter the credentials of an admin MySQL account.
# Launch with the '-h' option to see how to change the hostname and/or port.
$ /usr/libexec/stdair/load_tvlsim_data.sh
# Launch with the '-h' option to see how to change the hostname and/or port.
$ simulate
$ less simulate.log
```

`TvlSimServer` binary:
----------------------
Same as for the `simulate` executable.
A command-line (test) client exists in Python
(`appserver/django/tvlsimClient.py`).
```bash
$ TvlSimServer &
$ wget https://raw.github.com/airsim/tvlsim/trunk/appserver/django/tvlsimClient.py
$ python tvlsimClient.py
$ kill %1
$ less tvlsimServer.log
```

Note that a Django-based Web application server also exists,
but it has not been packaged yet.


Denis Arnaud (June 2015)

