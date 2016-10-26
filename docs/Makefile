FILE=IE_doc

all: $(FILE).pdf

.PHONY: clean
clean:
	\rm -f *.aux *.blg *.bbl *.log *.out *.pdf ./workflow.pdf ./test_case.pdf

$(FILE).pdf: $(FILE).tex
	dot -Tpdf ./workflow.dot -o ./workflow.pdf
	dot -Tpdf ./test_case.dot -o ./test_case.pdf
	pdflatex $(FILE)
