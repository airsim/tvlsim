# config
include $(top_srcdir)/Makefile.common

## Source directory

DISTCLEANFILES = simfqt-paths.hpp

MAINTAINERCLEANFILES = Makefile.in

EXTRA_DIST = simfqt-paths.hpp

# Targets
all-local: simfqt-paths.hpp

simfqt-paths.hpp: Makefile
	@echo '#ifndef __SIMFQT_PATHS_HPP' > $@
	@echo '#define __SIMFQT_PATHS_HPP' >> $@
	@echo >> $@
	@echo '#define PACKAGE "simfqt"' >> $@
	@echo '#define PACKAGE_NAME "SIMFQT"' >> $@
	@echo '#define PACKAGE_VERSION "@VERSION@"' >> $@
	@echo '#define PREFIXDIR "$(prefix)"' >> $@
	@echo '#define BINDIR "$(bindir)"' >> $@
	@echo '#define LIBEXECDIR "$(libexecdir)"' >> $@
	@echo '#define DATADIR "$(datadir)"' >> $@
	@echo '#define DOCDIR "$(docdir)"' >> $@
	@echo '#define STDAIR_SAMPLE_DIR "$(STDAIR_SAMPLE_DIR)"' >> $@
	@echo >> $@
	@echo '#endif // __SIMFQT_PATHS_HPP' >> $@
	@echo >> $@
