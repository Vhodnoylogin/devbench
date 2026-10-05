#include <SKSE/Impl/PCH.h>
#include <RE/T/TypeInfo.h>
#include <cstdint>
#include <cstdio>

int main()
{
    using Type = RE::BSScript::TypeInfo;
    // No fake object is dereferenced. Test actual library decoder preserving
    // legal eight-byte alignment, which a mask of ~11 wrongly shifts by eight.
    for (std::uintptr_t address : {0x1000ULL, 0x1008ULL, 0x1010ULL, 0x1018ULL}) {
        for (std::uintptr_t tag : {0ULL, 1ULL}) {
            Type info(static_cast<Type::RawType>(address | tag));
            if (reinterpret_cast<std::uintptr_t>(info.GetTypeInfo()) != address) {
                std::fprintf(stderr, "TypeInfo pointer mismatch address=%llx tag=%llu\n",
                    static_cast<unsigned long long>(address),static_cast<unsigned long long>(tag));
                return 1;
            }
        }
    }
    std::puts("TypeInfo actual decoder: all eight object/array alignment cases passed");
    return 0;
}
