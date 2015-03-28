/*
 **************************************************************
 *         C++ Mathematical Expression Toolkit Library        *
 *                                                            *
 * ExprTk Greatest Common Divisor Example                     *
 * Author: Arash Partow (1999-2015)                           *
 * URL: http://www.partow.net/programming/exprtk/index.html   *
 *                                                            *
 * Copyright notice:                                          *
 * Free use of the Mathematical Expression Toolkit Library is *
 * permitted under the guidelines and in accordance with the  *
 * most current version of the Common Public License.         *
 * http://www.opensource.org/licenses/cpl1.0.php              *
 *                                                            *
 **************************************************************
*/


#include <cstdio>
#include <iostream>
#include <string>

#include "exprtk.hpp"


template <typename T>
struct gcd_println : public exprtk::ifunction<T>
{
   gcd_println() : exprtk::ifunction<T>(3) {}

   inline T operator()(const T& x, const T& y, const T& z)
   {
      printf("gcd(%2d,%2d) = %2d\n",
             static_cast<int>(x),
             static_cast<int>(y),
             static_cast<int>(z));
      return T(0);
   }
};

template <typename T>
void gcd()
{
   typedef exprtk::symbol_table<T>      symbol_table_t;
   typedef exprtk::expression<T>          expression_t;
   typedef exprtk::parser<T>                  parser_t;
   typedef exprtk::function_compositor<T> compositor_t;

   symbol_table_t symbol_table;

   gcd_println<T> prtln;

   symbol_table.add_function("println",prtln);

   compositor_t compositor(symbol_table);

   compositor
      .add("gcd",
           "switch                        "
           "{                             "
           "  case 0 = x : 0;             "
           "  case 0 = y : x;             "
           "  case x = y : x;             "
           "  case x > y : gcd(x - y, y); "
           "  default    : gcd(x, y - x); "
           "}                             ",
           "x","y");

   std::string gcd_program =
                  "i := 0;                    "
                  "while ((i += 1) < 100)     "
                  "{                          "
                  "  j := 0;                  "
                  "  repeat                   "
                  "    println(i,j,gcd(i,j)); "
                  "  until ((j += 1) >= 100); "
                  "};                         ";

   parser_t parser;

   parser.enable_unknown_symbol_resolver();

   expression_t expression;
   expression.register_symbol_table(symbol_table);

   parser.compile(gcd_program,expression);

   expression.value();
}

int main()
{
   gcd<double>();
   return 0;
}
