#include <Poco/Net/HTTPClientSession.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/URI.h>
#include <Poco/StreamCopier.h>
#include <Poco/JSON/Parser.h>
#include <Poco/Dynamic/Var.h>
#include <iostream>
#include <sstream>
#include <string>
#include <memory>
#include <random>

struct InfoBot {
    int64_t id;
    bool is_bot;
    std::string username;
    std::string first_name;
};

struct Update {
    int64_t id = 0;
    int64_t message_id;
    int64_t update_id;
    std::string username;
    std::string text;
    bool cmd = false;
    std::string cmd_ans;
};

struct APIError : std::runtime_error {
    APIError(int http_code, std::string details)
        : std::runtime_error{"api error: code=" + std::to_string(http_code) +
                             " details=" + details},
          http_code{http_code},
          details{std::move(details)} {
    }
    int http_code;
    std::string details;
};

class API {
public:
    virtual ~API() = default;
    virtual InfoBot GetMe() = 0;
    virtual std::vector<Update> GetUpdates(int timeout = 0, int offset = 0) = 0;
    virtual std::istream& SendMessage(int64_t id, std::string text, int64_t val_reply = 0) = 0;
    virtual int64_t UpdateId() = 0;
    int64_t RandomGenerator() {
        std::random_device rd;
        std::mt19937 generator(rd());

        int64_t min_number = 1;
        int64_t max_number = 100;

        std::uniform_int_distribution<int64_t> dist(min_number, max_number);
        return dist(generator);
    }
};

std::unique_ptr<API> CreateApi(const std::string& id, std::string_view& url);