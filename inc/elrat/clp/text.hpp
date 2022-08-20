// Project......: Command Line Processor (clp)
// File.........: inc/elrat/clp/text.hpp
// Author.......: elratmacfat
// Description..: Unlike every other part of the library, the 
//                operator<< overloads have to be accessible
//                directly in global namespace.
//
//                  clp::parser::data d;
//                  std::cout << d; // wouldn't be possible if
//                                  // operator<< was inside a 
//                                  // nested namespace.
//
#ifndef ELRAT_CLP_TEXT_HPP
#define ELRAT_CLP_TEXT_HPP

#include <elrat/clp/parser.hpp>

#include <iostream>

std::ostream& operator<<(std::ostream&,const elrat::clp::parser::data&);
std::ostream& operator<<(std::ostream&,const elrat::clp::parser::error&);
std::ostream& operator<<(std::ostream&,const elrat::clp::parser::error::code&);

#endif 

