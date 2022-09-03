#include "descriptors_t_input.hpp"
#include "descriptors_t_util.hpp"

const std::vector<std::string> hexadecimal_zero {
    "0x0", "0X0", "0x0000"
};

const std::vector<std::string> hexadecimals {
    "0x0A", "0xF1"
};

const std::vector<std::string> positive_zero {
    "0", "000", "+0", "+000"
};

const std::vector<std::string> negative_zero {
    "-0", "-000"
};

const std::vector<std::string> positive_integers {
    "39", "+39", "1337", "+1337"
};

const std::vector<std::string> negative_integers {
    "-39", "-1337"
};

const std::vector<std::string> floating_point_numbers {
    "0.0", ".0", "+.0", "-.0", "1.337", "-1.337", "+1.337"
};

const std::vector<std::string> identifiers {
    "identifier", "iden_ti_fier", "_identifier_", "a123", "a_123", "_a_123_"
};

const std::vector<std::string> names {
    "names-can-have-dashes--in-between"
};

const std::vector<std::string> win_paths {
    "C:\\Program Files\\program_v1.exe" 
};
const std::vector<std::string> nix_paths {
    "/home/elrat/../elrat/.vim/vimrc" 
};

const std::vector<std::string> range_0_to_9{ 
    convertRangeToStrings(0,9,1) 
};

const std::vector<std::string> range_10_to_19 { 
    convertRangeToStrings(10,19,1) 
};

const std::vector<std::string> range_20_to_29 { 
    convertRangeToStrings(20,29,1) 
};



