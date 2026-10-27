#include <array>   // std::array
#include <cstddef> // std::size_t
#include <cstdint>

class Memory
{
    using VirtualAddress = std::uint32_t;
    using PhysicalAddress = std::uint32_t;

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
    static constexpr std::size_t Virtual_memory_size = Page_size * Page_count;
    PhysicalAddress translate_virtual_to_physical(VirtualAddress virtual_address)
    {
        if(virtual_address <= Virtual_memory_size)
        {
            int page_number = virtual_address / Page_size ;
            int offset = virtual_address % Page_size;
            auto entry = page_table_.at(page_number);
            if(entry.valid)
            {
                return entry.frame * Page_size + offset;
            }
            else
            {
//TODO: 缺页情况下的分配
            }
        }
    }
public:
    char read(VirtualAddress virtual_address);
    void write(VirtualAddress virtual_address, char value);
};