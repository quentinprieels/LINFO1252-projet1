#!/bin/bash
pdflatex -interaction=nonstopmode -shell-escape main.tex
rm -r *.aux *.log *.out *.pyg
