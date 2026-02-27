# PACS — Physical Access Control System

A lightweight command-line event logger for door access control, written in C.

## What It Does

PACS logs and tracks door access events in real time. It records when doors are opened, access is denied, or security alerts occur, and persists the log between sessions.

**Event types:**
- `GRANTED` — Access was allowed
- `DENIED` — Access was refused
- `DOOR FORCED` — Door opened without valid credential
- `DOOR HELD` — Door held open too long

**Key features:**
- Log access events with door ID, credential ID, and timestamp
- View the full event history
- Search events by door
- Automatic lockout warning after 3 consecutive denied attempts on the same door/credential pair
- Log saved to disk and reloaded on startup (`access_log.dat`)

## Building

Requires a C compiler (GCC or Clang) and `make` (optional).

```bash
gcc -o door_logger main.c events.c storage.c
```

## Usage

```bash
./door_logger
```

You will see a simple menu:

```
=== Door Access Event Logger ===

1) Add event
2) View all events
3) Search by door
4) Save and exit
```

**Adding an event** prompts for:
- Door ID (e.g. `DOOR-01`)
- Credential ID (e.g. `BADGE-42`)
- Event type (0 = GRANTED, 1 = DENIED, 2 = FORCED, 3 = HELD)

If a credential has been denied 3 or more consecutive times at the same door, a lockout warning is printed.

## Project Structure

```
PACS/
├── main.c       # CLI menu and entry point
├── events.c     # Event logging, searching, and lockout logic
├── events.h     # Shared types and function declarations
├── storage.c    # Binary file save/load
├── storage.h    # Storage constants and declarations
└── access_log.dat  # Generated at runtime — persisted event log
```

## Limits

| Parameter       | Value |
|-----------------|-------|
| Max events      | 100   |
| Door ID length  | 20    |
| Credential length | 20  |

## License

MIT — see [LICENSE](LICENSE).
