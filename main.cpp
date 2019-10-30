
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
        } else if (e.Name() == "event3") {
            printf("worker1 - event3.\n");
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
        } else if (e.Name() == "event4") {
            printf("worker2 - event4.\n");
        }
    }
};

int main() {
    Facade a;

    a.ph().add("model1", new model1);
    a.ph().add("model2", new model2);

    a.wh().add("worker1", new worker1);
    a.wh().add("worker2", new worker2);

    a.proc().add("event4", [&](ProcEvent const& e) {
        printf("proc - %s.\n", e.event.Name().c_str());
    });

    a.notify("event1");
    a.notify("event2");

    auto& m1 = a.ph().get<model1>("model1");
    m1.doSome();
    m1.notify("event3");

    auto& m2 = a.ph().get<model2>("model2");
    m2.doSome();
    m2.notify("event4");

    return 0;
}