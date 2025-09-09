# my_grep

A custom implementation of the Unix `grep` utility written in C.  
This project supports searching through text files (or `stdin`) using both plain strings and regular expressions, with extra features such as colored output, line numbers, recursive directory traversal, and case-insensitive matching.  

Unlike the standard `grep`, this version is built from scratch and uses:
- **KMP string search algorithm** (for plain string search).
- **POSIX regex** (for regex search).
- **Custom dynamic vectors** implemented with **X-macros** (template-like in C).
- **ANSI colors** for highlighting results.

---

## Features

- **Search patterns**: fixed string or regular expression (`-regex`).
- **Colored output**:  
  - file paths in cyan,  
  - line numbers in blue,  
  - matches in red.  
- **Line numbers** (`-n`).
- **Case-insensitive search** (`-i`).
- **Count matches only** (`-c`).
- **Recursive directory search** (`-r`).
- **Debug mode** to print raw input (`-d`).
- **Read from `stdin`** if no file is provided.

---

## Build

Use the provided `Makefile`:

```bash
make
```
this will procude the binary: `./my_grep`


--- 

## Usage

``` bash
./my_grep [options] <pattern> [file_or_dir]
```

## Examples

```bash
echo "Hello world" | ./my_grep Hello bash
```

```bash
echo "foo123 bar42 baz" | ./my_grep -regex '[a-z]+[0-9]+'
```

```bash
./my_grep -r -i tOdO ./src
```

```bash
./my_grep -c pattern file.txt
```

![image](/screenshot_1757429256.png)

---

## Project Structure
```
.
├── main.c         # entry point
├── Makefile
├── my_grep        # compiled binary
├── utils/         # regex/string search, file handling, ...
│   ├── utils.c
│   └── utils.h
└── structs/       # generic containers (X-macro vectors and pairs)
    ├── vector.c
    ├── vector.h
    └── pair.h
```
