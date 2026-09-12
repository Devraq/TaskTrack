# TaskTrack

A command-line task manager built with C++.

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