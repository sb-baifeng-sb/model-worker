
#include "source/mw-header.h"

using namespace mw;

class model1 : public Proxy {
public:
    void doSome() {
        printf("model1 do.\n");
    }
};
class model2 : public Proxy {
public:
    void doSome() {
        printf("model2 do.\n");
    }
};

class worker1 : public Worker {
private:
    WorkList worklist() const override {
        WorkList list;
        list.push_back("event1");
        list.push_back("event3");
        return list;
    }
    void handle(Event const& e) override {
        if (e.Name() == "event1") {
            printf("worker1 - event1.\n");
            this->notify("worker1-event1-notify");
        } else if (e.Name() == "event3") {
            printf("worker1 - event3.\n");
            this->notify("worker1-event3-notify");
        }
    }
};
class worker2 : public Worker {
    WorkList worklist() const override {
        WorkList list;
        list.push_back("event2");
        list.push_back("event4");
        return list;
    }
    void handle(Event const& e) override {
        if (e.Name() == "event2") {
            printf("worker2 - event2.\n");
            this->notify("worker2-event2-notify");
        } else if (e.Name() == "event4") {
            printf("worker2 - event4.\n");
            this->notify("worker2-event4-notify");
        }
    }
};

class eventHandler {
public:
    void handle(Event const& e) {
        printf("eventHandler - %s\n", e.Name().c_str());
    }
};

int main() {
    Context c;

    c.proxy().add("model1", new model1);
    c.proxy().add("model2", new model2);

    c.worker().add("worker1", new worker1);
    c.worker().add("worker2", new worker2);

    c.proc().set("event4", [&](ProcEvent const& e) {
        printf("proc - %s.\n", e.event.Name().c_str());
    });
    c.proc().set("worker1-event1-notify", [&](ProcEvent const& e) {
        printf("proc - %s.\n", e.event.Name().c_str());
    });
    c.proc().set("worker1-event3-notify", [&](ProcEvent const& e) {
        printf("proc - %s.\n", e.event.Name().c_str());
    });
    c.proc().set("worker2-event2-notify", [&](ProcEvent const& e) {
        printf("proc - %s.\n", e.event.Name().c_str());
    });
    c.proc().set("worker2-event4-notify", [&](ProcEvent const& e) {
        printf("proc - %s.\n", e.event.Name().c_str());
    });

    eventHandler eh;
    c.event().add("event4", &eventHandler::handle, &eh);

    c.notify("event1");
    c.notify("event2");

    auto& m1 = c.proxy().get<model1>("model1");
    m1.doSome();
    m1.notify("event3");

    auto& m2 = c.proxy().get<model2>("model2");
    m2.doSome();
    m2.notify("event4");

    c.event().remove("event4", &eh);
    c.notify("event4");

    return 0;
}