#include "Router.hpp"

void Router::get(const std::string& path, std::function<httpResponse(const httpRequest&)> handler) {
    routes[path] = handler;
}

httpResponse Router::handle(const httpRequest& req) {
    auto it = routes.find(req.path);

    if (it != std::end(routes)) {
        return it->second(req);
    }

    httpResponse notFound;
    notFound.statusCode = 404;
    notFound.statusText = "Not Found";
    notFound.contentType = "text/plain";
    notFound.body = "404 - Not Found";
    return notFound;
}