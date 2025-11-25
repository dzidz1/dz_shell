# DZ_Shell

**A lightweight Unix shell implementation in C**

DZ_Shell is a functional Unix shell built from scratch, demonstrating core concepts like process management (fork/exec), PATH resolution, and environment variable handling.

## Demo

🎥 **[Watch the demo]https://www.youtube.com/watch?v=idEPh_Yheh0**

## Installation

```bash
git clone https://github.com/dzidz1/DZ_Shell.git
cd DZ_Shell
make
./dz_shell
```

## Features

### Built-in Commands

| Command            | Description                              |
| ------------------ | ---------------------------------------- |
| `cd [path]`        | Change directory (`~`, `-`, `..`)        |
| `pwd`              | Print working directory                  |
| `echo [text]`      | Display text or variables (`$VAR`, `-n`) |
| `env`              | List environment variables               |
| `setenv VAR=value` | Set environment variable                 |
| `unsetenv VAR`     | Remove environment variable              |
| `which [command]`  | Show command location                    |
| `exit` / `quit`    | Exit shell                               |

### External Commands

Executes programs from PATH, absolute paths (`/bin/ls`), or relative paths (`./program`)

## Development

```bash
make                      # Build dz_shell
make test                 # Build and run all tests
make test_builtins_only   # Build and run only built-ins tests
make test_helpers_only    # Build and run helper tests
make test_parser_only     # Build and run parser tests
make test_executor_only   # Build and run executor tests
make clean                # Remove object files
make fclean               # Remove object files and all binaries
make re                   # Rebuild from scratch
```

## License

MIT License - see [LICENSE](LICENSE)

---

Built by Akaki Dzidziguri (dzidz1)
