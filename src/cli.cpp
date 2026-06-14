#include "cli.h"

CLI::CLI(Operate &&operate, Options &&options)
    : operate(std::move(operate)), options(std::move(options)) {}

int CLI::execute() {
    fprintf(stderr, "Executing CLI commands...\n");

    if (!operate.updateEcData()) {
        fprintf(stderr, "Could not get the data from the existing service\n");
        return 1;
    }

    if (options.cooler_boost.has_value()) {
        setCoolerBooster(options.cooler_boost.value());
    }
    if (options.user_mode.has_value()) {
        setUserMode(options.user_mode.value());
    }

    return 0;
}

void CLI::setCoolerBooster(Options::CoolerBoostState state) {
    if (state == Options::CoolerBoostState::TOGGLE) {

        operate.setCoolerBoostState(!operate.getCoolerBoostState());
    } else {
        operate.setCoolerBoostState(state == Options::CoolerBoostState::ON);
    }
}

void CLI::setUserMode(Options::UserMode mode) {
    switch (mode) {
    case Options::UserMode::PERFORMANCE:
        operate.setUserMode(user_mode::performance_mode);
        break;
    case Options::UserMode::BALANCED:
        operate.setUserMode(user_mode::balanced_mode);
        break;
    case Options::UserMode::SILENT:
        operate.setUserMode(user_mode::silent_mode);
        break;
    case Options::UserMode::BATTERY:
        operate.setUserMode(user_mode::super_battery_mode);
        break;
    case Options::UserMode::NEXT:
        using usermode_t =
            std::underlying_type_t<typeof(operate.getUserMode())>;

        auto usermode = static_cast<usermode_t>(operate.getUserMode());

        usermode =
            (usermode + 1) % static_cast<usermode_t>(user_mode::unknown_mode);
        operate.setUserMode(static_cast<user_mode>(usermode));
        break;
    }
}
