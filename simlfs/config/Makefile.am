# config
include $(top_srcdir)/Makefile.common

## Source directory

DISTCLEANFILES = simlfs-paths.hpp

MAINTAINERCLEANFILES = Makefile.in

EXTRA_DIST = simlfs-paths.hpp

# Targets
all-local: simlfs-paths.hpp

simlfs-paths.hpp: Makefile
	@echo '#ifndef __SIMLFS_PATHS_HPP' > $@
	@echo '#define __SIMLFS_PATHS_HPP' >> $@
	@echo '#define PACKAGE "simlfs"' >> $@
	@echo '#define PACKAGE_NAME "SIMLFS"' >> $@
	@echo '#define PACKAGE_VERSION "@VERSION@"' >> $@
	@echo '#define PREFIXDIR "$(prefix)"' >> $@
	@echo '#define BINDIR "$(bindir)"' >> $@
	@echo '#define LIBEXECDIR "$(libexecdir)"' >> $@
	@echo '#define DATADIR "$(datadir)"' >> $@
	@echo '#define DOCDIR "$(docdir)"' >> $@
	@echo '#endif // __SIMLFS_PATHS_HPP' >> $@
