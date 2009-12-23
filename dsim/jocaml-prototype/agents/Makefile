# Prototype of a distributed simulator

all: agent

binaryProtocol.cmx: binaryProtocol.ml
	ocamlfind ocamlopt -inline 100 \
     -package bitstring,bitstring.syntax -syntax bitstring.syntax \
     -linkpkg $^ -c

%.cmx: %.ml
	ocamlfind ocamlopt -package mysql -inline 100 -c $^


airline: types.cmx bi dbManager.cmx params.cmx flightQueue.cmx connector.cmx
	ocamlfind ocamlopt -inline 100 -package bitstring -linkpkg $^ -o $@

agent: types.cmx guillotine.cmx connector.cmx
	ocamlfind ocamlopt -package mysql -linkpkg $^ -o $@

clean: 
	rm *.cm*
