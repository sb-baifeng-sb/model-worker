
#include <stdio.h>
#include "source/mw-header.h"

int main() {
    mw::Facade a;
    a.hh().notify(mw::IntEvent("", 0));
    return 0;
}