#include "prx/libkernel/include/Api.hpp"
#include "prx/libc/include/Api.hpp"
#include "prx/libScePosixForWebKit/include/Api.hpp"
#include "prx/libc/include/GuestArena.hpp"
#include "prx/libc/include/General.hpp"
#include "prx/libc/include/FilesystemError.hpp"
#include "prx/libc/include/general/VabiMacros.hpp"
#include "prx/libkernel/File/include/FileFlags.hpp"
#include "prx/libkernel/Socket/include/SocketRuntime.hpp"
#include <algorithm>
#include <cerrno>
#include <cstring>
#include <filesystem>
#include <random>
#include <string>
#ifdef _WIN32
#include <windows.h>
#include <io.h>
extern "C" _invalid_parameter_handler _set_thread_local_invalid_parameter_handler(_invalid_parameter_handler);
#else
#include <unistd.h>
#endif

namespace {
constexpr char Characters[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

int Failure(int error) {
    *__error_nid_postfix() = error;
    return -1;
}

int Failure(const std::error_code& error) {
    *__error_nid_postfix() = FilesystemError(error);
    return -1;
}

#ifdef _WIN32
void IgnoreInvalidParameter(const wchar_t*, const wchar_t*, const wchar_t*, unsigned int, std::uintptr_t) {}
#endif
}

extern "C" int APS5_VABI mkstemp_nid_postfix(char* pattern) {
    if (!pattern) return Failure(14);
    const auto length = std::strlen(pattern);
    if (length == 0) return Failure(22);
    if (length >= 1024) return Failure(63);
    auto start = length;
    while (start > 0 && pattern[start - 1] == 'X') --start;
    const GuestArena::HostWrite destination(pattern + start, length - start);
    if (!destination.Open()) return Failure(14);
    if (start != length) {
        std::random_device random;
        std::uniform_int_distribution<unsigned int> character(0, sizeof(Characters) - 2);
        for (auto position = start; position < length; ++position)
            pattern[position] = Characters[character(random)];
    }
    const std::string initial(pattern + start, length - start);
    for (;;) {
        const int descriptor = open_nid_postfix(pattern, SCE_KERNEL_O_RDWR | SCE_KERNEL_O_CREAT | SCE_KERNEL_O_EXCL, 0600);
        if (descriptor >= 0 || *__error_nid_postfix() != 17) return descriptor;
        auto position = start;
        for (; position < length; ++position) {
            const auto index = std::strchr(Characters, pattern[position]) - Characters;
            pattern[position] = Characters[(index + 1) % (sizeof(Characters) - 1)];
            if (pattern[position] != initial[position - start]) break;
        }
        if (position == length) return -1;
    }
}

extern "C" int APS5_VABI isatty_nid_postfix(int descriptor) {
    if (GuestSocketIsOpen_nid_no_patch(descriptor)) {
        Failure(25);
        return 0;
    }
#ifdef _WIN32
    const auto previous = _set_thread_local_invalid_parameter_handler(IgnoreInvalidParameter);
    const auto handle = reinterpret_cast<HANDLE>(::_get_osfhandle(descriptor));
    _set_thread_local_invalid_parameter_handler(previous);
    if (handle == INVALID_HANDLE_VALUE) {
        Failure(9);
        return 0;
    }
    DWORD mode = 0;
    if (::GetConsoleMode(handle, &mode)) return 1;
    Failure(25);
#else
    if (::isatty(descriptor)) return 1;
    Failure(errno == EBADF ? 9 : 25);
#endif
    return 0;
}

extern "C" int APS5_VABI __wrap_link(const char* existing, const char* created) {
    if (existing == nullptr || created == nullptr) return Failure(14);
    if (*existing == '\0' || *created == '\0') return Failure(2);
    std::filesystem::path resolvedExisting;
    if (const int error = ResolvePathError_nid_no_patch(existing, resolvedExisting)) return Failure(error);
    std::filesystem::path resolvedCreated;
    if (const int error = ResolvePathError_nid_no_patch(created, resolvedCreated)) return Failure(error);
#ifdef _WIN32
    std::error_code error;
    std::filesystem::create_hard_link(resolvedExisting, resolvedCreated, error);
    if (error) return Failure(error);
    return 0;
#else
    if (::link(resolvedExisting.c_str(), resolvedCreated.c_str()) != 0)
        return Failure(std::error_code(errno, std::generic_category()));
    return 0;
#endif
}

extern "C" std::int64_t APS5_VABI __wrap_readlink(const char* path, char* buffer, std::size_t size) {
    if (path == nullptr || buffer == nullptr) return Failure(14);
    if (size == 0) return Failure(22);
    const GuestArena::HostWrite destination(buffer, size);
    if (!destination.Open()) return Failure(14);
    std::filesystem::path resolved;
    if (const int error = ResolvePathError_nid_no_patch(path, resolved)) return Failure(error);
#ifdef _WIN32
    std::error_code error;
    const auto target = std::filesystem::read_symlink(resolved, error);
    if (error) return Failure(error);
    const auto targetString = target.string();
    const auto toCopy = std::min(size, targetString.size());
    std::memcpy(buffer, targetString.data(), toCopy);
    return static_cast<std::int64_t>(toCopy);
#else
    const auto result = ::readlink(resolved.c_str(), buffer, size);
    if (result < 0) return Failure(std::error_code(errno, std::generic_category()));
    return static_cast<std::int64_t>(result);
#endif
}

extern "C" int APS5_VABI __wrap_symlink(const char* target, const char* created) {
    if (target == nullptr || created == nullptr) return Failure(14);
    if (*target == '\0' || *created == '\0') return Failure(2);
    const std::filesystem::path targetPath(target);
    if (targetPath.has_root_name() || targetPath.has_root_directory()) {
        NotImplemented_nid_no_patch("symlink: absolute targets");
        return -1;
    }
    for (const auto& part : targetPath) {
        if (part == "..") {
            NotImplemented_nid_no_patch("symlink: parent-directory targets");
            return -1;
        }
    }
    std::filesystem::path resolvedCreated;
    if (const int error = ResolvePathError_nid_no_patch(created, resolvedCreated)) return Failure(error);
#ifdef _WIN32
    std::error_code error;
    std::filesystem::create_symlink(targetPath, resolvedCreated, error);
    if (error) return Failure(error);
    return 0;
#else
    if (::symlink(target, resolvedCreated.c_str()) != 0)
        return Failure(std::error_code(errno, std::generic_category()));
    return 0;
#endif
}
