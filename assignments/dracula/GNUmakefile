# COMP2521 19t0 ... Fury of Dracula miscellanea
#
# 2017-11-24	Jashank Jeremy <{jashankj,z5017851}@cse.unsw.edu.au>

#PANDOC		?= pandoc
#PANDOC_TMPL	?= dracula.html.pandoc
#PANDOC_FLAGS	+= --smart --standalone --mathjax
#PANDOC_FLAGS	+= --template ${PANDOC_TMPL}

#PAGES		 = index rules data faq view hunt
#HTML_PAGES	:= $(addsuffix .html,${PAGES})

BLOBS		 = view hunt huntIV
BLOBS_ZIP	:= $(addsuffix .zip,${BLOBS})
BLOBS_TAR	:= $(addsuffix .tar,${BLOBS})

#TARGET		= /web/cs2521/19t0/assignments/assn2

########################################################################
# Suffix rules.

%.html: %.markdown ${PANDOC_TMPL}
	${PANDOC} ${PANDOC_FLAGS} -f markdown $< -t html -o $@

.PHONY: %.zip %.tar
%.zip: %
	(cd $< && zip -r ../$@ .)
%.tar: %
	(tar -C $< -chf $@ .)

########################################################################
# Targets.

#all: ${HTML_PAGES}
all: ${BLOBS_ZIP} ${BLOBS_TAR}

check:
	(cd view && ${MAKE})
	(cd dryrun.view && ${MAKE})
	# (cd _test.view && ${MAKE})
	(cd hunt && ${MAKE})

.PHONY: clean
clean:
#	-rm -f ${HTML_PAGES}
	-rm -f ${BLOBS_ZIP} ${BLOBS_TAR}
	-rm -f *~

#.PHONY: sync
#sync:
#	rsync -pau \
#	../scripts/dryrun ../dryrun.view ../dryrun.hunt \
#	Pics ${HTML_PAGES} \
#	${BLOBS_ZIP} ${BLOBS_TAR} \
#	${TARGET}/
