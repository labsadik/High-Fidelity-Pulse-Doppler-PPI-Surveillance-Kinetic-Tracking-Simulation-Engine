
# StratosRadar — High-Fidelity Pulse-Doppler PPI Surveillance & Kinetic Tracking Simulation Engine

A high-performance, real-time radar tracking simulation written in C++ using SFML 3. Designed to simulate a 10,000 km operational area with realistic target behaviors, movement physics, and a professional GUI interface.

## Project Overview

This system simulates a military-grade air and sea surveillance radar. It features a Plan Position Indicator (PPI) display, dynamic target generation, and real-time tracking capabilities. The simulation handles coordinate conversion, physics updates, and detection logic in a 60 FPS loop.

## Features & Functionality

### 1. Realistic Radar Display (PPI)
*   **Plan Position Indicator**: Classic green radar scope with rotating sweep line.
*   **Range Rings & Bearing Lines**: Visual grid overlay for quick range/bearing estimation.
*   **Afterglow Effect**: Visual "trail" effect left by the sweep line for aesthetic realism.
*   **Dynamic Zoom**: Seamless zooming from 10 km up to 10,000 km range scale.

### 2. Advanced Target Tracking
*   **Trail History**: Visual track history (trails) showing the path of detected targets over time.
*   **Vector Information**: Real-time calculation of speed, heading, altitude, and bearing.
*   **Polar Coordinate System**: Accurate conversion between Cartesian (X/Y) and Polar (Range/Bearing) coordinates.
*   **Detection Logic**: Targets are only "painted" when the radar sweep crosses their bearing, simulating real pulse-doppler behavior.

### 3. Diverse Target Types
The simulation distinguishes between different classes of objects with unique physics and visual representations:
*   **Airliners**: Commercial aircraft flying at high altitudes (FL300+) with standard cruise speeds (~900 km/h). Displayed as large triangles.
*   **Fighter Jets**: Military aircraft capable of high speeds and maneuvering. Displayed as small, fast triangles.
*   **Missiles**: High-velocity threats moving at speeds up to Mach 5. Displayed as small, intense red dots.
*   **Naval Vessels**: Surface ships moving at slow speeds. Displayed as squares.
*   **IFF Status**: Targets are color-coded by status:
    *   **Green**: Friendly
    *   **Red**: Hostile
    *   **Yellow**: Neutral/Unknown

### 4. Professional GUI Interface
*   **Side Control Panel**: Located on the right side for easy access.
*   **Spawning Controls**: Dedicated buttons to spawn specific target types (Airliner, Fighter, Missile, Ship).
*   **System Monitoring**: Real-time display of simulation time and active target count.
*   **Interactive Selection**: Click on any target to view detailed information (Callsign, Type, Speed, Range).

### 5. Physics & Coordinate System
*   **10,000 km Range**: Capable of simulating vast distances.
*   **Realistic Movement**: Targets move based on heading and speed vectors.
*   **Continuous Loop**: Aircraft that fly out of range are regenerated at random entry points to maintain constant traffic density.

## System Requirements

*   **OS**: Windows (MinGW/MSYS2), Linux, or macOS.
*   **Compiler**: C++17 compatible (GCC 16+ recommended).
*   **Libraries**: SFML 3.0 or higher.

## Installation & Build Instructions

### Windows (MSYS2 MinGW 64-bit)

1.  **Open MSYS2 MinGW 64-bit terminal**.
2.  **Navigate to the project folder**:
    ```bash
    cd /c/Users/YourName/Desktop/radar
    ```
3.  **Create build directory**:
    ```bash
    mkdir build
    cd build
    ```
4.  **Configure with CMake**:
    ```bash
    cmake .. -G "MinGW Makefiles"
    ```
5.  **Build the executable**:
    ```bash
    cmake --build .
    ```
6.  **Run the simulation**:
    ```bash
    ./radar_sim.exe
    ```

## Controls

| Input | Action |
| :--- | :--- |
| **Mouse Scroll** | Zoom In / Out (Change Range Scale) |
| **Left Click (Radar)** | Select a target to view info |
| **Left Click (GUI)** | Press buttons (Spawn/Clear) |
| **ESC** | Close Application |
