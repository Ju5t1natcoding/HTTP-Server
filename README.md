# HTTP Server in C++17

![C++](https://img.shields.io/badge/C++-17-blue)

## Contents

1. [About](#about)
2. [Features](#features)
3. [Project Structure](#project-structure)
4. [How to compile and run](#how-to-compile-and-run)
5. [Available routes](#available-routes)
6. [How it works internally](#how-it-works-internally)
7. [Running the tests](#running-the-tests)

## About

A lightweight HTTP/1.1 server built from scratch in C++17 using raw WinSock2 sockets, with no external libraries. Supports static file serving, custom routing, and parallel request handling via a thread pool.

## Features

- TCP connection handling via WinSock2
- HTTP/1.1 request parsing (method, path, version)
- Custom routing with lambda handlers
- Static file serving with automatic content-type detection
- Thread pool for parallel request processing
- Unit tests with doctest
- CI via GitHub Actions

## Project structure

```
HTTP-Server/
  main.cpp              — entry point, route definitions
  HttpRequest.hpp/cpp   — parses raw HTTP requests
  HttpResponse.hpp/cpp  — builds valid HTTP responses
  Router.hpp/cpp        — maps paths to handlers
  FileServer.hpp/cpp    — serves static files from disk
  ThreadPool.hpp/cpp    — processes connections in parallel
  static/               — static files served by the server
  tests/
    tests.cpp           — unit tests (doctest)
    doctest.h           — doctest header
  .github/workflows/
    build.yml           — CI pipeline (build + tests)
```

## How to compile and run

Requirements: g++ with C++17 support (MinGW/MSYS2 on Windows)

```
g++ -std=c++17 main.cpp HttpResponse.cpp HttpRequest.cpp Router.cpp FileServer.cpp ThreadPool.cpp -o server -lws2_32
./server
```

Server starts on port 8080. Open your browser at `http://localhost:8080`.

## Available routes

| Route | Method | Response |
|---|---|---|
| `/ping` | GET | `pong` |
| `/hello` | GET | HTML greeting |
| `/<file>` | GET | Serves file from `static/` folder |
| anything else | GET | `404 - Not Found` |

## How it works internally

**Request lifecycle:** Each incoming TCP connection is accepted by the main thread and immediately handed off to the thread pool. A worker thread reads the raw bytes, parses the HTTP request, routes it to the correct handler, builds the response, and sends it back.

**HTTP parsing:** The parser reads the first line of the request (`GET /path HTTP/1.1`) using `istringstream` to extract method, path, and HTTP version. Headers are ignored for now.

**Routing:** Routes are stored in an `unordered_map` mapping paths to lambda handlers. If no route matches, the server falls back to trying to serve a static file. If that also fails, it returns 404.

**Thread pool:** The pool is initialized with 4 worker threads. Each thread blocks on a `condition_variable` until a task appears in the queue. A `mutex` protects the queue from race conditions. This avoids busy waiting — threads sleep until there's actual work to do.

**Static files:** The file server prepends `static/` to the requested path and opens the file with `ifstream`. Content-type is determined by file extension.

## Running the tests

```
g++ -std=c++17 tests/tests.cpp HttpRequest.cpp HttpResponse.cpp Router.cpp -o tests/run_tests -lws2_32
./tests/run_tests
```

Expected output:

```
[doctest] test cases: 6 | 6 passed | 0 failed
```