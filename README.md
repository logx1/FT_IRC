# FT_IRC

A simple Internet Relay Chat (IRC) server implementation in C++.

## Description

This project is a basic IRC server that can handle multiple clients. It's built using C++98 and uses the `poll()` system call to manage connections. The server listens for incoming connections on a specified port and requires a password for clients to connect.

## Features

*   Handles multiple clients simultaneously using `poll()`.
*   Client registration with `NICK`, `USER`, and `REALNAME` commands.
*   Basic message receiving from clients.

## How to Build

To build the server, you can use the provided `Makefile`.

1.  **Clone the repository:**
    ```bash
    git clone <repository-url>
    cd <repository-directory>
    ```

2.  **Compile the project:**
    ```bash
    make
    ```
    This will create an executable named `FT_IRC`.

The `Makefile` also provides the following rules:
*   `make clean`: Removes the object files.
*   `make fclean`: Removes the object files and the `FT_IRC` executable.
*   `make re`: Rebuilds the project from scratch.

## How to Run

To run the server, execute the compiled binary with a port number and a password as arguments:

```bash
./FT_IRC <port> <password>
```

*   `<port>`: The port number the server will listen on (e.g., 6667).
*   `<password>`: The password required for clients to connect.

Example:
```bash
./FT_IRC 6667 mysecretpassword
```

## Usage

Once the server is running, you can connect to it using an IRC client (like `irssi`, `hexchat`, or even `netcat`).

To register with the server, the client must send the following commands:

1.  **Set a nickname:**
    ```
    NICK <your_nickname>
    ```

2.  **Set a username and realname:**
    ```
    USER <username> 0 * :<realname>
    ```
    *Note: The `client.cpp` implementation seems to look for `REALNAME` as a separate command, which is non-standard. A standard IRC client will send the realname as part of the `USER` command. For compatibility with this server, you might need to send `REALNAME <your_realname>` if a standard client doesn't work.*

3.  **Provide the password (if required by the client):**
    ```
    PASS <password>
    ```

After sending these commands, the client should be registered on the server.
