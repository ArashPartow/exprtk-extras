/*
 **************************************************************
 *         C++ Mathematical Expression Toolkit Library        *
 *                                                            *
 * ExprTk Chladni Contour Graphing Example                    *
 * Author: Arash Partow (1999-2025)                           *
 * URL: https://www.partow.net/programming/exprtk/index.html  *
 *                                                            *
 * Copyright notice:                                          *
 * Free use of the Mathematical Expression Toolkit Library is *
 * permitted under the guidelines and in accordance with the  *
 * most current version of the MIT License.                   *
 * https://www.opensource.org/licenses/MIT                    *
 * SPDX-License-Identifier: MIT                               *
 *                                                            *
 **************************************************************
*/


#include <chrono>
#include <cmath>
#include <cstdio>
#include <string>
#include <thread>

#include "exprtk.hpp"


template <typename T>
void chladni_contour_graphing_example()
{
   typedef exprtk::symbol_table<T>         symbol_table_t;
   typedef exprtk::expression<T>           expression_t;
   typedef exprtk::parser<T>               parser_t;
   typedef exprtk::function_compositor<T>  compositor_t;
   typedef typename compositor_t::function function_t;

   exprtk::rtl::io::println<T> println;
   exprtk::rtl::io::print  <T> print;

   symbol_table_t symbol_table;

   symbol_table.add_function("println", println);
   symbol_table.add_function("print"  , print  );

   symbol_table.
      add_function("sleep",
      [](T time_ms) -> T
      {
         std::this_thread::sleep_for(
         std::chrono::milliseconds(static_cast<std::size_t>(time_ms)));
         return T(1);
      });

   symbol_table.
      add_function("clear",
      [](T full) -> T
      {
         printf("%s\033[H", full == T(1) ? "" : "\033[2J");
         std::fflush(stdout);
         return T(1);
      });

   compositor_t compositor(symbol_table);

   compositor.load_variables(true);
   compositor.load_vectors  (true);

   compositor.add(
      function_t("draw_chladni_contour")
      .var("t")
      .expression
      (
         " var min_x  := -40;                              "
         " var min_y  := -30;                              "
         " var max_x  := +40;                              "
         " var max_y  := +30;                              "
         " var x_incr := +1.0 / 1.7;                       "
         " var y_incr := +1;                               "
         " var a      := 10.0 sin(t);                      "
         " var b      := 10.0 sin(t + 1.0);                "
         " var c      :=  0.9 sin(t);                      "
         "                                                 "
         " for (var y := min_y; y < max_y; y += y_incr)    "
         " {                                               "
         "    for (var x := min_x; x < max_x; x += x_incr) "
         "    {                                            "
         "       if (sin(x / 3 - a) + sin(y / 5 - b) <= c) "
         "          print('*');                            "
         "       else                                      "
         "          print(' ');                            "
         "    };                                           "
         "                                                 "
         "    println();                                   "
         " }                                               "
         "                                                 "
      ));

   const std::string chladni_contour_graphing_driver =
      " var t := 0;                                    "
      " for (var frame := 0; frame < 1000; frame += 1) "
      " {                                              "
      "    t += 0.03;                                  "
      "                                                "
      "    clear(frame % 100 == 0);                    "
      "    draw_chladni_contour(t);                    "
      "    sleep(40);                                  "
      " };                                             ";

   expression_t expression;
   expression.register_symbol_table(symbol_table);

   parser_t parser;
   parser.compile(chladni_contour_graphing_driver,expression);

   expression.value();
}

int main()
{
   chladni_contour_graphing_example<double>();
   return 0;
}
