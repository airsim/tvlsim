# 
# GNU ReadLine examples
#
# See:
# * http://sunsite.ualberta.ca/Documentation/Gnu/readline-4.1/html_node/readline_45.html
# * http://cc.byexamples.com/2008/06/16/gnu-readline-implement-custom-auto-complete/

STD_RDL_PRG = readline_autocomp
STD_RDL_SRC = ${STD_RDL_PRG:%=%.c}

SPE_RDL_PRG = readline_autocomp_altered
SPE_RDL_SRC = ${SPE_RDL_PRG:%=%.cpp}

ALL_PRG = $(STD_RDL_PRG) $(SPE_RDL_PRG)

LDFLAGS = -lreadline

all: $(ALL_PRG)

%.c%.o: %.c
	@echo "Compiling '$<'"
	gcc -Wall -fPIC -o $@ $<

%.cpp%.o: %.cpp
	@echo "Compiling '$<'"
	g++ -Wall -fPIC -o $@ $<


$(STD_RDL_PRG): $(STD_RDL_SRC)
	@echo "Compiling '$<'" && \
	gcc -g $(LDFLAGS) -o $@ $< && \
	echo "./$@ ready to be used"

$(SPE_RDL_PRG): $(SPE_RDL_SRC)
	@echo "Compiling '$<'" && \
	g++ -g $(LDFLAGS) -o $@ $< && \
	echo "./$@ ready to be used"

clean:
	rm -f *.o $(ALL_PRG)

