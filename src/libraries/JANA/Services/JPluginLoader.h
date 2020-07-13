
// Copyright 2020, Jefferson Science Associates, LLC.
// Subject to the terms in the LICENSE file found in the top-level directory.


#ifndef JANA2_JPLUGINLOADER_H
#define JANA2_JPLUGINLOADER_H

#include <JANA/Services/JLoggingService.h>
#include <JANA/Services/JParameterManager.h>

#include <string>
#include <vector>


class JComponentManager;
class JApplication;

class JPluginLoader : public JService {

public:

    JPluginLoader(JApplication* app);
    ~JPluginLoader() override = default;
    void acquire_services(JServiceLocator*) override;

    void add_plugin(std::string plugin_name);
    void add_plugin_path(std::string path);
    void attach_plugins(JComponentManager* jcm);
    void attach_plugin(JComponentManager* jcm, std::string plugin_name);

private:

    std::vector<std::string> _plugins_to_include;
    std::vector<std::string> _plugins_to_exclude;
    std::vector<std::string> _plugin_paths;
    std::vector<void*> _sohandles;

    bool _verbose = false;
    JLogger _logger;

    JApplication* _app = nullptr;

};


#endif //JANA2_JPLUGINLOADER_H
