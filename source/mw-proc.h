
#ifndef __MW_PROC_H__
#define __MW_PROC_H__

#include "mw-event.h"
#include "mw-facade.h"
#include "functional"

namespace mw {

class ProcHolder {
    friend class Facade;
public:
    typedef std::function<void(Facade& facade, Event const& e)> Proc;
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

}

#endif