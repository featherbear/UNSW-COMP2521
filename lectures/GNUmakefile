LECS		+= week01tue
week01tue_PARTS	 = week01tue-outline week01tue-syntax week01tue-tools week01tue-lls

LECS		+= week01thu
week01thu_PARTS	 = week01thu-adts week01thu-testing

LECS		+= week02tue
week02tue_PARTS	 = week02tue-complexity week02tue-recursion

LECS		+= week02thu
week02thu_PARTS	 = week02thu-recursion tree

LECS		+= week03tue
week03tue_PARTS	 = week03tue-pqueue graph

LECS		+= week03thu
week03thu_PARTS	 = graph

LECS		+= week04thu
week04thu_PARTS	 = graph

LECS		+= week05tue
week05tue_PARTS	 = graph

#LECS		+= week05thu
#week05thu_PARTS = sorting

LECS		+= week06tue
week06tue_PARTS	 = sorting

LECS		+= week06thu
week06thu_PARTS	 = sorting tree

LECS		+= week07tue
week07tue_PARTS	 = tree

LECS		+= week07thu
week07thu_PARTS	 = tree

LECS		+= week08tue
week08tue_PARTS	 = hashing

LECS		+= week08thu
week08thu_PARTS	 = # week08thu-review week08thu-exam

.PHONY: all clean realclean
all: \
	$(addsuffix .slides.pdf,${LECS}) \
	$(addsuffix .a4.pdf,${LECS})
clean:
	rm -f *.aux *.log *.nav *.out *.snm *.toc *.synctex.gz
realclean: clean
	rm -f *.pdf

.PHONY: count
count:
	@texcount -brief \
	$(foreach lec, ${LECS}, ${lec}.tex $(addsuffix .tex,${${lec}_PARTS}))

define lecture
  $(1).slides.pdf:  $(1).tex $$(addsuffix .tex,$${$(1)_PARTS}) lecture.tex GNUmakefile
  $(1).handout.pdf: $(1).tex $$(addsuffix .tex,$${$(1)_PARTS}) lecture.tex GNUmakefile
endef

$(foreach lec, ${LECS}, $(eval $(call lecture,${lec})))

mkslides	= "\documentclass[aspectratio=169]{beamer}\input $(1)"
mkhandout	= "\documentclass[aspectratio=169,handout]{beamer}\input $(1)"
twice		= $(1) && $(1)
thrice		= $(1) && $(1) && $(1)

%.slides.pdf: %.tex
ifndef DRAFT_MODE
	$(call twice,lualatex --draftmode --synctex=1 -jobname $*.slides $(call mkslides,$<))
endif
	lualatex --synctex=1 -jobname $*.slides $(call mkslides,$<)

%.handout.pdf: %.tex
ifndef DRAFT_MODE
	$(call twice,lualatex --draftmode -jobname $*.handout $(call mkhandout,$<))
endif
	lualatex --synctex=1 -jobname $*.handout $(call mkhandout,$<)

%.a4.pdf: %.handout.pdf
	pdfnup --nup 1x3 --a4paper --no-landscape -o $@ -- $<
