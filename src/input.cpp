
#include "elrat/clp/input.hpp"

#include <algorithm>

using namespace elrat::clp;

std::shared_ptr<Input> Input::MakeShared()
{
    return std::make_shared<Input>();
}

//
// Input (implementation of InputReaderInterface)
//

Input::operator bool() const 
{
    return ( _command.size() > 0 );
}

std::string_view Input::getCommand() const 
{
    return _command;
}

int Input::getCommandParameterCount() const 
{
    return _command_parameters.size();
}

std::string_view Input::getCommandParameter(int i) const 
{
    return _command_parameters.at(i);
};

int Input::getOptionCount() const 
{
    return _options.size();
}

std::string_view Input::getOption(int i) const 
{
    int x{0};
    auto it = _options.begin();
    while ( it != _options.end() && x < i ) 
    {
        it++;
        x++;
    }
    if ( it == _options.end() ) 
        throw std::out_of_range("Input::getOption() out of range.");
    return it->first;
}

bool Input::optionExists( const std::string& o ) const 
{
    return _options.count( o );
}

int Input::getOptionParameterCount(const std::string& o) const
{
    return _options.at(o).size();
}

std::string_view Input::getOptionParameter(const std::string& o, int i) const 
{
    return _options.at(o).at(i);
}

//
// Input (implementation of InputInterface)
//

void Input::setCommand( const std::string& s )
{
    _command = s;
}

void Input::addCommandParameter(const std::string& s )
{
    _command_parameters.push_back(s);
}

void Input::addOption( const std::string& s)
{
    _options.emplace( std::make_pair( s, std::vector<std::string>() ) );
}

void Input::addOptionParameter(const std::string& o, const std::string& p)
{
    _options.at(o).push_back(p);
}


