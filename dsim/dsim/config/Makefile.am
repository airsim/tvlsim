# config
include $(top_srcdir)/Makefile.common

## Source directory

DISTCLEANFILES = dsim-paths.hpp

MAINTAINERCLEANFILES = Makefile.in

EXTRA_DIST = dsim-paths.hpp

# Targets
all-local: dsim-paths.hpp

dsim-paths.hpp: Makefile
	@echo '#ifndef __DSIM_PATHS_HPP' > $@
	@echo '#define __DSIM_PATHS_HPP' >> $@
	@echo '#define PACKAGE "dsim"' >> $@
	@echo '#define PACKAGE_NAME "DSIM"' >> $@
	@echo '#define PACKAGE_VERSION "@VERSION@"' >> $@
	@echo '#define PREFIXDIR "$(prefix)"' >> $@
	@echo '#define BINDIR "$(bindir)"' >> $@
	@echo '#define LIBEXECDIR "$(libexecdir)"' >> $@
	@echo '#define DATADIR "$(datadir)"' >> $@
	@echo '#define DOCDIR "$(docdir)"' >> $@
	@echo '#endif // __DSIM_PATHS_HPP' >> $@
