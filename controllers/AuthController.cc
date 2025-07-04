#pragma once

#include <drogon/HttpSimpleController.h>
#include <json/json.h>
#include <jwt-cpp/jwt.h>

using namespace drogon;

class AuthController : public drogon::HttpSimpleController<AuthController> {
public:
    virtual void asyncHandleHttpRequest(const HttpRequestPtr& req,
                                        std::function<void(const HttpResponsePtr&)>&& callback) override {
        if (req->method() == Post) {
            Json::Value reqJson;
            Json::Reader reader;
            std::string bodyStr = std::string(req->body());
            reader.parse(bodyStr, reqJson);
            std::string username = reqJson["username"].asString();
            std::string password = reqJson["password"].asString();

            // Dummy auth logic
            if (username == "admin" && password == "pass") {
                std::string token = jwt::create()
                    .set_issuer("auth")
                    .set_payload_claim("username", jwt::claim(username))
                    .set_expires_at(std::chrono::system_clock::now() + std::chrono::hours(1))
                    .sign(jwt::algorithm::hs256{"secret"});

                Json::Value respJson;
                respJson["token"] = token;
                auto resp = HttpResponse::newHttpJsonResponse(respJson);
                callback(resp);
                return;
            }
        }

        auto resp = HttpResponse::newHttpResponse();
        resp->setStatusCode(k401Unauthorized);
        callback(resp);
    }

    PATH_LIST_BEGIN
        PATH_ADD("/register", Post);
        PATH_ADD("/protected", Get, "JwtAuthFilter");
    PATH_LIST_END
};
