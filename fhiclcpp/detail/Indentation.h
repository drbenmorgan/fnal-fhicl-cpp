#ifndef fhiclcpp_detail_Indentation_h
#define fhiclcpp_detail_Indentation_h

#include <stack>
#include <string>

// =========================================================
//
// Indentation
//
// Used for providing appropriate indentation for
// ParameterSet::to_indented_string.
//
//
// The 'pop' and 'push' commands must be used symmetrically.
//
//
// =========================================================

namespace fhicl {
  namespace detail {

    class Indentation {
    public:

      Indentation(unsigned const iil = 0u)
        : indents_{{ std::string(iil*indent_increment,' ') }}
      {}

      Indentation(std::string const& prefix)
        : indents_{{prefix}}
      {}

      std::string const & operator()() const { return indents_.top(); }

      void modify_top(std::string const& s)
      {
        indents_.pop();
        indents_.emplace(indents_.top() + s);
      }

      auto size() { return indents_.size(); }

      void pop () { indents_.pop() ; }
      void push() { indents_.emplace( indents_.top()+std::string(indent_increment, ' ' )); }

    private:
      static constexpr std::size_t indent_increment = 3u;
      std::stack<std::string> indents_;
    };

  }
}

#endif

// Local variables:
// mode: c++
// End:
