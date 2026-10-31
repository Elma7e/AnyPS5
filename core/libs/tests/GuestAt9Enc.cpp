#include "prx/libSceAt9Enc/include/Api.hpp"
#include "prx/libc/include/general/VabiMacros.hpp"
#include <cstdint>
#include <cstdlib>
#include <stdexcept>

namespace {

void Require(bool value, const char* message = nullptr) {
    if (!value) {
        std::abort();
    }
}

template <typename Call>
bool Throws(Call call) {
    try {
        static_cast<void>(call());
    } catch (const std::runtime_error&) {
        return true;
    }
    return false;
}

}

int main() {
    int32_t memSize = 0;
    int32_t internalResult = 0;

    Require(Throws([&] { return sceAt9EncClearContext(nullptr, &internalResult); }));
    Require(Throws([&] { return sceAt9EncCreateEncoder(nullptr, nullptr, 0, &internalResult); }));
    Require(Throws([&] { return sceAt9EncEncode(nullptr, nullptr, nullptr, 0, 0, nullptr, nullptr, &internalResult); }));
    Require(Throws([&] { return sceAt9EncFlush(nullptr, nullptr, 0, nullptr, &internalResult); }));
    Require(Throws([&] { return sceAt9EncQueryMemSize(nullptr, &memSize, &internalResult); }));

    Require(memSize == 0);
    Require(internalResult == 0);

    alignas(16) uint8_t workArea[256];
    auto FillSentinel = [&] {
        for (auto& byte : workArea) {
            byte = 0x5a;
        }
    };
    FillSentinel();

    auto Untouched = [&] {
        for (uint8_t byte : workArea) {
            if (byte != 0x5a) {
                return false;
            }
        }
        return true;
    };

    uint8_t paramBuffer[64]{};
    uint8_t inputBuf[64]{};
    uint8_t outputBuf[64]{};
    uint32_t outputDataByte = 0;
    uint32_t usedBufSize = 0;

    FillSentinel();
    Require(Throws([&] { return sceAt9EncClearContext(workArea, nullptr); }), "sceAt9EncClearContext did not throw on null internal result");
    Require(Untouched(), "sceAt9EncClearContext modified work area buffer");

    FillSentinel();
    Require(Throws([&] { return sceAt9EncCreateEncoder(workArea, nullptr, 0, &internalResult); }), "sceAt9EncCreateEncoder did not throw on null param buffer");
    Require(Untouched(), "sceAt9EncCreateEncoder modified work area buffer");

    FillSentinel();
    Require(Throws([&] { return sceAt9EncCreateEncoder(workArea, paramBuffer, (int32_t)sizeof(paramBuffer), nullptr); }), "sceAt9EncCreateEncoder did not throw on null internal result");
    Require(Untouched(), "sceAt9EncCreateEncoder modified work area buffer");

    FillSentinel();
    Require(Throws([&] { return sceAt9EncCreateEncoder(workArea, nullptr, 0, nullptr); }), "sceAt9EncCreateEncoder did not throw on null buffers and null internal result");
    Require(Untouched(), "sceAt9EncCreateEncoder modified work area buffer");

    FillSentinel();
    Require(Throws([&] { return sceAt9EncEncode(workArea, nullptr, nullptr, 0, 0, nullptr, nullptr, &internalResult); }), "sceAt9EncEncode did not throw on null buffers");
    Require(Untouched(), "sceAt9EncEncode modified work area buffer");

    FillSentinel();
    Require(Throws([&] { return sceAt9EncEncode(workArea, inputBuf, outputBuf, (uint32_t)sizeof(inputBuf), (uint32_t)sizeof(outputBuf), &outputDataByte, &usedBufSize, nullptr); }), "sceAt9EncEncode did not throw on null internal result");
    Require(Untouched(), "sceAt9EncEncode modified work area buffer");

    FillSentinel();
    Require(Throws([&] { return sceAt9EncEncode(workArea, nullptr, outputBuf, 0, (uint32_t)sizeof(outputBuf), &outputDataByte, &usedBufSize, &internalResult); }), "sceAt9EncEncode did not throw on null input buffer");
    Require(Untouched(), "sceAt9EncEncode modified work area buffer");

    FillSentinel();
    Require(Throws([&] { return sceAt9EncEncode(workArea, inputBuf, nullptr, (uint32_t)sizeof(inputBuf), 0, &outputDataByte, &usedBufSize, &internalResult); }), "sceAt9EncEncode did not throw on null output buffer");
    Require(Untouched(), "sceAt9EncEncode modified work area buffer");

    FillSentinel();
    Require(Throws([&] { return sceAt9EncEncode(workArea, nullptr, nullptr, 0, 0, nullptr, nullptr, nullptr); }), "sceAt9EncEncode did not throw on null buffers and null internal result");
    Require(Untouched(), "sceAt9EncEncode modified work area buffer");

    FillSentinel();
    Require(Throws([&] { return sceAt9EncFlush(workArea, nullptr, 0, nullptr, &internalResult); }), "sceAt9EncFlush did not throw on null output buffer");
    Require(Untouched(), "sceAt9EncFlush modified work area buffer");

    FillSentinel();
    Require(Throws([&] { return sceAt9EncFlush(workArea, outputBuf, (uint32_t)sizeof(outputBuf), &outputDataByte, nullptr); }), "sceAt9EncFlush did not throw on null internal result");
    Require(Untouched(), "sceAt9EncFlush modified work area buffer");

    FillSentinel();
    Require(Throws([&] { return sceAt9EncFlush(workArea, nullptr, 0, nullptr, nullptr); }), "sceAt9EncFlush did not throw on null buffers and null internal result");
    Require(Untouched(), "sceAt9EncFlush modified work area buffer");

    FillSentinel();
    Require(Throws([&] { return sceAt9EncQueryMemSize(workArea, nullptr, &internalResult); }), "sceAt9EncQueryMemSize did not throw on null mem size buffer");
    Require(Untouched(), "sceAt9EncQueryMemSize modified work area buffer");

    FillSentinel();
    Require(Throws([&] { return sceAt9EncQueryMemSize(workArea, &memSize, nullptr); }), "sceAt9EncQueryMemSize did not throw on null internal result");
    Require(Untouched(), "sceAt9EncQueryMemSize modified work area buffer");

    FillSentinel();
    Require(Throws([&] { return sceAt9EncQueryMemSize(workArea, nullptr, nullptr); }), "sceAt9EncQueryMemSize did not throw on null mem size and null internal result");
    Require(Untouched(), "sceAt9EncQueryMemSize modified work area buffer");

    FillSentinel();
    Require(Throws([&] { return sceAt9EncQueryMemSize(nullptr, (int32_t*)workArea, &internalResult); }), "sceAt9EncQueryMemSize did not throw on null param buffer");
    Require(Untouched(), "sceAt9EncQueryMemSize modified work area buffer");

    FillSentinel();
    Require(Throws([&] { return sceAt9EncQueryMemSize(nullptr, (int32_t*)workArea, nullptr); }), "sceAt9EncQueryMemSize did not throw on null param buffer and null internal result");
    Require(Untouched(), "sceAt9EncQueryMemSize modified work area buffer");

    Require(memSize == 0, "memSize was modified");
    Require(internalResult == 0, "internalResult was modified");
}
