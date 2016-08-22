#
# **************************************************************
# *         C++ Mathematical Expression Toolkit Library        *
# *                                                            *
# * Extra Examples                                             *
# * Author: Arash Partow (1999-2016)                           *
# * URL: http://www.partow.net/programming/exprtk/index.html   *
# *                                                            *
# * Copyright notice:                                          *
# * Free use of the Mathematical Expression Toolkit Library is *
# * permitted under the guidelines and in accordance with the  *
# * most current version of the Common Public License.         *
# * http://www.opensource.org/licenses/cpl1.0.php              *
# *                                                            *
# **************************************************************
#


COMPILER         = -c++
#COMPILER        = -clang
OPTIMIZATION_OPT = -O1
BASE_OPTIONS     = -pedantic-errors -Wall -Wextra -Werror -Wno-long-long
OPTIONS          = $(BASE_OPTIONS) $(OPTIMIZATION_OPT)
LINKER_OPT       = -L/usr/lib -lstdc++ -lm

BUILD_LIST+=exprtk_bsm_benchmark
BUILD_LIST+=exprtk_calc
BUILD_LIST+=exprtk_collatz
BUILD_LIST+=exprtk_fizzbuzz
BUILD_LIST+=exprtk_funcall_benchmark
BUILD_LIST+=exprtk_gcd
BUILD_LIST+=exprtk_gnuplot
BUILD_LIST+=exprtk_gnuplot_multi
BUILD_LIST+=exprtk_mandelbrot
BUILD_LIST+=exprtk_nthroot_bisection.cpp
BUILD_LIST+=exprtk_prime_sieve
BUILD_LIST+=exprtk_repl
BUILD_LIST+=exprtk_sumofprimes
BUILD_LIST+=exprtk_testgen
BUILD_LIST+=exprtk_truthtable_gen
BUILD_LIST+=exprtk_vectornorm
BUILD_LIST+=exprtk_vector_benchmark

all: $(BUILD_LIST)

$(BUILD_LIST) : %: %.cpp exprtk.hpp
	$(COMPILER) $(OPTIONS) -o $@ $@.cpp $(LINKER_OPT)

strip_bin :
	@for f in $(BUILD_LIST); do if [ -f $$f ]; then strip -s $$f; echo $$f; fi done;

valgrind :
	@for f in $(BUILD_LIST); do \
		if [ -f $$f ]; then \
			cmd="valgrind --leak-check=full --show-reachable=yes --track-origins=yes --log-file=$$f.log -v ./$$f"; \
			echo $$cmd; \
			$$cmd; \
		fi done;

clean:
	rm -f core.* *~ *.o *.bak *stackdump gmon.out *.gcda *.gcno *.gcnor *.gch
