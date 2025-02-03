#include "process.hpp"

#define BYTE_LEN 1
#define TX  true
#define RX false


process::process(/* args */)
{
     process_thread = new std::thread([this](){this->rx_thread(); });
    //process_thread = new std::thread(std::bind(&process::rx_thread, this));
}

process::~process()
{
    Close();

}

bool process::Close()
{
    if ((process_thread != nullptr) && (process_thread->joinable()))
    {
        process_thread->join();
        delete process_thread;
    }
    return true;
}

uint8_t process::create_random(void)
{
    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_int_distribution<int> dist(0, 255);

    int random_byte = dist(gen);
    return (uint8_t)random_byte;
}

void process::display(bool isTx, uint8_t data, ssize_t len)
{
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);

    std::cout << std::hex << std::showbase << std::uppercase;
    if (isTx == TX)
    {    
        std::cout << "[TX]: VAL = " << (int)data << " BYTES WRITTEN = "  << len << " [TIMESTAMP] "  << std::ctime(&now_c) << std::endl;
    }
    else if(isTx == RX)
    {
        std::cout << "[RX]: VAL = " << (int)data << " BYTES READ = "  << len << " [TIMESTAMP] " << std::ctime(&now_c) << std::endl;
    }
}

int process::write_process(uint8_t *data, ssize_t len)
{
    for (int i = 0; i < len; i++)
    {
        data[i] = create_random();
        // std::cout<<int(data[i]);
        std::lock_guard<std::mutex> lock(memory_mutex);
        this->memory_buffer[this->memory_pointer] = data[i];
        this->memory_pointer++;
        display(TX, data[i], BYTE_LEN);
    }
    return len;
}

void process::read_process(uint8_t *rx_buff, ssize_t len)
{
    for (int i = 0; i < len; i++)
    {
        std::lock_guard<std::mutex> lock(memory_mutex);
        if (this->memory_pointer > 0)
        {
            rx_buff[i] = this->memory_buffer[--this->memory_pointer];
            //this->memory_pointer--;
            display(RX, rx_buff[i], BYTE_LEN);
        }
        
    }
}

void process::rx_thread(void)
{
    static uint8_t rx_buff[10] = {0};
    while (true)
    {
        read_process(rx_buff, BYTE_LEN);
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    }
}

int main()
{
    uint8_t tx_buff[10] = {0};
    process *rxTxProcess = new process();
    while(true)
    {
        rxTxProcess->write_process(tx_buff, BYTE_LEN);
        std::this_thread::sleep_for(std::chrono::milliseconds(990));
    }
}


