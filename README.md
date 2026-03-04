# Multithreaded Audio Stitching Engine

## Project Overview
This project is a high-performance multithreaded application designed to stitch together 23 separate WAV files into a continuous audio stream for playback. It was developed as part of an Optimized C++ Multithreading course to demonstrate advanced thread management, synchronization, and design patterns without relying on high-level abstractions like `std::atomic` or `std::future`/`std::promise`.

The core challenge of this project is to manage multiple threads manually, ensuring thread safety and data integrity using only `std::mutex` and `std::condition_variable`, while maintaining real-time audio playback performance.

## Requirements
*   **IDE:** Visual Studio 2022 (MSVC C++ 14.44 or later)
*   **OS:** Windows 10/11
*   **Hardware:** Audio output device

## How to Run
1.  **Open the Project:**
    *   Navigate to the project root directory.
    *   Open `PA1.sln` in Visual Studio.
2.  **Build:**
    *   Select the **Release** or **Debug** configuration.
    *   Build the solution (Ctrl+Shift+B).
3.  **Execute:**
    *   Run the application (F5 or Ctrl+F5).
    *   The console will display the status of the threads.
    *   The application will automatically load the 23 split WAV files from the `Data/` directory, stitch them together, and play the continuous audio.
    *   The program will exit automatically upon completion or can be terminated via the configured kill key.

## Architecture & Design Highlights

### Thread Management
This project avoids `std::async` and `std::future` to focus on raw thread management. It utilizes five distinct threads, each with a specific responsibility:

1.  **File Thread (`FileThread`):**
    *   **Role:** Producer.
    *   **Function:** Sequentially loads the 23 raw WAV files (2KB chunks) from the disk.
    *   **Mechanism:** Reads a file into a local buffer and signals the Coordinator when ready. It sleeps briefly between files to simulate load and prevent buffer starvation.

2.  **Coordinator Thread (`CoordThread`):**
    *   **Role:** Manager / Middle-man.
    *   **Function:** Manages the data flow between the File Thread and the Playback Thread.
    *   **Mechanism:** Implements a **Double Buffering** scheme (`Front` and `Back` buffers).
        *   **Back Buffer:** Receives data from the File Thread.
        *   **Front Buffer:** Feeds data to the Playback Thread.
        *   **Swapping:** When the Front buffer is exhausted and the Back buffer is full, they are swapped atomically.
    *   **State Machine:** Uses the **State Pattern** (`CoordInitState`, `CoordPlayState`, `CoordEndingState`) to manage the lifecycle of the stitching process.

3.  **Playback Thread (`PlaybackThread`):**
    *   **Role:** Consumer.
    *   **Function:** Sends audio data to the hardware driver.
    *   **Mechanism:** Consumes small (2KB) chunks from the Coordinator's Front buffer. It uses a **Circular Queue** to manage playback commands.

4.  **Kill Thread (`KillThread`):**
    *   **Role:** System Monitor.
    *   **Function:** Listens for user input to gracefully terminate all running threads and clean up resources.

5.  **Unit Test Thread (`UnitTestThread`):**
    *   **Role:** Validator.
    *   **Function:** Runs concurrent verifications to ensure data integrity during the multithreaded execution.

### Key Design Patterns & Concepts

*   **Producer-Consumer Pattern:**
    *   Implemented multiple times: File -> Coordinator, and Coordinator -> Playback.
    *   Ensures decoupled processing where data production (loading files) happens independently of consumption (playing audio).

*   **Double Buffering:**
    *   Used in the `CoordThread` to allow simultaneous reading and writing. While one buffer is being emptied by the playback engine, the other is being filled by the file loader, minimizing playback stutter.

*   **Command Pattern:**
    *   `WaveFillCmd`: Encapsulates the request to fill a buffer with audio data.
    *   `WaveFinishedCmd`: Signals that a buffer has finished playing.
    *   Allows for asynchronous communication between the Playback and Coordinator threads via a lock-free (or lock-guarded) queue.

*   **State Pattern:**
    *   The `CoordThread` transitions through various states (`Init`, `Play`, `Ending`, `Finished`) to handle the complexity of starting up, streaming, and shutting down the pipeline correctly.

*   **Manual Synchronization:**
    *   **Mutexes (`std::mutex`):** Protect shared resources like the file read count, shared buffers, and status flags.
    *   **Condition Variables (`std::condition_variable`):** Used for signaling between threads (e.g., File thread signaling Coordinator that a file is loaded).
    *   **No Atomics:** All synchronization is achieved through explicit locking, demonstrating a deep understanding of memory visibility and race conditions.

## Project Structure
*   `PA1/`: Contains the source code for the application.
    *   `main.cpp`: Entry point, initializes threads.
    *   `*Thread.cpp/h`: Thread implementations.
    *   `*State.cpp/h`: State machine logic.
    *   `*Cmd.cpp/h`: Command pattern implementations.
*   `Data/`: Contains the 23 split WAV files.
*   `Framework/` & `Dist/`: Library dependencies and helper classes.
