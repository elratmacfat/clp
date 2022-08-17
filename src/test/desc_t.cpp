// Project......: Command Line Processor (clp)
// File.........: src/test/desc_t.cpp
// Author.......: elratmacfat
// Description..:
//
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include "elrat/clp/desc.hpp"

using namespace elrat;


BOOST_AUTO_TEST_SUITE( desc )

	struct fixture {
	    fixture()
	    : t{ clp::command(
	        "name",
	        "description",
	        {
	            clp::parameter(
	                "p1-name",
	                "p1-description", 
	                clp::parameter_properties::mandatory,
	                clp::parameter_type::natural_number,
	                {
	                    clp::constraint::at_most(100),
	                    clp::constraint::is_not(59),
	                    clp::constraint::at_least(40)
	                }
	            ),
	            clp::parameter(
	                "p2-name",
	                "p2-description", 
	                clp::parameter_properties::optional,
	                clp::parameter_type::real_number,
	                {
	                    clp::constraint::in_range(10.0f, 15.0f)
	                }
	            )
	        }, // end: command parameter declaration
	        {
	            clp::option(
	                "o1-name",
	                "o1-description",
	                {
	                    clp::parameter(
	                        "o1-p1-name",
	                        "o1-p1-description",
	                        clp::parameter_properties::optional,
	                        clp::parameter_type::identifier
	                        // no constraints = default
	                    )
	                }
	            ),
	            clp::option(
	                "o2-name",
	                "o2-description",
	                {
	                    clp::parameter(
	                        "o2-p1-name",
	                        "o2-p1-description"
	                        // parameter is mandatory by default
	                        //           is of type 'any' by default 
	                        //           has no constraints by default
	                    ),
	                    clp::parameter(
	                        "o2-p2-name",
	                        "o2-p2-description",
	                        clp::parameter_properties::optional
	                    )
	                }
	            )
	        } // end: option declaration
	      )} // end: fixture member initialization 
	    {
	        // done
	    }
	
	    ~fixture() 
	    {
	        // done
	    }
	
	    std::shared_ptr<clp::cmd_desc> t;
	};

    // Checks if the composition of cmd_desc, opt_desc, ... is initialized
    // correctly.
    //
    BOOST_AUTO_TEST_CASE( initialization )
    {
        fixture f;

        // command name & description
        //
        auto cmd{ f.t };
        BOOST_CHECK_EQUAL( cmd->name(), "name" );
        BOOST_CHECK_EQUAL( cmd->description(), "description" );
       
        // command parameter
        //
        auto parameters{ f.t->parameters() };
        BOOST_CHECK_EQUAL( parameters.size(), 2 );

        // first command parameter
        //
        auto param{ parameters.at(0) };
        BOOST_CHECK_EQUAL( param->name(), "p1-name" );
        BOOST_CHECK_EQUAL( param->description(), "p1-description");
        
        //BOOST_CHECK_EQUAL( param->type_checker(), 
        //    clp::parameter_type::natural_number );

        auto constraints{ param->constraints() };
        BOOST_CHECK_EQUAL( constraints.size(), 3 );
        
        // second command parameter
        //
        param = parameters.at(1);

        // options
        //
        auto options{ f.t->options() };
        BOOST_CHECK_EQUAL( options.size(), 2 );

        auto opt{ options.at(0) };
        BOOST_CHECK_EQUAL( opt->name(), "o1-name" );
        BOOST_CHECK_EQUAL( opt->description(), "o1-description" );

        opt = options.at(1);
        BOOST_CHECK_EQUAL( opt->name(), "o2-name" );
        BOOST_CHECK_EQUAL( opt->description(), "o2-description" );

        // option's parameters
        //
        
    }

BOOST_AUTO_TEST_SUITE_END(); // desc_test

