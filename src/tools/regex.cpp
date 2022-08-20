#include "elrat/clp/util.hpp"
#include <iostream>

int main() 
{
    using namespace elrat::clp;
    std::string src("abc def");
    std::regex rgx("[^\\s]+");
    std::vector<std::string_view> results = util::regex_separate( src, rgx );
    for( auto res : results )
        std::cout << '\"' << res << "\"\n";

    return 0;
}
