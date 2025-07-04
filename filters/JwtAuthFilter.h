#pragma once
#include <drogon/HttpFilter.h>
#include <jwt-cpp/jwt.h>

using namespace drogon;

class JwtAuthFilter : public HttpFilter<JwtAuthFilter> {
public:
    void doFilter(const HttpRequestPtr &req,
                  FilterCallback &&fcb,
                  FilterChainCallback &&fccb) override {
        auto authHeader = req->getHeader("Authorization");
        if (authHeader.empty() || authHeader.substr(0, 7) != "Bearer ") {
            auto resp = HttpResponse::newHttpResponse();
            resp->setStatusCode(k401Unauthorized);
            fcb(resp);
            return;
        }

        std::string token = authHeader.substr(7);
        try {
            auto decoded = jwt::decode(token);
            auto verifier = jwt::verify().allow_algorithm(jwt::algorithm::hs256{"secret"}).with_issuer("auth");
            verifier.verify(decoded);
            req->attributes()->insert("jwt_payload", decoded.get_payload_json());
            fccb();
        } catch (...) {
            auto resp = HttpResponse::newHttpResponse();
            resp->setStatusCode(k401Unauthorized);
            fcb(resp);
        }
    }
};
