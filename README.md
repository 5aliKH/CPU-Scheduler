# CPU-Scheduler
Advanced CPU Scheduling Simulator in C++ implementing FCFS, SJF, Priority, Round Robin, SRTF, and HRRN algorithms with Gantt Charts and performance analysis.
# CPU Scheduler Simulator (Chronos Scheduler)

## Overview

Chronos Scheduler is a C++ console-based CPU Scheduling Simulator designed to demonstrate and compare the behavior of multiple CPU scheduling algorithms used in operating systems. The project provides a user-friendly interactive interface, colorful console output, Gantt chart visualization, and detailed performance reports for each scheduling technique.

The simulator allows users to register processes with custom attributes such as Arrival Time, Burst Time, and Priority, then execute different scheduling algorithms and analyze their performance through calculated metrics including Waiting Time, Turnaround Time, and Completion Time.

## Features

* Interactive console-based user interface.
* Dynamic process registration and management.
* Gantt Chart visualization for execution order.
* Detailed scheduling reports.
* Average Waiting Time calculation.
* Average Turnaround Time calculation.
* Manual implementation of data structures without STL containers.
* Colorized terminal output for better user experience.

## Implemented Scheduling Algorithms

### 1. First Come First Serve (FCFS)

Executes processes in the order of their arrival time.

### 2. Shortest Job First (SJF)

Selects the process with the smallest burst time among ready processes.

### 3. Priority Scheduling

Schedules processes according to priority values.

### 4. Round Robin (RR)

Uses a configurable time quantum to fairly distribute CPU time.

### 5. Shortest Remaining Time First (SRTF)

Preemptive version of SJF that always executes the process with the shortest remaining execution time.

### 6. Highest Response Ratio Next (HRRN)

Chooses the process with the highest response ratio to reduce starvation.

## Technical Implementation

The project is developed entirely in C++ and includes:

* Custom Linked List implementation.
* Priority Queue simulation.
* Process management system.
* Scheduling engine.
* Reporting and statistics module.
* Console visualization utilities.

## Process Attributes

Each process contains:

* Process ID
* Arrival Time
* Burst Time
* Priority
* Remaining Burst Time
* Start Time
* Completion Time
* Waiting Time
* Turnaround Time

## Performance Metrics

The simulator calculates:

* Completion Time (CT)
* Waiting Time (WT)
* Turnaround Time (TAT)
* Average Waiting Time
* Average Turnaround Time

## Technologies Used

* C++
* Object-Oriented Programming (OOP)
* Windows Console API
* Custom Data Structures

## Project Structure

* Process Management Module
* Scheduler Queue Module
* History & Reporting Module
* Scheduling Engine
* User Interface Layer

## Educational Purpose

This project was developed for educational purposes to help students understand CPU scheduling concepts, process management, queue operations, and operating system scheduling techniques through practical simulation.

## Future Improvements

* GUI version using Qt.
* Export reports to PDF or CSV.
* Multi-core CPU simulation.
* Real-time scheduling algorithms.
* Process execution timeline visualization.
* Linux cross-platform support.

## Author

Developed as an Operating Systems scheduling simulation project using C++.
