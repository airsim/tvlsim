# airsched/config
include $(top_srcdir)/Makefile.common

## Source directory
DISTCLEANFILES = airsched-paths.hpp

MAINTAINERCLEANFILES = Makefile.in

EXTRA_DIST = airsched-paths.hpp

# Targets
all-local: airsched-paths.hpp

airsched-paths.hpp: Makefile
	@echo '#ifndef __AIRSCHED_PATHS_HPP' > $@
	@echo '#define __AIRSCHED_PATHS_HPP' >> $@
	@echo >> $@
	@echo '#define PACKAGE "airsched"' >> $@
	@echo '#define PACKAGE_NAME "AIRSCHED"' >> $@
	@echo '#define PACKAGE_VERSION "@VERSION@"' >> $@
	@echo '#define PREFIXDIR "$(prefix)"' >> $@
	@echo '#define BINDIR "$(bindir)"' >> $@
	@echo '#define LIBEXECDIR "$(libexecdir)"' >> $@
	@echo '#define DATADIR "$(datadir)"' >> $@
	@echo '#define DOCDIR "$(docdir)"' >> $@
	@echo '#define STDAIR_SAMPLE_DIR "$(STDAIR_SAMPLE_DIR)"' >> $@
	@echo >> $@
	@echo '#endif // __AIRSCHED_PATHS_HPP' >> $@
	@echo >> $@

