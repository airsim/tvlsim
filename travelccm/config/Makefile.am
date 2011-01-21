# travelccm/config
include $(top_srcdir)/Makefile.common

## Source directory
DISTCLEANFILES = travelccm-paths.hpp

MAINTAINERCLEANFILES = Makefile.in

EXTRA_DIST = travelccm-paths.hpp

# Targets
all-local: travelccm-paths.hpp

travelccm-paths.hpp: Makefile
	@echo '#ifndef __TRAVELCCM_PATHS_HPP' > $@
	@echo '#define __TRAVELCCM_PATHS_HPP' >> $@
	@echo >> $@
	@echo '#define PACKAGE "travelccm"' >> $@
	@echo '#define PACKAGE_NAME "TRAVELCCM"' >> $@
	@echo '#define PACKAGE_VERSION "@VERSION@"' >> $@
	@echo '#define PREFIXDIR "$(prefix)"' >> $@
	@echo '#define BINDIR "$(bindir)"' >> $@
	@echo '#define LIBEXECDIR "$(libexecdir)"' >> $@
	@echo '#define DATADIR "$(datadir)"' >> $@
	@echo '#define DOCDIR "$(docdir)"' >> $@
	@echo '#define STDAIR_SAMPLE_DIR "$(STDAIR_SAMPLE_DIR)"' >> $@
	@echo >> $@
	@echo '#endif // __TRAVELCCM_PATHS_HPP' >> $@
	@echo >> $@
