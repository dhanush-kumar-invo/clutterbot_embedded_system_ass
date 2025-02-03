**Embedded Systems Intern - Assessment Documentation**

---

**1. Introduction**

This document outlines the debugging process, performance evaluation, and optimization suggestions for a multithreaded producer-consumer system. The project involved identifying bugs in a provided codebase, ensuring correct data transmission between threads, and improving system performance.

---

**2. Approach and Changes Made**

**Initial Issues Identified:**
1. **Data Mismatch:** The data received (RX) did not always match the data transmitted (TX).
2. **Shared Memory Buffer:** Both threads accessed the shared memory buffer without synchronization, causing unpredictable behavior.
3. **High CPU Utilization:** The RX thread ran continuously, leading to high CPU usage.
4. **Incorrect Buffer Access:** The memory pointer logic led to improper reading from the buffer.

**Steps to Resolve Issues:**


1. **Corrected Buffer Management:**
   - **Change:** Adjusted the `memory_pointer` to correctly point to the most recent data.
   - **Reason:** Ensured that RX reads the latest data and prevents buffer overflows/underflows.
   - **Code Snippet:**
     ```cpp
     if (this->memory_pointer > 0) {
         rx_buff[i] = this->memory_buffer[--this->memory_pointer];
     }
     ```

2. **Optimized CPU Utilization:**
   - **Change:** Introduced `std::this_thread::sleep_for(std::chrono::milliseconds(1000));` in the RX thread.
   - **Reason:** Reduced CPU load by avoiding constant polling in the RX thread.
   - **Code Snippet:**
     ```cpp
     std::this_thread::sleep_for(std::chrono::milliseconds(1000));
     ```

3. **Implemented Thread Synchronization:**
   - **Change:** Added `std::mutex` to manage access to shared memory.
   - **Reason:** Prevent simultaneous read/write access, thus eliminating race conditions.
   - **Code Snippet:**
     ```cpp
     std::lock_guard<std::mutex> lock(memory_mutex);
     ```

4. **Consistent Output Verification:**
   - **Change:** Ensured that TX data is immediately followed by the corresponding RX data.
   - **Reason:** Verified correctness of data transmission by matching transmitted and received values.
![Screenshot from 2025-02-04 01-38-40](https://github.com/user-attachments/assets/878d4a04-d2b4-474e-94bb-0d34a26b9194)

---

**3. Performance Evaluation**

**CPU Utilization Measurement:**
1. **Tools Used:**
   - `top` or `htop`: To monitor real-time CPU usage.
   - `time ./process_executable`: To measure execution time.

2. **Results:**
   - **Before Fix:**![Screenshot from 2025-02-04 01-02-03](https://github.com/user-attachments/assets/f6e4dbb3-972e-44e5-8d31-8ed803982228)

     - CPU utilization was **100%** due to continuous polling in the RX thread.
     - 
   - **After Fix:**![Screenshot from 2025-02-04 01-09-23](https://github.com/user-attachments/assets/825c0731-0c14-4a28-b1fb-7d4c88bc7e22)

     - CPU utilization dropped to **less than 10%** with the introduction of sleep intervals.

3. **Command Examples:**
   ```bash
   top -p 202974
   htop
   time Bin/process.cpp
   ```

---

**4. Suggestions for Further Performance Improvement**

1. **Use Condition Variables:**
   - **Why:** Instead of using sleep intervals, condition variables (`std::condition_variable`) can signal the RX thread only when new data is available.
   - **Benefit:** Eliminates unnecessary sleep time, improving responsiveness and efficiency.

2. **Dynamic Thread Management:**
   - **Why:** Adjust the sleep intervals or processing logic dynamically based on the load.
   - **Benefit:** Optimizes resource usage under varying workloads.
     
3. **Implement Semaphores:**
   - **Why:** Semaphores can be used to control access to shared resources, allowing multiple threads to coordinate efficiently.
   - **Benefit:** Provides finer control over resource allocation and prevents resource starvation, improving system stability and performance.

---

**5. Conclusion**

Through a systematic debugging approach, we resolved data integrity issues, eliminated race conditions, and significantly reduced CPU utilization. Further optimizations, such as implementing condition variables and circular buffers, could enhance the system's performance and scalability. The system now operates reliably and efficiently, matching the desired output specifications.


