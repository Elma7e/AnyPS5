#include "prx/libSceEditMp4/include/Api.hpp"
#include <array>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include "cstdio"
#include "stdexcept"

namespace {

void Require(bool value) { if (!value) std::abort(); }

void Require(bool condition, const char* message) {
    if (!condition) {
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

std::array<std::uint8_t, 0x28> Initialized(std::uint8_t fill) {
    std::array<std::uint8_t, 0x28> bytes{};
    bytes.fill(fill);
    sceEditMp4InitializeParam(bytes.data());
    return bytes;
}

}

int main() {
    for (const std::uint8_t fill : {std::uint8_t{0x00}, std::uint8_t{0xaa}}) {
        const auto bytes = Initialized(fill);
        for (std::size_t index = 0; index < 0x10; ++index) Require(bytes[index] == 0);
        std::uint64_t at10 = 0;
        std::uint32_t at18 = 0;
        std::memcpy(&at10, bytes.data() + 0x10, sizeof(at10));
        std::memcpy(&at18, bytes.data() + 0x18, sizeof(at18));
        Require(at10 == 0x3f);
        Require(at18 == 700);
        for (std::size_t index = 0x1c; index < bytes.size(); ++index) Require(bytes[index] == fill);
    }
    sceEditMp4InitializeParam(nullptr);

    Require(sceEditMp4QueryMemSize() == 0x387d30);
    Require(sceEditMp4QueryMemSize() == 0x387d30);

    Require(Throws([] { sceEditMp4Abort(); }), "sceEditMp4Abort did not throw");
    Require(Throws([] { sceEditMp4Edit(); }), "sceEditMp4Edit did not throw");
    Require(Throws([] { sceEditMp4GetStatus(); }), "sceEditMp4GetStatus did not throw");
    Require(Throws([] { sceEditMp4Initialize(); }), "sceEditMp4Initialize did not throw");
    Require(Throws([] { sceEditMp4SetFinishCallback(); }), "sceEditMp4SetFinishCallback did not throw");
    Require(Throws([] { sceEditMp4Terminate(); }), "sceEditMp4Terminate did not throw");
    Require(Throws([] { sceEditMp4Unite(); }), "sceEditMp4Unite did not throw");
    return 0;
}
