#include <iostream>
#include <chrono>
#include <thread>
#include <mach/mach.h>
#include <sys/types.h>
#include <sys/sysctl.h>
#include <sys/vmmeter.h>
#include <mach/mach.h>
#include <mach/mach_init.h>
#include <mach/mach_host.h>
#include <mach/mach_port.h>
#include <mach/mach_traps.h>
#include <mach/task_info.h>
#include <mach/thread_info.h>
#include <mach/thread_act.h>
#include <mach/vm_region.h>
#include <mach/vm_map.h>
#include <mach/task.h>
#include <mach/shared_region.h>
#include <unistd.h>

using namespace std;

enum BYTE_UNITS
{
    BYTES = 0,
    KILOBYTES = 1,
    MEGABYTES = 2,
    GIGABYTES = 3
};

template <class T>
T convert_unit(T num, int to, int from = BYTES)
{
    for (; from < to; from++)
    {
        num /= 1024;
    }
    return num;
}

// 获取mac系统下总物理内存、所使用内存
void getMemUsePercentage()
{
    u_int64_t total_mem = 0;
    float used_mem = 0;

    vm_size_t page_size;
    vm_statistics_data_t vm_stats;

    // Get total physical memory
    int mib[] = { CTL_HW, HW_MEMSIZE };
    size_t length = sizeof(total_mem);
    sysctl(mib, 2, &total_mem, &length, NULL, 0);

    mach_port_t mach_port = mach_host_self();
    mach_msg_type_number_t count = sizeof(vm_stats) / sizeof(natural_t);
    if (KERN_SUCCESS == host_page_size(mach_port, &page_size) &&
        KERN_SUCCESS == host_statistics(mach_port, HOST_VM_INFO,
            (host_info_t)&vm_stats, &count)
        )
    {
        used_mem = static_cast<float>(
            (vm_stats.active_count + vm_stats.wire_count) * page_size);
    }

    uint usedMem = convert_unit(static_cast<float>(used_mem), MEGABYTES);
    uint totalMem = convert_unit(static_cast<float>(total_mem), MEGABYTES);
    cout << "\nusedMem:" << usedMem << endl;
    cout << "totalMem:" << totalMem << endl;
}

// 获取mac系统下当前进程使用内存
int runGetDynamicProcInfo(unsigned int& m_nMemUsed)
{
    unsigned int task = 0;
    int error = 0;
    unsigned int count = 0;
    struct task_basic_info ti;

    error = task_for_pid(mach_task_self(), getpid(), &task);
    if (error != KERN_SUCCESS)
    {
        m_nMemUsed = 0;
        return 0;
    }
    count = TASK_BASIC_INFO_COUNT;
    error = task_info(task, TASK_BASIC_INFO, (task_info_t)&ti, &count);
    if (error != KERN_SUCCESS)
    {
        m_nMemUsed = 0;
        return 0;
    }

    vm_region_basic_info_data_64_t b_info;
    unsigned long address = SHARED_REGION_BASE_PPC;
    unsigned long size = 0;
    unsigned int object_name = 0;
    count = VM_REGION_BASIC_INFO_COUNT_64;
    error = vm_region_64(task, &address, &size, VM_REGION_BASIC_INFO, (vm_region_info_t)&b_info, &count, &object_name);
    if (error == KERN_SUCCESS) 
    {
        if (b_info.reserved && size == (SHARED_REGION_NESTING_SIZE_PPC) &&
            ti.virtual_size > (SHARED_REGION_NESTING_SIZE_PPC + SHARED_REGION_NESTING_MIN_PPC))
        {
            ti.virtual_size -= (SHARED_REGION_NESTING_SIZE_PPC + SHARED_REGION_NESTING_MIN_PPC);
        }
    }
    m_nMemUsed = (ti.resident_size / 1024 / 1024);
    std::cout << "m_nMemUsed:" << m_nMemUsed << std::endl;
    return 0;
}

int main()
{
    unsigned int m_usfmem = 0;

    while (1)
    {
        runGetDynamicProcInfo(m_usfmem);
        getMemUsePercentage();
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;

}