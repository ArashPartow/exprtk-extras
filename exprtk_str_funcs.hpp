/*
 ******************************************************************
 *           C++ Mathematical Expression Toolkit Library          *
 *                                                                *
 * Author: Arash Partow (1999-2017)                              *
 * URL: http://www.partow.net/programming/exprtk/index.html       *
 *                                                                *
 * Copyright notice:                                              *
 * Free use of the C++ Mathematical Expression Toolkit Library is *
 * permitted under the guidelines and in accordance with the most *
 * current version of the Common Public License.                  *
 * http://www.opensource.org/licenses/cpl1.0.php                  *
 *                                                                *
 ******************************************************************
*/


#ifndef INCLUDE_EXPRTK_STR_FUNCS_HPP
#define INCLUDE_EXPRTK_STR_FUNCS_HPP


#include "exprtk.hpp"


namespace exprtk
{
   namespace helper
   {
      namespace details
      {
         template <typename T, typename Process>
         struct igf_impl : public exprtk::igeneric_function<T>
         {
            typedef typename igeneric_function<T>::parameter_list_t parameter_list_t;

            igf_impl(const std::string& param_sequence = "")
            : exprtk::igeneric_function<T>(param_sequence)
            {}

            inline T operator()(parameter_list_t parameters)
            {
               return Process::execute(parameters);
            }
         };

         struct toupper_impl         {};
         struct tolower_impl         {};
         struct trim_leading_ws_impl {};
         struct trim_trailing_ws_impl{};
         struct trim_ws_impl         {};
         struct trim_leading_impl    {};
         struct trim_trailing_impl   {};
         struct trim_impl            {};
         struct sort_impl            {};
         struct remove_impl          {};
         struct removec_impl         {};
         struct to_str_impl          {};
         struct rotate_l_impl        {};
         struct rotate_r_impl        {};
         struct join_impl            {};
         struct alphabet_uc_impl     {};
         struct alphabet_lc_impl     {};
         struct digits_impl          {};
         struct odd_digits_impl      {};
         struct even_digits_impl     {};
      }
   }
} // namespace exprtk

#endif
