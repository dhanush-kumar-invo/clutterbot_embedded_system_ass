#include <iostream>
#include <thread>
#include <random>
#include <functional>
#include "stdint.h"
#include <mutex>

class process
{
private:
    /* data */
    std::mutex memory_mutex;
    uint8_t memory_buffer[1024];
    uint16_t memory_pointer = 0;
    uint8_t create_random(void);
    void display(bool isTx, uint8_t data, ssize_t len);
    std::thread *process_thread = nullptr;
public:
    process(/* args */);
    ~process();
    int write_process(uint8_t *data, ssize_t len);
    void read_process(uint8_t *rx_buff, ssize_t len);
    void rx_thread(void);
    bool Close();
};


