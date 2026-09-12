# TaskTrack

A command-line task manager built with C++. Made for https://roadmap.sh/projects/task-tracker

## Features

- Add, update, and remove tasks
- View and list tasks
- Track task status
- Task descriptions
- JSON-based data persistence
- Undo the last modification
- File attachments
- Automatic file path detection

## Commands

```text
TaskTrack add "<task>"
TaskTrack list
TaskTrack view <task number>
TaskTrack update <task number> "<new task>"
TaskTrack remove <task number>
TaskTrack mark <task number> <status>
TaskTrack attach <task number> "<file path>"
TaskTrack undo
TaskTrack help
```

## Statuses

- `todo`
- `in-progress`
- `done`

## Technologies

- C++
- nlohmann/json
- JSON
- File I/O
- Windows API

## Setup

1. **Move TaskTrack to a permanent location.**

   For example:

   ```text
   C:\Tools\TaskTrack\
   ```

2. **Compile `main.cpp`** using a C++ development environment such as Dev-C++, VS Code with MinGW/GCC, or another C++ compiler.

   This should produce:

   ```text
   TaskTrack.exe
   ```

3. **Add the TaskTrack folder containing `TaskTrack.exe` to the Windows PATH environment variable.**

   For example:

   ```text
   C:\Tools\TaskTrack\
   ```

4. **Open a new terminal** and run:

   ```text
   TaskTrack help
   ```

   This displays the available commands and their usage.

## sol
[https://roadmap.sh/projects/task-tracker/solutions?u=69c2377533a0ad7a5763d181](https://roadmap.sh/projects/task-tracker/solutions?u=69c2377533a0ad7a5763d181)
