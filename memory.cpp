#include <array>   // std::array
#include <cstddef> // std::size_t
#include <cstdint>

class Memory
{
using Address = std::uint32_t;

private:
    static constexpr std::size_t Page_size = 4096;
    static constexpr std::size_t Page_count = 64;
    static constexpr std::size_t Frame_count = 16;
    std::array<char, Page_size * Frame_count> physical_mem_{};

    struct PageTableEntry
    {
        bool valid = false;
        int frame = -1;
    };

    std::array<PageTableEntry, Page_count> page_table_{};

public:
    char read(Address virtual_address);
    void write(Address virtual_address, char value);
};