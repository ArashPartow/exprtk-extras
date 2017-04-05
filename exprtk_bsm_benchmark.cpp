/*
 **************************************************************
 *         C++ Mathematical Expression Toolkit Library        *
 *                                                            *
 * ExprTk Black Scholes Merton Benchmark                      *
 * Author: Arash Partow (1999-2017)                           *
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
#include <string>
#include "exprtk.hpp"

static const std::size_t rounds = 10000000;

template <typename T>
struct bsm_parameters
{
   T s;
   T x;
   T t;
   T r;
   T v;
};

bsm_parameters<double> bsm_list[] =
                         {
                            { 60.11, 65.11, 0.25, 0.08,  0.31 },
                            { 60.22, 65.22, 0.25, 0.08,  0.32 },
                            { 60.33, 65.33, 0.25, 0.08,  0.33 },
                            { 60.44, 65.44, 0.25, 0.08,  0.34 },
                            { 60.55, 65.55, 0.25, 0.08,  0.35 },
                            { 60.66, 65.66, 0.25, 0.08,  0.36 },
                            { 60.77, 65.77, 0.25, 0.08,  0.37 },
                            { 60.88, 65.88, 0.25, 0.08,  0.38 },
                            { 60.11, 65.11, 0.25, 0.08,  0.31 },
                            { 60.22, 65.22, 0.25, 0.08,  0.32 },
                            { 60.33, 65.33, 0.25, 0.08,  0.33 },
                            { 60.44, 65.44, 0.25, 0.08,  0.34 },
                            { 60.55, 65.55, 0.25, 0.08,  0.35 },
                            { 60.66, 65.66, 0.25, 0.08,  0.36 },
                            { 60.77, 65.77, 0.25, 0.08,  0.37 },
                            { 60.88, 65.88, 0.25, 0.08,  0.38 }
                         };

const std::size_t bsm_list_size = sizeof (bsm_list) / sizeof(bsm_parameters<double>);

template <typename T>
void black_scholes_merton_model()
{
   typedef exprtk::symbol_table<T> symbol_table_t;
   typedef exprtk::expression<T>     expression_t;
   typedef exprtk::parser<T>             parser_t;

   std::string bsm_model_program =
                  " var d1 := (log(s / x) + (r + v^2 / 2) * t) / (v * sqrt(t)); "
                  " var d2 := d1 - v * sqrt(t);                                 "
                  "                                                             "
                  " if (callput_flag == 'call')                                 "
                  "   s * ncdf(d1) - x * e^(-r * t) * ncdf(d2);                 "
                  " else if (callput_flag == 'put')                             "
                  "   x * e^(-r * t) * ncdf(-d2) - s * ncdf(-d1);               "
                  "                                                             ";

   std::string bsm_model_program_opt =
                  " var v_sqrtt := (v * sqrt(t));                               "
                  " var d1   := (log(s / x) + (r + v * v / 2) * t) / v_sqrtt;   "
                  " var d2   := d1 - v_sqrtt;                                   "
                  " var xert := x * exp(-r * t);                                "
                  "                                                             "
                  " if (callput_flag == 'call')                                 "
                  "   s * ncdf(d1) - xert * ncdf(d2);                           "
                  " else if (callput_flag == 'put')                             "
                  "   xert * ncdf(-d2) - s * ncdf(-d1);                         "
                  "                                                             ";

   T s = T(0);
   T x = T(0);
   T t = T(0);
   T r = T(0);
   T v = T(0);

   std::string callput_flag;

   static const T e = exprtk::details::numeric::constant::e;

   symbol_table_t symbol_table;
   symbol_table.add_variable("s",s);
   symbol_table.add_variable("x",x);
   symbol_table.add_variable("t",t);
   symbol_table.add_variable("r",r);
   symbol_table.add_variable("v",v);
   symbol_table.add_constant("e",e);
   symbol_table.add_stringvar("callput_flag",callput_flag);

   expression_t bsm_expression;
   bsm_expression.register_symbol_table(symbol_table);

   expression_t bsm_expression_opt;
   bsm_expression_opt.register_symbol_table(symbol_table);

   parser_t parser;

   parser.compile(bsm_model_program,    bsm_expression    );
   parser.compile(bsm_model_program_opt,bsm_expression_opt);

   {
      exprtk::timer timer;
      timer.start();

      T total = T(0);

      for (std::size_t k = 0; k < rounds; ++k)
      {
         bsm_parameters<T>& params = bsm_list[k % bsm_list_size];

         s = params.s;
         x = params.x;
         t = params.t;
         r = params.r;
         v = params.v;

         callput_flag = "call";
         total += bsm_expression.value();

         callput_flag = "put";
         total += bsm_expression.value();
      }

      timer.stop();

      printf("[exprtk0] Total: %16.5f\tTime:%8.3fsec\tRate:%16.3f\n",
             total,
             timer.time(),
             (2.0 * rounds) / timer.time());
   }

   {
      exprtk::timer timer;
      timer.start();

      T total = T(0);

      for (std::size_t k = 0; k < rounds; ++k)
      {
         bsm_parameters<T>& params = bsm_list[k % bsm_list_size];

         s = params.s;
         x = params.x;
         t = params.t;
         r = params.r;
         v = params.v;

         callput_flag = "call";
         total += bsm_expression_opt.value();

         callput_flag = "put";
         total += bsm_expression_opt.value();
      }

      timer.stop();

      printf("[exprtk1] Total: %16.5f\tTime:%8.3fsec\tRate:%16.3f\n",
             total,
             timer.time(),
             (2.0 * rounds) / timer.time());
   }
}

template <typename T>
inline T bsm_model(const std::string& callput_flag, T s, T x, T t, T r, T v)
{
   T d1 = (std::log(s / x) + (r + (v * v) / 2) * t) / (v * std::sqrt(t));
   T d2 = d1 - v * sqrt(t);

   if (callput_flag == "call")
      return  s * exprtk::details::numeric::ncdf(d1) - x * exp(-r * t) * exprtk::details::numeric::ncdf(d2);
   else if (callput_flag == "put")
      return x * exp(-r * t) * exprtk::details::numeric::ncdf(-d2) - s * exprtk::details::numeric::ncdf(-d1);
   else
      return T(0);
}

template <typename T>
void bsm_native()
{
   T s = T(0);
   T x = T(0);
   T t = T(0);
   T r = T(0);
   T v = T(0);

   std::string callput_flag;

   exprtk::timer timer;
   timer.start();

   T total = T(0);

   for (std::size_t k = 0; k < rounds; ++k)
   {
      bsm_parameters<T>& params = bsm_list[k % bsm_list_size];

      s = params.s;
      x = params.x;
      t = params.t;
      r = params.r;
      v = params.v;

      callput_flag = "call";
      total += bsm_model(callput_flag,s,x,t,r,v);

      callput_flag = "put";
      total += bsm_model(callput_flag,s,x,t,r,v);
   }

   timer.stop();

   printf("[native ] Total: %16.5f\tTime:%8.3fsec\tRate:%16.3f\n",
          total,
          timer.time(),
          (2.0 * rounds) / timer.time());
}

int main()
{
   black_scholes_merton_model<double>();
   bsm_native<double>();
   return 0;
}

/*
   Build command:
   c++ -pedantic-errors -Wall -Wextra -Werror -Wno-long-long -flto -march=native -O3 -o exprtk_bsm_benchmark exprtk_bsm_benchmark.cpp -L/usr/lib -lstdc++ -lm
*/
