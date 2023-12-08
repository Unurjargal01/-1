#include <Poco/URI.h>
#include <Poco/JSON/Parser.h>
#include <Poco/JSON/Object.h>
#include <Poco/Net/HTTPSClientSession.h>
#include <Poco/Net/HTTPSClientSession.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>

#include <iostream>
#include <sstream>
#include <memory>
#include <telegram/api.h>

class Bot : public API {
public:
    Bot(Poco::URI& uri) : uri_(uri), last_update_id_(0){};
    InfoBot GetMe() override;
    std::vector<Update> GetUpdates(int timeout = 0, int offset = 0) override;
    std::istream& SendMessage(int64_t id, std::string text, int64_t val_reply = 0) override;
    int64_t UpdateId() override {
        return last_update_id_;
    }

private:
    Poco::URI uri_;
    int64_t last_update_id_;
    std::vector<Update> must_answer_;
};

InfoBot Bot::GetMe() {
    Poco::Net::HTTPClientSession session{uri_.getHost(), uri_.getPort()};
    session.setKeepAlive(true);

    Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_GET, uri_.getPath() + "/getMe",
                                   Poco::Net::HTTPMessage::HTTP_1_1);
    session.sendRequest(request);
    Poco::Net::HTTPResponse response;
    std::istream& body = session.receiveResponse(response);
    InfoBot info;
    if (response.getStatus() == Poco::Net::HTTPResponse::HTTP_OK) {
        Poco::JSON::Parser parser;
        auto reply = parser.parse(body);
        Poco::JSON::Object::Ptr json_object = reply.extract<Poco::JSON::Object::Ptr>();

        auto result = json_object->getObject("result");

        info.username = result->getValue<std::string>("username");
        info.first_name = result->getValue<std::string>("first_name");
        info.is_bot = result->getValue<bool>("is_bot");
        info.id = result->getValue<int64_t>("id");
    } else {
        throw APIError(response.getStatus(), "Bad Request");
    }
    return info;
};

std::vector<Update> Bot::GetUpdates(int timeout, int offset) {
    std::vector<Update> ans;
    Poco::Net::HTTPClientSession session{uri_.getHost(), uri_.getPort()};
    session.setKeepAlive(true);
    std::string path = uri_.getPath();

    path += "/getUpdates";
    if (offset != 0) {
        path += "?offset=" + std::to_string(offset);
    }
    if (offset != 0 && timeout != 0) {
        path += "&timeout=" + std::to_string(timeout);
    } else if (offset == 0 && timeout != 0) {
        path += "?timeout=" + std::to_string(timeout);
    }
    Poco::Net::HTTPResponse response;
    Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_GET, path,
                                   Poco::Net::HTTPMessage::HTTP_1_1);
    session.sendRequest(request);
    std::istream& body = session.receiveResponse(response);

    if (response.getStatus() == Poco::Net::HTTPResponse::HTTP_OK) {
        Poco::JSON::Parser parser;
        auto reply = parser.parse(body);
        Poco::JSON::Object::Ptr json_object = reply.extract<Poco::JSON::Object::Ptr>();
        Poco::JSON::Array::Ptr messages = json_object->getArray("result");
        for (size_t i = 0; i < messages->size(); ++i) {
            Poco::JSON::Object::Ptr message = messages->getObject(i);
            Update update_msg;
            update_msg.update_id = message->getValue<int64_t>("update_id");
            last_update_id_ = std::max(last_update_id_, update_msg.update_id);
            if (message->has("message")) {
                Poco::JSON::Object::Ptr msg = message->getObject("message");
                if (msg->has("chat")) {
                    update_msg.id = msg->getObject("chat")->getValue<int64_t>("id");
                }
                if (msg->has("message_id")) {
                    update_msg.message_id = msg->getValue<int64_t>("message_id");
                }
                if (msg->has("username")) {
                    update_msg.username = msg->getObject("from")->getValue<std::string>("username");
                }
                if (msg->has("text")) {
                    update_msg.text = msg->getValue<std::string>("text");
                }
                // Requirements of bots;
                if (update_msg.text == "/random") {
                    update_msg.cmd = true;
                    update_msg.cmd_ans = std::to_string(RandomGenerator());
                    must_answer_.push_back(update_msg);
                } else if (update_msg.text == "/weather") {
                    update_msg.cmd = true;
                    update_msg.cmd_ans = "Winter Is Coming";
                    must_answer_.push_back(update_msg);
                } else if (update_msg.text == "/styleguide") {
                    update_msg.cmd = true;
                    update_msg.cmd_ans = "I am German";
                    must_answer_.push_back(update_msg);
                } else if (update_msg.text == "/stop") {
                    break;
                } else if (update_msg.text == "/abort") {
                    std::abort();
                }
            }
            ans.push_back(update_msg);
        }
    } else {
        throw APIError(response.getStatus(), "Bad Request");
    }
    return ans;
}

std::istream& Bot::SendMessage(int64_t id, std::string text, int64_t val_reply) {
    std::string path = uri_.getPath();
    path += "/sendMessage";
    Poco::Net::HTTPClientSession session{uri_.getHost(), uri_.getPort()};
    session.setKeepAlive(true);

    Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_POST, path,
                                   Poco::Net::HTTPMessage::HTTP_1_1);
    request.setContentType("application/json");
    std::stringstream ss;
    ss << "{\"chat_id\": \"" << std::to_string(id) << "\", \"text\": \"" << text << "\"";
    if (val_reply != 0) {
        ss << ", \"reply_to_message_id\": \"" << std::to_string(val_reply) << "\"}";
    } else {
        ss << "}";
    }
    request.setContentLength(ss.str().length());
    std::ostream& os = session.sendRequest(request);
    os << ss.str();
    Poco::Net::HTTPResponse response;
    std::istream& body = session.receiveResponse(response);
    return body;
}

std::unique_ptr<API> CreateApi(const std::string& id, std::string_view& url) {
    Poco::URI uri{std::string{url}};
    uri.setScheme(Poco::Net::HTTPRequest::HTTP_GET);
    uri.setPath("/bot" + id);

    return std::make_unique<Bot>(uri);
}