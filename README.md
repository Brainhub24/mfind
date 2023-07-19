```
 __    __     ______   __     __   __     _____    
/\ "-./  \   /\  ___\ /\ \   /\ "-.\ \   /\  __-.  
\ \ \-./\ \  \ \  __\ \ \ \  \ \ \-.  \  \ \ \/\ \ 
 \ \_\ \ \_\  \ \_\    \ \_\  \ \_\\"\_\  \ \____- 
  \/_/  \/_/   \/_/     \/_/   \/_/ \/_/   \/____/ 
                                                   
```

**mfind** is a command-line utility for searching and listing currently only in MKV (Matroska Video) files in a specified directory or drive.
It provides a convenient way to quickly find and view information about MKV files, such as path, file type, size, inode, and date/time.

## Features

- Search for MKV files recursively in a specified directory or drive
- Print detailed information about each found MKV file
- Optional logging of MKV files to a log file with date and time

## Futured Upgrades:
- High availability encryption methods
- gpg, zip, gzip & bz2 Encryption Methods
- Text filter & extraction methods
- Media file analysis (EXIF)

## Usage

To search for MKV files in a specific directory:
```
mfind /path/to/directory
```

To search for MKV files in the default directory (home directory):
```
mfind -d
```

To enable logging of MKV files to a log file:
```
mfind -log logfile.txt /path/to/directory
```

Installation
To compile and install the mfind utility, follow these steps.
Clone the repository or download the source code:
```
git clone https://github.com/your-username/mfind.git
```

Navigate to the mfind directory:
```
cd mfind
```

Compile the code using the provided Makefile:
```
make
```

This will generate the mfind executable.

### Run the mfind utility with the desired command-line options.

# Usage Examples
To search for MKV files in a specific directory:
```
./mfind /path/to/directory
```

To search for MKV files in the default directory (home directory):
```
./mfind -d
```

To enable logging of MKV files to a log file:
```
./mfind -log logfile.txt /path/to/directory
```

### Note: Make sure to replace /path/to/directory with the actual path to the directory you want to search.

## Dependencies
```
C compiler (e.g., GCC)
GNU Make
```
Make sure you have the necessary dependencies installed before compiling the code.

## License
This project is licensed under the GNU GENERAL PUBLIC LICENSE | Version 3.

## Contributing
Contributions are welcome!

If you encounter any issues or have suggestions for improvements, please feel free to open an issue or submit a pull request.
