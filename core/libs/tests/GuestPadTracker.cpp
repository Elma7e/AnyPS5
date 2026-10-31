#include "prx/libScePadTracker/include/Api.hpp"
#include <array>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include "cstdio"
#include "stdexcept"

namespace {

void Require(bool value) { if (!value) std::abort(); }

void Require(bool value, const char* message) {
    if (!value) {
        std::fprintf(stderr, "%s\n", message);
        std::abort();
    }
}

bool Throws(auto call) {
    try {
        call();
    } catch (const std::runtime_error&) {
        return true;
    } catch (...) {
        return false;
    }
    return false;
}

constexpr int kErrInvalidArg = static_cast<int>(0x80c40003);
constexpr std::uint32_t kFill = 0xaaaaaaaau;

}

int main() {
    std::array<std::uint32_t, 2> first{kFill, kFill};
    std::array<std::uint32_t, 2> second{kFill, kFill};

    Require(scePadTrackerGetWorkingMemorySize(nullptr, nullptr) == kErrInvalidArg);
    Require(scePadTrackerGetWorkingMemorySize(first.data(), nullptr) == kErrInvalidArg);
    Require(scePadTrackerGetWorkingMemorySize(nullptr, second.data()) == kErrInvalidArg);
    Require(first[0] == kFill && first[1] == kFill && second[0] == kFill && second[1] == kFill);

    Require(scePadTrackerGetWorkingMemorySize(first.data(), second.data()) == 0);
    Require(first[0] == 0x800000 && second[0] == 0x800000);
    Require(first[1] == kFill && second[1] == kFill);

    Require(Throws([] { scePadTrackerInit(); }), "scePadTrackerInit did not throw");
    Require(Throws([] { scePadTrackerCalibrate(); }), "scePadTrackerCalibrate did not throw");
    Require(Throws([] { scePadTrackerReadState(); }), "scePadTrackerReadState did not throw");
    Require(Throws([] { scePadTrackerTerm(); }), "scePadTrackerTerm did not throw");
    Require(Throws([] { scePadTrackerUpdate(); }), "scePadTrackerUpdate did not throw");

    return 0;
}
