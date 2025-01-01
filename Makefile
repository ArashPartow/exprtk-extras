#
# **************************************************************
# *         C++ Mathematical Expression Toolkit Library        *
# *                                                            *
# * Extra Examples                                             *
# * Author: Arash Partow (1999-2025)                           *
# * URL: https://www.partow.net/programming/exprtk/index.html  *
# *                                                            *
# * Copyright notice:                                          *
# * Free use of the Mathematical Expression Toolkit Library is *
# * permitted under the guidelines and in accordance with the  *
# * most current version of the MIT License.                   *
# * https://www.opensource.org/licenses/MIT                    *
# * SPDX-License-Identifier: MIT                               *
# *                                                            *
# **************************************************************
#


COMPILER         := -c++
#COMPILER        := -clang++
OPTIMIZATION_OPT := -O2 -DNDEBUG
BASE_OPTIONS     = -pedantic-errors -Wall -Wextra -Werror -Wno-long-long
OPTIONS          = $(BASE_OPTIONS) $(OPTIMIZATION_OPT)
LINKER_OPT       = -L/usr/lib -lstdc++ -lm
BUILD_SRC        := $(sort $(wildcard exprtk_*.cpp))
BUILD_LIST       := $(BUILD_SRC:%.cpp=%)

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
