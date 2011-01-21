# simcrs/config
include $(top_srcdir)/Makefile.common

## Source directory

DISTCLEANFILES = simcrs-paths.hpp

MAINTAINERCLEANFILES = Makefile.in

EXTRA_DIST = simcrs-paths.hpp

# Targets
all-local: simcrs-paths.hpp

simcrs-paths.hpp: Makefile
	@echo '#ifndef __SIMCRS_PATHS_HPP' > $@
	@echo '#define __SIMCRS_PATHS_HPP' >> $@
	@echo >> $@
	@echo '#define PACKAGE "simcrs"' >> $@
	@echo '#define PACKAGE_NAME "SIMCRS"' >> $@
	@echo '#define PACKAGE_VERSION "@VERSION@"' >> $@
	@echo '#define PREFIXDIR "$(prefix)"' >> $@
	@echo '#define BINDIR "$(bindir)"' >> $@
	@echo '#define LIBEXECDIR "$(libexecdir)"' >> $@
	@echo '#define DATADIR "$(datadir)"' >> $@
	@echo '#define DOCDIR "$(docdir)"' >> $@
	@echo '#define STDAIR_SAMPLE_DIR "$(STDAIR_SAMPLE_DIR)"' >> $@
	@echo >> $@
	@echo '#endif // __SIMCRS_PATHS_HPP' >> $@
	@echo >> $@

