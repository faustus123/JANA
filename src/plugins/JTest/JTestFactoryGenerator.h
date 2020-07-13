
// Copyright 2020, Jefferson Science Associates, LLC.
// Subject to the terms in the LICENSE file found in the top-level directory.

#ifndef JTestFactoryGenerator_h
#define JTestFactoryGenerator_h

#include <JANA/JFactoryGenerator.h>
#include "JTestDisentangler.h"
#include "JTestTracker.h"

class JTestFactoryGenerator : public JFactoryGenerator {
public:

    virtual const char *className() {
        return "JTestFactoryGenerator";
    }

    void GenerateFactories(JFactorySet *factory_set) override {
        factory_set->Add(new JTestDisentangler());
        factory_set->Add(new JTestTracker());
    }
};

#endif // JTestFactoryGenerator_h

