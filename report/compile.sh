#!/bin/bash
pdflatex -interaction=nonstopmode -shell-escape -output-directory report/ report/main.tex
rm -r report/*.aux report/*.log report/*.out
