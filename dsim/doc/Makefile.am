include $(top_srcdir)/doc/local/sources.mk
include $(top_srcdir)/doc/tutorial/sources.mk
include $(top_srcdir)/doc/tutorial/src/sources.mk
include $(top_srcdir)/stdair/basic/sources.mk
include $(top_srcdir)/stdair/bom/sources.mk
include $(top_srcdir)/stdair/factory/sources.mk
include $(top_srcdir)/stdair/dbadaptor/sources.mk
include $(top_srcdir)/stdair/command/sources.mk
include $(top_srcdir)/stdair/service/sources.mk
include $(top_srcdir)/stdair/core/sources.mk
include $(top_srcdir)/stdair/batches/sources.mk
#include $(top_srcdir)/trademgen/basic/sources.mk
#include $(top_srcdir)/trademgen/bom/sources.mk
#include $(top_srcdir)/trademgen/factory/sources.mk
#include $(top_srcdir)/trademgen/command/sources.mk
#include $(top_srcdir)/trademgen/service/sources.mk
#include $(top_srcdir)/trademgen/core/sources.mk
#include $(top_srcdir)/trademgen/batches/sources.mk
#include $(top_srcdir)/airsched/basic/sources.mk
#include $(top_srcdir)/airsched/bom/sources.mk
#include $(top_srcdir)/airsched/factory/sources.mk
#include $(top_srcdir)/airsched/command/sources.mk
#include $(top_srcdir)/airsched/service/sources.mk
#include $(top_srcdir)/airsched/core/sources.mk
#include $(top_srcdir)/airsched/batches/sources.mk
#include $(top_srcdir)/airrac/basic/sources.mk
#include $(top_srcdir)/airrac/bom/sources.mk
#include $(top_srcdir)/airrac/factory/sources.mk
#include $(top_srcdir)/airrac/command/sources.mk
#include $(top_srcdir)/airrac/service/sources.mk
#include $(top_srcdir)/airrac/core/sources.mk
include $(top_srcdir)/rmol/basic/sources.mk
include $(top_srcdir)/rmol/field/sources.mk
include $(top_srcdir)/rmol/bom/sources.mk
include $(top_srcdir)/rmol/factory/sources.mk
include $(top_srcdir)/rmol/command/sources.mk
include $(top_srcdir)/rmol/service/sources.mk
include $(top_srcdir)/rmol/core/sources.mk
include $(top_srcdir)/rmol/batches/sources.mk
#include $(top_srcdir)/airinv/basic/sources.mk
#include $(top_srcdir)/airinv/bom/sources.mk
#include $(top_srcdir)/airinv/factory/sources.mk
#include $(top_srcdir)/airinv/command/sources.mk
#include $(top_srcdir)/airinv/service/sources.mk
#include $(top_srcdir)/airinv/core/sources.mk
#include $(top_srcdir)/avlcal/basic/sources.mk
#include $(top_srcdir)/avlcal/bom/sources.mk
#include $(top_srcdir)/avlcal/factory/sources.mk
#include $(top_srcdir)/avlcal/command/sources.mk
#include $(top_srcdir)/avlcal/service/sources.mk
#include $(top_srcdir)/avlcal/core/sources.mk
#include $(top_srcdir)/avlcal/batches/sources.mk
#include $(top_srcdir)/simfqt/basic/sources.mk
#include $(top_srcdir)/simfqt/bom/sources.mk
#include $(top_srcdir)/simfqt/factory/sources.mk
#include $(top_srcdir)/simfqt/command/sources.mk
#include $(top_srcdir)/simfqt/service/sources.mk
#include $(top_srcdir)/simfqt/core/sources.mk
#include $(top_srcdir)/simfqt/batches/sources.mk
#include $(top_srcdir)/simcrs/basic/sources.mk
#include $(top_srcdir)/simcrs/bom/sources.mk
#include $(top_srcdir)/simcrs/factory/sources.mk
#include $(top_srcdir)/simcrs/command/sources.mk
#include $(top_srcdir)/simcrs/service/sources.mk
#include $(top_srcdir)/simcrs/core/sources.mk
#include $(top_srcdir)/simcrs/batches/sources.mk
#include $(top_srcdir)/travelccm/basic/sources.mk
#include $(top_srcdir)/travelccm/bom/sources.mk
#include $(top_srcdir)/travelccm/factory/sources.mk
#include $(top_srcdir)/travelccm/command/sources.mk
#include $(top_srcdir)/travelccm/service/sources.mk
#include $(top_srcdir)/travelccm/core/sources.mk
#include $(top_srcdir)/travelccm/batches/sources.mk
include $(top_srcdir)/dsim/basic/sources.mk
include $(top_srcdir)/dsim/bom/sources.mk
include $(top_srcdir)/dsim/factory/sources.mk
include $(top_srcdir)/dsim/command/sources.mk
include $(top_srcdir)/dsim/service/sources.mk
include $(top_srcdir)/dsim/core/sources.mk
include $(top_srcdir)/dsim/batches/sources.mk

SUBDIRS = images tutorial local

docdir = @docdir@

html_tarname = @PACKAGE_TARNAME@-doc-@PACKAGE_VERSION@

noinst_DATA = sourceforge/howto_release_dsim.html.in
EXTRA_DIST = $(noinst_DATA)

# Targets
all-local: html-local

html-local: html/index.html

html/index.html: doxygen_html.cfg \
	$(doc_local_sources) $(html_local_sources) \
	$(doc_tutorial_sources) $(cpp_tutorial_sources) \
	$(stdair_service_h_sources) $(stdair_service_cc_sources) \
	$(stdair_bas_h_sources) $(stdair_bas_cc_sources) \
	$(stdair_bom_h_sources) $(stdair_bom_cc_sources) \
	$(stdair_fac_h_sources) $(stdair_fac_cc_sources) \
	$(stdair_cmd_h_sources) $(stdair_cmd_cc_sources) \
	$(stdair_svc_h_sources) $(stdair_svc_cc_sources) \
	$(rmol_service_h_sources) $(rmol_service_cc_sources) \
	$(rmol_batches_h_sources) $(rmol_batches_cc_sources) \
	$(rmol_bas_h_sources) $(rmol_bas_cc_sources) \
	$(rmol_fld_h_sources) $(rmol_fld_cc_sources) \
	$(rmol_bom_h_sources) $(rmol_bom_cc_sources) \
	$(rmol_fac_h_sources) $(rmol_fac_cc_sources) \
	$(rmol_cmd_h_sources) $(rmol_cmd_cc_sources) \
	$(rmol_svc_h_sources) $(rmol_svc_cc_sources) \
	$(dsim_service_h_sources) $(dsim_service_cc_sources) \
	$(dsim_bas_h_sources) $(dsim_bas_cc_sources) \
	$(dsim_bom_h_sources) $(dsim_bom_cc_sources) \
	$(dsim_fac_h_sources) $(dsim_fac_cc_sources) \
	$(dsim_cmd_h_sources) $(dsim_cmd_cc_sources) \
	$(dsim_svc_h_sources) $(dsim_svc_cc_sources)
	doxygen $<; \
	if test -d html; then \
		cp $(srcdir)/images/dsim_logo.png html; \
		cp $(srcdir)/images/sfx_logo.png html; \
		cp $(srcdir)/images/favicon.ico html; \
	fi

dist-html: html-local
	cp -a $(top_builddir)/doc/html $(top_builddir)/$(html_tarname)
	tar chof - $(top_builddir)/$(html_tarname) | \
		gzip --best -c > $(top_builddir)/$(html_tarname).tar.gz
	tar chof - $(top_builddir)/$(html_tarname) | \
		bzip2 -9 -c > $(top_builddir)/$(html_tarname).tar.bz2
	rm -rf $(top_builddir)/$(html_tarname)

install-data-local: html-local
	$(mkinstalldirs) $(DESTDIR)$(docdir)
	if test -d html; then \
		$(mkinstalldirs) $(DESTDIR)$(docdir)/html; \
		for f in html/*; do \
			$(INSTALL_DATA) $$f $(DESTDIR)$(docdir)/html; \
		done \
	fi

uninstall-local:
	rm -rf $(DESTDIR)$(docdir)

clean-local:
	rm -rf html *.log *.tag
