#ifndef __FN_LOOKUP__
#define __FN_LOOKUP__

#include <algorithm>

#include "fit/conditional.h"
#include "fit/pipable.h"

#include "detail/trait.h"
#include "detail/macro.h"

#include "maybe.h"

namespace fn {

FIT_STATIC_FUNCTION(lookup) = ::fit::pipable(::fit::conditional(

    // specialization for container lookup with a UnaryPredicate
    [] (auto input, auto func,

        REQUIRES(
            detail::is_container<decltype(input)>(),
            detail::is_function_template<decltype(func)>::value
        )
    ) {
        auto itr = ::std::find_if(
            ::std::begin(input),
            ::std::end(input),
            func
        );
        if (itr != ::std::end(input)) {
            return maybe(*itr);
        }
        else {
            return maybe<
                ::std::remove_cv_t<
                    ::std::remove_reference_t<
                        decltype(*itr)>>
            >();
        }
    },

    // specialization for container lookup with a value
    [] (auto input, auto val,

        REQUIRES(
            detail::is_container<decltype(input)>(),
            !detail::is_function_template<decltype(val)>::value
        )
    ) {
        auto itr = ::std::find(
            ::std::begin(input),
            ::std::end(input),
            val
        );
        if (itr != ::std::end(input)) {
            return maybe(*itr);
        }
        else {
            return maybe<
                ::std::remove_cv_t<
                    ::std::remove_reference_t<
                        decltype(*itr)>>
            >();
        }
    }

));

}

#endif
