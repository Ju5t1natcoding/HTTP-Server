#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include "httpResponse.hpp"
#include "httpRequest.hpp"
#include "Router.hpp"
#include "FileServer.hpp"
#include "ThreadPool.hpp"

int main() {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(8080);
    bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));
    listen(serverSocket, 5);
    std::cout << "Server started on port 8080...\n";

    Router router;

    router.get("/ping", [](const httpRequest& req) {
        httpResponse resp;
        resp.statusCode = 200;
        resp.statusText = "OK";
        resp.contentType = "text/plain";
        resp.body = "pong";
        return resp;
    });

    router.get("/hello", [](const httpRequest& req) {
        httpResponse resp;
        resp.statusCode = 200;
        resp.statusText = "OK";
        resp.contentType = "text/html";
        resp.body = "<h1>Hello from my server!</h1>";
        return resp;
    });

    ThreadPool pool(4);

    while (true) {
        SOCKET clientSocket = accept(serverSocket, nullptr, nullptr);
        
        pool.enqueue([clientSocket, &router] {
            char buf[1024] = {};
            recv(clientSocket, buf, sizeof(buf), 0);

            httpRequest req = httpRequest::parse(buf);
            std::cout << "[" << req.method << "] " << req.path << "\n";

            httpResponse resp = router.handle(req);

            if (resp.statusCode == 404) {
                resp = FileServer::serve(req.path);
            }

            std::string raw = resp.build();
            send(clientSocket, raw.c_str(), raw.size(), 0);
            closesocket(clientSocket);
        });
    }

    WSACleanup();
}