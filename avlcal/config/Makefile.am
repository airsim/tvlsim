# avlcal/config
include $(top_srcdir)/Makefile.common

## Source directory

DISTCLEANFILES = avlcal-paths.hpp

MAINTAINERCLEANFILES = Makefile.in

EXTRA_DIST = avlcal-paths.hpp

# Targets
all-local: avlcal-paths.hpp

avlcal-paths.hpp: Makefile
	@echo '#ifndef __AVLCAL_PATHS_HPP' > $@
	@echo '#define __AVLCAL_PATHS_HPP' >> $@
	@echo >> $@
	@echo '#define PACKAGE "avlcal"' >> $@
	@echo '#define PACKAGE_NAME "AVLCAL"' >> $@
	@echo '#define PACKAGE_VERSION "@VERSION@"' >> $@
	@echo '#define PREFIXDIR "$(prefix)"' >> $@
	@echo '#define BINDIR "$(bindir)"' >> $@
	@echo '#define LIBEXECDIR "$(libexecdir)"' >> $@
	@echo '#define DATADIR "$(datadir)"' >> $@
	@echo '#define DOCDIR "$(docdir)"' >> $@
	@echo '#define STDAIR_SAMPLE_DIR "$(STDAIR_SAMPLE_DIR)"' >> $@
	@echo >> $@
	@echo '#endif // __AVLCAL_PATHS_HPP' >> $@
	@echo >> $@

