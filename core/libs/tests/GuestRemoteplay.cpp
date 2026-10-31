#include "prx/libSceRemoteplay/include/Api.hpp"
#include "prx/libSceRemoteplay/include/InventoryExports.hpp"

#include <array>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <stdexcept>

static void Require(bool value) { if (!value) std::abort(); }

static void Require(bool condition, const char* message) {
    if (!condition) {
        std::fprintf(stderr, "%s\n", message);
        std::abort();
    }
}

template <typename TCall>
static bool Throws(TCall call) {
    try {
        call();
    } catch (const std::runtime_error&) {
        return true;
    }
    return false;
}

int main() {
    constexpr auto invalidArgument = static_cast<int>(0x80FC0001u);
    constexpr auto alreadyInitialized = static_cast<int>(0x80FC0003u);
    constexpr auto notInitialized = static_cast<int>(0x80FC0004u);
    constexpr int connectionStatusDisconnect = 0;
    std::array<unsigned char, 0x1800> heap{};

    int status = -1;
    Require(sceRemoteplayGetConnectionStatus(0, &status) == notInitialized && status == -1);
    Require(sceRemoteplayTerminate() == notInitialized);

    Require(sceRemoteplayInitialize(nullptr, 0) == 0);
    Require(sceRemoteplayGetConnectionStatus(0, nullptr) == invalidArgument);
    status = -1;
    Require(sceRemoteplayGetConnectionStatus(0, &status) == 0 && status == connectionStatusDisconnect);
    Require(sceRemoteplayInitialize(nullptr, 0) == alreadyInitialized);
    Require(sceRemoteplayInitialize(heap.data(), heap.size()) == alreadyInitialized);

    Require(sceRemoteplayTerminate() == 0);
    Require(sceRemoteplayTerminate() == notInitialized);
    status = -1;
    Require(sceRemoteplayGetConnectionStatus(0, &status) == notInitialized && status == -1);

    Require(sceRemoteplayInitialize(heap.data(), heap.size() - 1) == invalidArgument);
    Require(sceRemoteplayTerminate() == notInitialized);

    Require(sceRemoteplayInitialize(heap.data(), heap.size()) == 0);
    Require(sceRemoteplayTerminate() == 0);

    Require(sceRemoteplayInitialize(nullptr, 1) == 0);
    Require(sceRemoteplayTerminate() == 0);

    Require(sceRemoteplayInitialize(heap.data(), heap.size()) == 0);
    status = -1;
    Require(sceRemoteplayGetConnectionStatus(3, &status) == 0 && status == connectionStatusDisconnect);
    Require(sceRemoteplayTerminate() == 0);

    Require(Throws([] { sceRemoteplayIsRemoteViewering(); }), "sceRemoteplayIsRemoteViewering did not throw std::runtime_error");
    Require(Throws([] { sceRemoteplayNotifyCanWakeup(); }), "sceRemoteplayNotifyCanWakeup did not throw std::runtime_error");
    Require(Throws([] { sceRemoteplayRegistCallback(); }), "sceRemoteplayRegistCallback did not throw std::runtime_error");
    Require(Throws([] { sceRemoteplayUnregistCallback(); }), "sceRemoteplayUnregistCallback did not throw std::runtime_error");
    Require(Throws([] { sceRemoteplayApprove(); }), "sceRemoteplayApprove did not throw std::runtime_error");
    Require(Throws([] { sceRemoteplayClearAllRegistData(); }), "sceRemoteplayClearAllRegistData did not throw std::runtime_error");
    Require(Throws([] { sceRemoteplayClearConnectHistory(); }), "sceRemoteplayClearConnectHistory did not throw std::runtime_error");
    Require(Throws([] { sceRemoteplayConfirmDeviceRegist(); }), "sceRemoteplayConfirmDeviceRegist did not throw std::runtime_error");
    Require(Throws([] { sceRemoteplayDisconnect(); }), "sceRemoteplayDisconnect did not throw std::runtime_error");
    Require(Throws([] { sceRemoteplayGeneratePinCode(); }), "sceRemoteplayGeneratePinCode did not throw std::runtime_error");
    Require(Throws([] { sceRemoteplayGetApMode(); }), "sceRemoteplayGetApMode did not throw std::runtime_error");
    Require(Throws([] { sceRemoteplayGetConnectHistory(); }), "sceRemoteplayGetConnectHistory did not throw std::runtime_error");
    Require(Throws([] { sceRemoteplayGetConnectUserId(); }), "sceRemoteplayGetConnectUserId did not throw std::runtime_error");
    Require(Throws([] { sceRemoteplayGetMbusDeviceInfo(); }), "sceRemoteplayGetMbusDeviceInfo did not throw std::runtime_error");
    Require(Throws([] { sceRemoteplayGetOperationStatus(); }), "sceRemoteplayGetOperationStatus did not throw std::runtime_error");
    Require(Throws([] { sceRemoteplayGetPrimaryUserId(); }), "sceRemoteplayGetPrimaryUserId did not throw std::runtime_error");
    Require(Throws([] { sceRemoteplayGetRemoteplayStatus(); }), "sceRemoteplayGetRemoteplayStatus did not throw std::runtime_error");
    Require(Throws([] { sceRemoteplayGetRpMode(); }), "sceRemoteplayGetRpMode did not throw std::runtime_error");
    Require(Throws([] { sceRemoteplayImeClose(); }), "sceRemoteplayImeClose did not throw std::runtime_error");
    Require(Throws([] { sceRemoteplayImeFilterResult(); }), "sceRemoteplayImeFilterResult did not throw std::runtime_error");
    Require(Throws([] { sceRemoteplayImeGetEvent(); }), "sceRemoteplayImeGetEvent did not throw std::runtime_error");
    Require(Throws([] { sceRemoteplayImeNotify(); }), "sceRemoteplayImeNotify did not throw std::runtime_error");
    Require(Throws([] { sceRemoteplayImeNotifyEventResult(); }), "sceRemoteplayImeNotifyEventResult did not throw std::runtime_error");
    Require(Throws([] { sceRemoteplayImeOpen(); }), "sceRemoteplayImeOpen did not throw std::runtime_error");
    Require(Throws([] { sceRemoteplayImeSetCaret(); }), "sceRemoteplayImeSetCaret did not throw std::runtime_error");
    Require(Throws([] { sceRemoteplayImeSetText(); }), "sceRemoteplayImeSetText did not throw std::runtime_error");
    Require(Throws([] { sceRemoteplayIsRemoteOskReady(); }), "sceRemoteplayIsRemoteOskReady did not throw std::runtime_error");
    Require(Throws([] { sceRemoteplayIsRemotePlaying(); }), "sceRemoteplayIsRemotePlaying did not throw std::runtime_error");
    Require(Throws([] { sceRemoteplayNotifyMbusDeviceRegistComplete(); }), "sceRemoteplayNotifyMbusDeviceRegistComplete did not throw std::runtime_error");
    Require(Throws([] { sceRemoteplayNotifyNpPushWakeup(); }), "sceRemoteplayNotifyNpPushWakeup did not throw std::runtime_error");
    Require(Throws([] { sceRemoteplayNotifyPinCodeError(); }), "sceRemoteplayNotifyPinCodeError did not throw std::runtime_error");
    Require(Throws([] { sceRemoteplayNotifySubmitCode(); }), "sceRemoteplayNotifySubmitCode did not throw std::runtime_error");
    Require(Throws([] { sceRemoteplayNotifyUserDelete(); }), "sceRemoteplayNotifyUserDelete did not throw std::runtime_error");
    Require(Throws([] { sceRemoteplayPrintAllRegistData(); }), "sceRemoteplayPrintAllRegistData did not throw std::runtime_error");
    Require(Throws([] { sceRemoteplayProhibit(); }), "sceRemoteplayProhibit did not throw std::runtime_error");
    Require(Throws([] { sceRemoteplayProhibitStreaming(); }), "sceRemoteplayProhibitStreaming did not throw std::runtime_error");
    Require(Throws([] { sceRemoteplayServerLock(); }), "sceRemoteplayServerLock did not throw std::runtime_error");
    Require(Throws([] { sceRemoteplayServerUnLock(); }), "sceRemoteplayServerUnLock did not throw std::runtime_error");
    Require(Throws([] { sceRemoteplaySetApMode(); }), "sceRemoteplaySetApMode did not throw std::runtime_error");
    Require(Throws([] { sceRemoteplaySetLogLevel(); }), "sceRemoteplaySetLogLevel did not throw std::runtime_error");
    Require(Throws([] { sceRemoteplaySetProhibition(); }), "sceRemoteplaySetProhibition did not throw std::runtime_error");
    Require(Throws([] { sceRemoteplaySetProhibitionForVsh(); }), "sceRemoteplaySetProhibitionForVsh did not throw std::runtime_error");
    Require(Throws([] { sceRemoteplaySetRpMode(); }), "sceRemoteplaySetRpMode did not throw std::runtime_error");
    Require(Throws([] { sceRemoteplayDisableInputDevice(); }), "sceRemoteplayDisableInputDevice did not throw std::runtime_error");
    Require(Throws([] { sceRemoteplayEnableInputDevice(); }), "sceRemoteplayEnableInputDevice did not throw std::runtime_error");

    return 0;
}
