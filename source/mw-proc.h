
#ifndef __MW_PROC_H__
#define __MW_PROC_H__

#include "mw-event.h"
#include "mw-context.h"
#include <functional>

namespace mw {

class ProcHolder {
    friend class Context;
public:
    class ProcEvent {
    public:
        Context& context;
        Event const& event;
    public:
        ProcEvent(Context& c, Event const& e):context(c), event(e) {}
    };
public:
    typedef std::function<void(Event const& e)> Proc;
    typedef std::map<std::string, Proc> ProcMap;
public:
    bool set(std::string const& eventName, Proc const& p);
    bool remove(std::string const& eventName);
public:
    ProcHolder(Context* context);
private:
    void handle(Event const& e);
private:
    Context* context;
    ProcMap mProcMap;
};

typedef ProcHolder::ProcEvent ProcEvent;

}

#endif