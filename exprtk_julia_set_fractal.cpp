/*
 **************************************************************
 *         C++ Mathematical Expression Toolkit Library        *
 *                                                            *
 * ExprTk Julia Set Fractal Generator Example                 *
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
void julia_set_fractal()
{
   typedef exprtk::symbol_table<T>         symbol_table_t;
   typedef exprtk::expression<T>           expression_t;
   typedef exprtk::parser<T>               parser_t;
   typedef exprtk::function_compositor<T>  compositor_t;
   typedef typename compositor_t::function function_t;

   const T width  = T(120);
   const T height = T( 42);

   exprtk::rtl::io::println<T> println;

   symbol_table_t symbol_table;

   symbol_table.add_constant("width"  , width  );
   symbol_table.add_constant("height" , height );
   symbol_table.add_function("println", println);

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

   symbol_table.
      add_function("log_details",
      [](T frame, T c_real, T c_imag) -> T
      {
         printf("ExprTk Julia Set Fractal Animation (Frame: %04lu - c = %6.3f + %6.3fi)\n",
                static_cast<std::size_t>(frame + 1),
                c_real,
                c_imag);
         return T(1);
      });

   symbol_table.
      add_function("putch",
      [](T v) -> T
      {
          static const char palette[]    = "........,'`^\":;-_~=+<>!?|\\/()[]"
                                           "{}1iIljtfxrnuvczmwqpdbkhaoGXYUJCL"
                                           "Q0OZ*#MW&8%@$";
          static const int  palette_size = sizeof(palette) / sizeof(char) - 1;
          int index = static_cast<int>(v);
          if (index < 0) index = 0;
          if (index >= palette_size) index = palette_size - 1;
          printf("%c", palette[index]);
          return T(1);
      });

   compositor_t compositor(symbol_table);

   compositor.load_variables(true);
   compositor.load_vectors  (true);

   compositor.add(
      function_t("julia_set_fractal")
      .vars("c_real", "c_imag")
      .expression
      (
         " var imag_max := +1.5;                               "
         " var imag_min := -1.5;                               "
         " var real_max := +1.5;                               "
         " var real_min := -1.5;                               "
         " var x_step   := (real_max - real_min) / width;      "
         " var y_step   := (imag_max - imag_min) / height;     "
         "                                                     "
         " for (var y := 0; y < height; y += 1)                "
         " {                                                   "
         "    var imag := imag_min + (y_step * y);             "
         "                                                     "
         "    for (var x := 0; x < width; x += 1)              "
         "    {                                                "
         "       var real       := real_min + x_step * x;      "
         "       var z_real     := real;                       "
         "       var z_imag     := imag;                       "
         "       var plot_value := 0;                          "
         "                                                     "
         "       for (var n := 0; n < 50; n += 1)              "
         "       {                                             "
         "          var a := z_real^2;                         "
         "          var b := z_imag^2;                         "
         "          plot_value := n;                           "
         "                                                     "
         "          if ((a + b) < 4)                           "
         "          {                                          "
         "             z_imag := 2 * z_real * z_imag + c_imag; "
         "             z_real := a - b + c_real;               "
         "          }                                          "
         "          else                                       "
         "             break;                                  "
         "       };                                            "
         "                                                     "
         "       putch(plot_value);                            "
         "    };                                               "
         "                                                     "
         "    println()                                        "
         " }                                                   "
      ));

   const std::string julia_set_fractal_driver =
      " var t := 0;                                    "
      " for (var frame := 0; frame < 1000; frame += 1) "
      " {                                              "
      "    var radius := 0.4 + 0.1 * sin(t / 5);       "
      "    var c_real := -0.74543 + radius * cos(t);   "
      "    var c_imag :=  0.27015 + radius * sin(t);   "
      "    t += 0.02;                                  "
      "                                                "
      "    clear(frame % 100 == 0);                    "
      "    log_details(frame, c_real, c_imag);         "
      "    julia_set_fractal(c_real, c_imag);          "
      "    sleep(10);                                  "
      " };                                             ";

   expression_t expression;
   expression.register_symbol_table(symbol_table);

   parser_t parser;
   parser.compile(julia_set_fractal_driver,expression);

   expression.value();
}

int main()
{
   julia_set_fractal<double>();
   return 0;
}
