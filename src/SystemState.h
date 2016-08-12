#ifndef SYSTEMSTATE_H
#define SYSTEMSTATE_H

enum class SystemState {
        PRE_FLIGHT = 1,
        DURING_FLIGHT,
        DURING_DESCENT,
        LOW_BATTERY,
        LANDED,
        INVALID = -1
};

#endif // SYSTEMSTATE_H