#ifndef NALL_TRAITS_HPP
#define NALL_TRAITS_HPP

#include <type_traits>
#include <utility>

namespace nall {
  using std::nullptr_t;
  using std::forward;
  using std::move;
  using std::decay;
  using std::declval;

  using true_type = std::true_type;
  using false_type = std::false_type;

  template<typename T, typename U> using is_same = std::is_same<T, U>;
  template<typename T, typename U> using is_base_of = std::is_base_of<T, U>;
  template<typename T> using is_array = std::is_array<T>;
  template<typename T> using is_function = std::is_function<T>;
  template<typename T> using is_integral = std::is_integral<T>;
  template<typename T> using add_const = std::add_const<T>;
  template<typename T> using remove_extent = std::remove_extent<T>;
  template<typename T> using remove_reference = std::remove_reference<T>;
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

#endif
