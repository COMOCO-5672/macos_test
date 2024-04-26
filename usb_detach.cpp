#include <iostream>
#include <CoreFoundation/CoreFoundation.h>
#include <IOKit/IOKitLib.h>

// 回调函数
void DeviceAdded(void *refCon, io_iterator_t iterator);
void DeviceRemoved(void *refCon, io_iterator_t iterator);

int main() {
    // 创建一个 CFRunLoop 来监听事件
    CFRunLoopRef runLoop = CFRunLoopGetCurrent();

    // 创建一个 IOUSB 设备监听
    CFMutableDictionaryRef matchingDict = IOServiceMatching(kIOUSBDeviceClassName);
    io_service_t usbServices;
    kern_return_t kr;

    // 添加设备插入的监听
    kr = IOServiceAddMatchingNotification(kIOMasterPortDefault,
                                           kIOFirstMatchNotification,
                                           matchingDict,
                                           DeviceAdded,
                                           NULL,
                                           &usbServices);
    // 处理错误
    if (kr != kIOReturnSuccess) {
        std::cerr << "Error: Unable to create notification port for device addition." << std::endl;
        return 1;
    }

    // 注册监听端口
    kr = IOServiceAddMatchingNotification(kIOMasterPortDefault,
                                           kIOTerminatedNotification,
                                           matchingDict,
                                           DeviceRemoved,
                                           NULL,
                                           &usbServices);
    // 处理错误
    if (kr != kIOReturnSuccess) {
        std::cerr << "Error: Unable to create notification port for device removal." << std::endl;
        return 1;
    }

    // 启动 RunLoop
    CFRunLoopRun();

    // 不会运行到这里，因为 RunLoop 在循环中
    return 0;
}

// 设备添加的回调函数
void DeviceAdded(void *refCon, io_iterator_t iterator) {
    io_service_t service;
    while ((service = IOIteratorNext(iterator))) {
        std::cout << "USB Device Added" << std::endl;
        IOObjectRelease(service);
    }
}

// 设备移除的回调函数
void DeviceRemoved(void *refCon, io_iterator_t iterator) {
    io_service_t service;
    while ((service = IOIteratorNext(iterator))) {
        std::cout << "USB Device Removed" << std::endl;
        IOObjectRelease(service);
    }
}
