
#ifndef __MW_PROC_H__
#define __MW_PROC_H__

#include "mw-event.h"
#include "mw-facade.h"
#include "functional"

namespace mw {

class ProcHolder {
    friend class Facade;
public:
    class ProcEvent {
    public:
        Facade& facade;
        Event const& event;
    public:
        ProcEvent(Facade& facade, Event const& e):facade(facade), event(e) {}
    };
public:
    typedef std::function<void(ProcEvent const& e)> Proc;
    typedef std::map<std::string, Proc> ProcMap;
public:
    bool add(std::string const& eventName, Proc const& p);
    bool remove(std::string const& eventName);
public:
    ProcHolder(Facade* facade);
private:
    void handle(Event const& e);
private:
    Facade* facade;
    ProcMap mProcMap;
};

typedef ProcHolder::ProcEvent ProcEvent;

}

#endif