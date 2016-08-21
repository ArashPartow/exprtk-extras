/*
 **************************************************************
 *         C++ Mathematical Expression Toolkit Library        *
 *                                                            *
 * ExprTk Vector Processing Benchmark                         *
 * Author: Arash Partow (1999-2016)                           *
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
#include <fstream>
#include <limits>
#include <string>
#include <vector>

#include "exprtk.hpp"


const std::string global_expression_list[] =
                  {
                     "2v0",
                     "2v0 + 3",
                     "v0 + v1",
                     "2v0 + 3v1",
                     "2v0 - v1 / 3",
                     "v0 * v1 / v2",
                     "2(v0 * v1 / v2)",
                     "2(v0 / 3 + v1 / 4)",
                     "(2v0 - v1 / v2)",
                     "3(2v0 - v1 / v2)",
                     "7(5v0 * 3v1 / 2v2)",
                     "sum(2v0)",
                     "sum(2v0 + 3)",
                     "sum(v0 + v1)",
                     "sum(2v0 + 3v1)",
                     "sum(v0^2.2 + v1^3.3)",
                     "sum((2v0 - v1 / v2))",
                     "sum(3(2v0 - v1 / v2))",
                     "sum(abs(v0 * v1) / v2)",
                     "sum(v0 + v1) + avg(v0 - v1)",
                     "7(sum(abs(5v0 * 3v1) / 2v2))",
                     "sum(2v0 + 3v1) + sum(4 / v0 - 5 / v1)"
                  };

const std::size_t global_expression_list_size = sizeof(global_expression_list) / sizeof(std::string);

const std::size_t rounds = 2000;

typedef double numeric_type;

template <typename T>
void run_expression_benchmark(const std::size_t vec_size, const std::string& expr_string)
{
   typedef exprtk::symbol_table<numeric_type> symbol_table_t;
   typedef exprtk::expression<numeric_type>     expression_t;
   typedef exprtk::parser<numeric_type>             parser_t;

   std::vector<T> v0(vec_size, T(2.1234567890));
   std::vector<T> v1(vec_size, T(3.1234567890));
   std::vector<T> v2(vec_size, T(5.1234567890));

   symbol_table_t symbol_table;

   symbol_table.add_vector("v0",v0);
   symbol_table.add_vector("v1",v1);
   symbol_table.add_vector("v2",v2);

   expression_t expression;
   expression.register_symbol_table(symbol_table);

   parser_t parser;

   if (!parser.compile(expr_string,expression))
   {
      printf("[load_expression] - Parser Error: %s\tExpression: %s\n",
             parser.error().c_str(),
             expr_string.c_str());

      return;
   }

   exprtk::timer timer;

   T total = T(0);

   timer.start();

   for (std::size_t r = 0; r < rounds; ++r)
   {
      total += expression.value();
   }

   timer.stop();

   if (T(0) != total)
      printf("Total Time:%12.8f  Rate:%14.3fevals/sec Expression: %s\n",
             timer.time(),
             rounds / timer.time(),
             expr_string.c_str());
   else
      printf("run_expression_benchmark() - Error running benchmark for expression: %s\n",
             expr_string.c_str());
}

template <typename T>
void run_benchmark(const std::size_t& vec_size)
{
   for (std::size_t i = 0; i < global_expression_list_size; ++i)
   {
      run_expression_benchmark<T>(vec_size,global_expression_list[i]);
   }
}

template <typename T>
void run_file_benchmark(const std::size_t& vec_size, const std::string& file_name)
{
   std::ifstream stream(file_name.c_str());

   if (!stream)
   {
      printf("run_file_benchmark() - Failed to open file: %s\n",
             file_name.c_str());
      return;
   }

   std::string line;
   std::vector<std::string> expr_list;

   while(std::getline(stream, line))
   {
      if (line.empty())
         continue;
      else if ('#' == line[0])
         continue;

      expr_list.push_back(line);
   }

   for (std::size_t i = 0; i < expr_list.size(); ++i)
   {
      run_expression_benchmark<T>(vec_size,expr_list[i]);
   }
}

int main(int argc, char* argv[])
{
   const std::size_t vec_size  = ((argc >= 2) ? atoi(argv[1]) : 100000);
   const std::string file_name = ((argc == 3) ? argv[2] : ""          );

   switch (argc)
   {
      case 2 : run_benchmark     <numeric_type>(vec_size          ); break;
      case 3 : run_file_benchmark<numeric_type>(vec_size,file_name); break;
   }

   return 0;
}
