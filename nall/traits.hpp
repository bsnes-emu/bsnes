#pragma once

#include <type_traits>
#include <utility>

namespace nall {
  using std::add_const;
  using std::decay;
  using std::declval;
  using std::false_type;
  using std::forward;
  using std::initializer_list;
  using std::is_array;
  using std::is_base_of;
  using std::is_function;
  using std::is_integral;
  using std::is_same;
  using std::move;
  using std::nullptr_t;
  using std::remove_extent;
  using std::remove_reference;
  using std::swap;
  using std::true_type;
}

namespace nall {
  template<bool C> struct expression { static constexpr bool value = C; };
}

namespace nall {
  namespace traits {
    enum class enable_type {};
    enum class disable_type {};

    template<bool C, typename T = void> struct enable_if { using type = T; };
    template<typename T> struct enable_if<false, T> {};

    template<bool C, typename T = void> struct disable_if { using type = T; };
    template<typename T> struct disable_if<true, T> {};
  }

  template<typename C, typename T = void> using enable_if = typename traits::enable_if<C::value, T>::type;
  template<typename C, typename T = void> using disable_if = typename traits::disable_if<C::value, T>::type;
}

namespace nall {
  namespace traits {
    template<bool C, typename T, typename F> struct type_if { using type = T; };
    template<typename T, typename F> struct type_if<false, T, F> { using type = F; };
  }

  template<typename C, typename T, typename F> using type_if = typename traits::type_if<C::value, T, F>::type;
}
