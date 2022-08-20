#include <elrat/clp/util.hpp>

namespace elrat {
namespace clp {

bool util::regex_match( std::string_view sv, std::regex rgx )
{
    return std::regex_match( sv.cbegin(), sv.cend(), rgx );
}

bool util::regex_match( std::string_view sv, std::string_view rgxsv )
{
    return util::regex_match( sv, std::regex(rgxsv.data(), rgxsv.size()) );
}

std::vector<std::string_view> util::regex_separate( 
    std::string_view src, 
    std::regex rgx )
{
    using svregex_iterator = std::regex_iterator<std::string_view::iterator>;
    std::vector<std::string_view> results{};
    

    auto a = src.begin();
    auto b = src.end();
    svregex_iterator begin( a, b, rgx );
    svregex_iterator end;

    for( auto it = begin; it != end; it++ )
        results.emplace_back( it->str(), it->length()  );

    /*
    std::match_results<std::string_view::const_iterator> matches;

    if( std::regex_search( src.cbegin(), src.cend(), matches, rgx ) )
    {
        for( std::size_t i{0}; i < matches.size(); i++ )
        {
            const char* first = matches[i].first;
            const char* last = matches[i].second;
            std::size_t size{ static_cast<std::size_t>( last - first ) };
            results.emplace_back( first, size );
        }
    }
    */
    return results;
}

} // clp
} // elrat
