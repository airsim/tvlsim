# Prototype of a distributed simulator

all: tvlsim

binaryProtocol.cmx: binaryProtocol.ml
	ocamlfind ocamlopt -inline 100 \
     -package bitstring,bitstring.syntax -syntax bitstring.syntax \
     -linkpkg $^ -c

%.cmx: %.ml
	ocamlfind ocamlopt -package bitstring -inline 100 -c $^


tvlsim: types.cmx agents.cmx simulation.cmx nutshell.cmx
	ocamlfind ocamlopt -inline 100 -package bitstring -linkpkg $^ -o $@

clean: 
	rm *.cm*
