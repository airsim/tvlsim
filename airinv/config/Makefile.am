# airinv/config
include $(top_srcdir)/Makefile.common

## Source directory

DISTCLEANFILES = airinv-paths.hpp

MAINTAINERCLEANFILES = Makefile.in

EXTRA_DIST = airinv-paths.hpp

# Targets
all-local: airinv-paths.hpp

airinv-paths.hpp: Makefile
	@echo '#ifndef __AIRINV_PATHS_HPP' > $@
	@echo '#define __AIRINV_PATHS_HPP' >> $@
	@echo >> $@
	@echo '#define PACKAGE "airinv"' >> $@
	@echo '#define PACKAGE_NAME "AIRINV"' >> $@
	@echo '#define PACKAGE_VERSION "@VERSION@"' >> $@
	@echo '#define PREFIXDIR "$(prefix)"' >> $@
	@echo '#define BINDIR "$(bindir)"' >> $@
	@echo '#define LIBEXECDIR "$(libexecdir)"' >> $@
	@echo '#define DATADIR "$(datadir)"' >> $@
	@echo '#define DOCDIR "$(docdir)"' >> $@
	@echo '#define STDAIR_SAMPLE_DIR "$(STDAIR_SAMPLE_DIR)"' >> $@
	@echo >> $@
	@echo '#endif // __AIRINV_PATHS_HPP' >> $@
	@echo >> $@

