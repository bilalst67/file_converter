# File Converter in C++

This project is a C++ application that converts text files into **CSV, JSON, XML, and YML** formats.

## Features

  - 📁 **Graphical file selection interface**: A user-friendly interface for choosing files.
  - 🔄 **Multi-format support**: Conversion from TXT to CSV, JSON, XML, and YML formats.
  - 🎯 **Automatic data type detection**: Automatically recognizes numeric (`123`) and boolean (`true`, `false`) values within strings.
  - 📊 **List support**: Automatically processes values separated by a `"-"` character as a list/array.
  - 🏗️ **Hierarchical data structure support**: Creates nested data structures using `.` notation.

## ERROR

There is only a comma error in the json file, it puts one extra comma

## Installation

### Requirements

  - A compiler that supports C++17 (GCC, Clang, MSVC)
  - CMake (Recommended for project management)
  - vcpkg (For package management)

### Library Installation

#### 1\. fmt Library

  - **With vcpkg:**
    ```bash
    vcpkg install fmt
    ```
  - **Linux (Debian/Ubuntu):**
    ```bash
    sudo apt-get install libfmt-dev
    ```
  - **Linux (Fedora):**
    ```bash
    sudo dnf install fmt-devel
    ```
  - **macOS (Homebrew):**
    ```bash
    brew install fmt
    ```

#### 2\. tinyfiledialogs Library

  - **With vcpkg:**
    ```bash
    vcpkg install tinyfiledialogs
    ```
  - **Manual Installation:**
    1.  Download the source code from [tinyfiledialogs](https://sourceforge.net/projects/tinyfiledialogs/).
    2.  Add the `tinyfiledialogs.h` and `tinyfiledialogs.c` files to your project's main directory.

### Example CMakeLists.txt

```cmake
cmake_minimum_required(VERSION 3.10)
project(TextConverter)

set(CMAKE_CXX_STANDARD 17)

# Find libraries
find_package(fmt REQUIRED)

# Manually added source files
set(MANUAL_SOURCES
    tinyfiledialogs.c
    main.cpp
)

# Create the executable
add_executable(TextConverter ${MANUAL_SOURCES})

# Link libraries
target_link_libraries(TextConverter PRIVATE fmt)

# Compiler settings
target_compile_features(TextConverter PRIVATE cxx_std_17)
```

## Building

  - **With CMake (Recommended):**

    ```bash
    mkdir build
    cd build
    cmake ..
    make
    ```

  - **Manual Compilation (GCC):**

    ```bash
    g++ -std=c++17 -I. -o TextConverter main.cpp tinyfiledialogs.c -lfmt
    ```

## Usage

### Basic Usage

1.  Run the program.
2.  Select a `.txt` file from the file dialog that opens.
3.  Choose the format you want to convert to (`csv`, `json`, `xml`, `yml`).
4.  Enter a name for the output file.
5.  Specify any additional parameters if necessary.

### Input Formats

#### Hierarchical Structure for JSON/XML/YML

Keys separated by a dot (`.`) create a hierarchical structure.

**Input (`.txt`):**

```text
persons.person.name.yusuf
persons.person.skill.python
persons.person.language.Türkçe
companys.company.name.paperworks
companys.company.founder.name.Yusuf
companys.company.founder.skill.python-renpy
```

**Example Output (JSON):**
 [Output json](./file_converter/result.json)

#### Table Structure for CSV

The first line is treated as the header, and subsequent lines are treated as data. Values are separated by spaces.

**Input (`.txt`):**

```text
isim yas sehir
ahmet 25 istanbul
ayse 30 ankara
mehmet 35 izmir
```

**Example Output (CSV):**

```csv
isim,yas,sehir
ahmet,25,istanbul
ayse,30,ankara
mehmet,35,izmir
```

### List Support

Values separated by a `"-"` character are automatically processed as a list (array).

**Input (`.txt`):**

```text
skills.programming.python-c++-java
```

**Example Output (JSON):**
 [Output json](./file_converter/result.json)

## Functions

  - `txt_to_csv(file, output_name, separation_count)`
      - Converts a TXT file to CSV format. The `separation_count` parameter determines after how many spaces a new line should be created.
  - `txt_to_json(file, output_name)`
      - Converts a TXT file to JSON format. Automatically detects types like numbers and booleans.
  - `txt_to_xml(file, output_name)`
      - Converts a TXT file to XML format, preserving the hierarchical structure.
  - `txt_to_yml(file, output_name)`
      - Converts a TXT file to the indentation-based YML format.

### Helper Functions

  - `trim()`: Removes leading and trailing whitespace from a string.
  - `split()`: Splits a string by a specified delimiter.
  - `is_number()`: Checks if a string is a number.
  - `is_bool()`: Checks if a string is a boolean (`true`/`false`) value.
  - `indent()`: Generates spaces according to a given indentation level.

## Platform Support

  - ✅ Windows
  - ✅ Linux
  - ✅ macOS

## Contributing

Contributions will make this project even better\!

1.  Fork the Project.
2.  Create your Feature Branch (`git checkout -b feature/AmazingFeature`).
3.  Commit your Changes (`git commit -m 'Add some AmazingFeature'`).
4.  Push to the Branch (`git push origin feature/AmazingFeature`).
5.  Open a Pull Request.

## 🙏 Acknowledgements

This project benefits from the following open-source libraries:

### 🎯 tinyfiledialogs

  - **Author:** Milan Nikolic (gen2brain)
  - **Purpose:** Cross-platform file selection dialogs.
  - **License Type:** Custom Open Source License

**Important Note:** This library is provided "as is" without any warranty. The original license text is preserved in the project files.

### 🚀 fmt Library

  - **Purpose:** Modern C++ formatting and printing operations.
  - **License Type:** Apache License 2.0

**Apache 2.0 Summary:**

  - Permits commercial use
  - Permits modification
  - Permits distribution
  - Provides patent use
  - Requires attribution

> *For the full license texts, please see the `LICENSES/` directory.*
