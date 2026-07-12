#pragma once
#include "db.h"
#include "events.h"
#include <string>
#include <functional>
#include <unordered_map>
#include <any>
#include <stdexcept>

// The FaaS runtime dispatcher.
// Routes named events to registered handler functions.
// This indirection layer is what makes FaaS architecturally different:
// every call pays the cost of a name lookup + std::any boxing/unboxing.

class Dispatcher {
public:
    using Handler = std::function<std::any(const std::any&, Database&)>;

    void register_function(const std::string& name, Handler h) {
        registry_[name] = std::move(h);
    }

    std::any invoke(const std::string& name, const std::any& event, Database& db) {
        auto it = registry_.find(name);
        if (it == registry_.end())
            throw std::runtime_error("Unknown function: " + name);
        return it->second(event, db);
    }

private:
    std::unordered_map<std::string, Handler> registry_;
};

// Build and return a fully-wired dispatcher
Dispatcher make_dispatcher();
