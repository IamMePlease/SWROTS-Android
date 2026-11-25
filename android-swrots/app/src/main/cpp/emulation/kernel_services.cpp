#include "kernel_services.h"
#include "../common.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <pthread.h>

struct KernelServices::Impl {
    bool initialized = false;
    std::vector<int> file_handles;
    std::vector<pthread_t> threads;
};

KernelServices::KernelServices() : impl(std::make_unique<Impl>()) {}

KernelServices::~KernelServices() {
    shutdown();
}

bool KernelServices::initialize() {
    LOGI("Initializing Xbox kernel services");
    
    // Initialize file handle table
    impl->file_handles.resize(256, -1);
    
    impl->initialized = true;
    LOGI("Kernel services initialized");
    return true;
}

void KernelServices::shutdown() {
    // Close all file handles
    for (int& fd : impl->file_handles) {
        if (fd >= 0) {
            close(fd);
            fd = -1;
        }
    }
    impl->initialized = false;
}

int KernelServices::CreateFile(const char* path, int access, int mode) {
    // Map Xbox access modes to POSIX
    int flags = 0;
    if (access & 0x1) flags |= O_RDONLY;
    if (access & 0x2) flags |= O_WRONLY;
    if (access & 0x4) flags |= O_RDWR;
    
    int fd = open(path, flags, mode);
    if (fd >= 0 && fd < 256) {
        impl->file_handles[fd] = fd;
        return fd;
    }
    return -1;
}

bool KernelServices::ReadFile(int handle, void* buffer, int size, int* bytesRead) {
    if (handle < 0 || handle >= 256 || impl->file_handles[handle] < 0) {
        return false;
    }
    
    ssize_t result = read(impl->file_handles[handle], buffer, size);
    if (result >= 0) {
        *bytesRead = result;
        return true;
    }
    return false;
}

bool KernelServices::WriteFile(int handle, const void* buffer, int size, int* bytesWritten) {
    if (handle < 0 || handle >= 256 || impl->file_handles[handle] < 0) {
        return false;
    }
    
    ssize_t result = write(impl->file_handles[handle], buffer, size);
    if (result >= 0) {
        *bytesWritten = result;
        return true;
    }
    return false;
}

void KernelServices::CloseHandle(int handle) {
    if (handle >= 0 && handle < 256 && impl->file_handles[handle] >= 0) {
        close(impl->file_handles[handle]);
        impl->file_handles[handle] = -1;
    }
}

int KernelServices::CreateThread(void (*func)(void*), void* param, int priority) {
    pthread_t thread;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    
    // Set priority (nice value)
    sched_param sch;
    sch.sched_priority = priority;
    pthread_attr_setschedparam(&attr, &sch);
    
    int result = pthread_create(&thread, &attr, (void*(*)(void*))func, param);
    pthread_attr_destroy(&attr);
    
    if (result == 0) {
        impl->threads.push_back(thread);
        return (int)thread;
    }
    return -1;
}

void KernelServices::WaitForSingleObject(int handle, int timeout) {
    // TODO: Implement wait with timeout
    // For now, just yield
    usleep(1000);
}

void KernelServices::SetEvent(int event) {
    // TODO: Implement event signaling
}

void* KernelServices::VirtualAlloc(int size, int type) {
    void* addr = mmap(nullptr, size, PROT_READ | PROT_WRITE,
                      MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    return addr == MAP_FAILED ? nullptr : addr;
}

bool KernelServices::VirtualFree(void* addr, int size) {
    return munmap(addr, size) == 0;
}
