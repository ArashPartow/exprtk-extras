/*
 **************************************************************
 *         C++ Mathematical Expression Toolkit Library        *
 *                                                            *
 * ExprTk Collatz Example                                     *
 * Author: Arash Partow (1999-2022)                           *
 * URL: http://www.partow.net/programming/exprtk/index.html   *
 *                                                            *
 * Copyright notice:                                          *
 * Free use of the Mathematical Expression Toolkit Library is *
 * permitted under the guidelines and in accordance with the  *
 * most current version of the MIT License.                   *
 * http://www.opensource.org/licenses/MIT                     *
 *                                                            *
 **************************************************************
*/


#include <cstdio>
#include <iostream>
#include <string>

#include "exprtk.hpp"


template <typename T>
void collatz()
{
   typedef exprtk::symbol_table<T>         symbol_table_t;
   typedef exprtk::expression<T>           expression_t;
   typedef exprtk::parser<T>               parser_t;
   typedef exprtk::function_compositor<T>  compositor_t;
   typedef typename compositor_t::function function_t;

   exprtk::rtl::io::print<T> print("%3.0f ");

   symbol_table_t symbol_table;
   symbol_table.add_function("print",print);

   compositor_t compositor(symbol_table);

   compositor
      .add(
      function_t( // define function: collatz_trace(x)
           "collatz_trace",
           " while (x > 1)                          "
           " {                                      "
           "   x := if (x % 2 == 0, x / 2, 3x + 1); "
           "   print(x);                            "
           " }                                      ",
           "x"));

   const std::string collatz_program =
                  " x := 0;             "
                  " repeat              "
                  "   print(x += 1);    "
                  "   collatz_trace(x); "
                  "   print('\n\n');    "
                  " until (x > 100);    "
                  " println;            ";

   expression_t expression;
   expression.register_symbol_table(symbol_table);

   parser_t parser;
   parser.enable_unknown_symbol_resolver();
   parser.compile(collatz_program,expression);

   expression.value();
}

int main()
{
   collatz<double>();
   return 0;
}
