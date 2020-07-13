
:w
// Copyright 2020, Jefferson Science Associates, LLC.
// Subject to the terms in the LICENSE file found in the top-level directory.


#include "catch.hpp"
#include "ExactlyOnceTests.h"

#include <JANA/JEventSourceGeneratorT.h>
#include <JANA/JApplication.h>

TEST_CASE("ExactlyOnceTests") {

    JApplication app;

    auto source = new SimpleSource("SimpleSource", &app);
    auto processor = new SimpleProcessor(&app);

    app.Add(source);
    app.Add(processor);
    app.SetParameterValue("jana:extended_report", 0);

    SECTION("Old engine: JEventProcessor::Init(), Finish() called exactly once") {

        REQUIRE(source->open_count == 0);
        REQUIRE(processor->init_count == 0);
        REQUIRE(processor->finish_count == 0);

        app.SetParameterValue("jana:legacy_mode", 1);
        app.Run(true);

        REQUIRE(source->open_count == 1);
        REQUIRE(processor->init_count == 1);
        REQUIRE(processor->finish_count == 1);
    }


    SECTION("New engine: JEventProcessor::Init(), Finish() called exactly once") {

        REQUIRE(source->open_count == 0);
        REQUIRE(processor->init_count == 0);
        REQUIRE(processor->finish_count == 0);

        app.SetParameterValue("jana:legacy_mode", 0);
        app.Run(true);

        REQUIRE(source->open_count == 1);
        REQUIRE(processor->init_count == 1);
        REQUIRE(processor->finish_count == 1);
    }
}



