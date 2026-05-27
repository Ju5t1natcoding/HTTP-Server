#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "../httpRequest.hpp"
#include "../httpResponse.hpp"
#include "../Router.hpp"

TEST_CASE("httpRequest parseaza GET corect") {
    std::string raw = "GET /ping http/1.1\r\nHost: localhost\r\n\r\n";
    httpRequest req = httpRequest::parse(raw);

    CHECK(req.method == "GET");
    CHECK(req.path == "/ping");
    CHECK(req.httpVersion == "http/1.1");
}

TEST_CASE("httpRequest parseaza path cu slash simplu") {
    std::string raw = "GET / http/1.1\r\nHost: localhost\r\n\r\n";
    httpRequest req = httpRequest::parse(raw);

    CHECK(req.path == "/");
}

TEST_CASE("httpResponse build produce status line corecta") {
    httpResponse resp;
    resp.statusCode = 200;
    resp.statusText = "OK";
    resp.contentType = "text/plain";
    resp.body = "pong";

    std::string raw = resp.build();
    CHECK(raw.find("HTTP/1.1 200 OK") != std::string::npos);
}

TEST_CASE("httpResponse contine body-ul corect") {
    httpResponse resp;
    resp.statusCode = 200;
    resp.statusText = "OK";
    resp.contentType = "text/plain";
    resp.body = "pong";

    std::string raw = resp.build();
    CHECK(raw.find("pong") != std::string::npos);
}

TEST_CASE("Router returneaza 200 pentru ruta existenta") {
    Router router;
    router.get("/ping", [](const httpRequest& req) {
        httpResponse resp;
        resp.statusCode = 200;
        resp.statusText = "OK";
        resp.contentType = "text/plain";
        resp.body = "pong";
        return resp;
    });

    httpRequest req;
    req.method = "GET";
    req.path = "/ping";

    httpResponse resp = router.handle(req);
    CHECK(resp.statusCode == 200);
    CHECK(resp.body == "pong");
}

TEST_CASE("Router returneaza 404 pentru ruta inexistenta") {
    Router router;

    httpRequest req;
    req.method = "GET";
    req.path = "/inexistent";

    httpResponse resp = router.handle(req);
    CHECK(resp.statusCode == 404);
}