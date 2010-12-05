# airsched doc sub-directory
# StdAir
#include $(top_srcdir)/stdair/basic/sources.mk
#include $(top_srcdir)/stdair/bom/sources.mk
#include $(top_srcdir)/stdair/factory/sources.mk
#include $(top_srcdir)/stdair/command/sources.mk
#include $(top_srcdir)/stdair/service/sources.mk
#include $(top_srcdir)/stdair/core/sources.mk
# Air-Sched
include $(top_srcdir)/airsched/basic/sources.mk
include $(top_srcdir)/airsched/bom/sources.mk
include $(top_srcdir)/airsched/factory/sources.mk
include $(top_srcdir)/airsched/command/sources.mk
include $(top_srcdir)/airsched/service/sources.mk
include $(top_srcdir)/airsched/core/sources.mk
include $(top_srcdir)/airsched/batches/sources.mk
#
include $(top_srcdir)/doc/local/sources.mk
include $(top_srcdir)/doc/tutorial/sources.mk
include $(top_srcdir)/doc/tutorial/src/sources.mk

SUBDIRS = images tutorial local

docdir = @docdir@

html_tarname = @PACKAGE_TARNAME@-doc-@PACKAGE_VERSION@
pdf_tarname = @PACKAGE_TARNAME@-pdf-@PACKAGE_VERSION@

noinst_DATA =
EXTRA_DIST = $(noinst_DATA)

# Targets
all-local: html-local

html-local: html/index.html

html/index.html: doxygen_html.cfg \
	$(doc_local_sources) $(html_local_sources) \
	$(doc_tutorial_sources) $(cpp_tutorial_sources) \
	$(airsched_batches_h_sources) $(airsched_batches_cc_sources) \
	$(airsched_service_h_sources) $(airsched_service_cc_sources) \
	$(airsched_bas_h_sources) $(airsched_bas_cc_sources) \
	$(airsched_bom_h_sources) $(airsched_bom_cc_sources) \
	$(airsched_fac_h_sources) $(airsched_fac_cc_sources) \
	$(airsched_dba_h_sources) $(airsched_dba_cc_sources) \
	$(airsched_cmd_h_sources) $(airsched_cmd_cc_sources) \
	$(airsched_svc_h_sources) $(airsched_svc_cc_sources)
	doxygen $<; \
	if test -d html; then \
		cp $(srcdir)/images/airsched_logo.png html; \
		cp $(srcdir)/images/sfx_logo.png html; \
		cp $(srcdir)/images/favicon.ico html; \
	fi

# 	$(stdair_batches_h_sources) $(stdair_batches_cc_sources) \
#	$(stdair_service_h_sources) $(stdair_service_cc_sources) \
#	$(stdair_bas_h_sources) $(stdair_bas_cc_sources) \
#	$(stdair_bom_h_sources) $(stdair_bom_cc_sources) \
#	$(stdair_fac_h_sources) $(stdair_fac_cc_sources) \
#	$(stdair_dba_h_sources) $(stdair_dba_cc_sources) \
#	$(stdair_cmd_h_sources) $(stdair_cmd_cc_sources) \
#	$(stdair_svc_h_sources) $(stdair_svc_cc_sources) \
#

dist-html: html-local
	cp -a $(top_builddir)/doc/html $(top_builddir)/$(html_tarname)
	tar chof - $(top_builddir)/$(html_tarname) | \
		gzip --best -c > $(top_builddir)/$(html_tarname).tar.gz
	tar chof - $(top_builddir)/$(html_tarname) | \
		bzip2 -9 -c > $(top_builddir)/$(html_tarname).tar.bz2
	rm -rf $(top_builddir)/$(html_tarname)

dist-pdf: html-local
	cp -a $(top_builddir)/doc/latex $(top_builddir)/$(pdf_tarname)
	tar chof - $(top_builddir)/$(pdf_tarname) | \
		gzip --best -c > $(top_builddir)/$(pdf_tarname).tar.gz
	tar chof - $(top_builddir)/$(pdf_tarname) | \
		bzip2 -9 -c > $(top_builddir)/$(pdf_tarname).tar.bz2
	rm -rf $(top_builddir)/$(pdf_tarname)

install-data-local: html-local
	$(mkinstalldirs) $(DESTDIR)$(docdir)
	if test -d html; then \
		$(mkinstalldirs) $(DESTDIR)$(docdir)/html; \
		for f in html/*; do \
			$(INSTALL_DATA) $$f $(DESTDIR)$(docdir)/html; \
		done \
	fi
	$(mkinstalldirs) $(DESTDIR)$(docdir)
	if test -d latex; then \
		$(mkinstalldirs) $(DESTDIR)$(docdir)/latex; \
		for f in latex/*; do \
			$(INSTALL_DATA) $$f $(DESTDIR)$(docdir)/latex; \
		done \
	fi

uninstall-local:
	rm -rf $(DESTDIR)$(docdir)

clean-local:
	rm -rf html latex *.log *.tag
