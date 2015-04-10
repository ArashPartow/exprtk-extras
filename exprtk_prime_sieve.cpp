/*
 **************************************************************
 *         C++ Mathematical Expression Toolkit Library        *
 *                                                            *
 * ExprTk Sieve Of Eratosthenes                               *
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
void sieve_of_eratosthenes()
{
   typedef exprtk::expression<T> expression_t;
   typedef exprtk::parser<T>         parser_t;

   std::string sieve_of_eratosthenes_program =
                     " var sieve[10^8] := [false];                     "
                     " var m := trunc(sqrt(sieve[]));                  "
                     "                                                 "
                     " sieve[0] := true;                               "
                     " sieve[1] := true;                               "
                     "                                                 "
                     " for (var i := 0; i <= m; i += 1)                "
                     " {                                               "
                     "   if (false == sieve[i])                        "
                     "   {                                             "
                     "     for (var j := (i * i); j < sieve[]; j += i) "
                     "     {                                           "
                     "       sieve[j] := true;                         "
                     "     }                                           "
                     "   }                                             "
                     " };                                              "
                     "                                                 "
                     " var prime_count := 0;                           "
                     " for (var i := 0; i < sieve[]; i += 1)           "
                     " {                                               "
                     "   if (false == sieve[i])                        "
                     "   {                                             "
                     "     prime_count += 1;                           "
                     "   };                                            "
                     " };                                              "
                     "                                                 "
                     " prime_count == 5761455;                         ";

   parser_t parser;

   expression_t expression;

   parser.compile(sieve_of_eratosthenes_program,expression);

   exprtk::timer timer;
   timer.start();

   expression.value();

   timer.stop();

   printf("Total time: %8.3fsec\n",timer.time());
}

int main()
{
   sieve_of_eratosthenes<double>();
   return 0;
}