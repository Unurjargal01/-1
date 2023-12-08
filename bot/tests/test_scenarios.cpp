#include <tests/test_scenarios.h>
#include <iostream>
#include <catch2/catch_test_macros.hpp>
#include <sstream>
#include <Poco/Net/HTTPSClientSession.h>
#include <Poco/Net/HTTPSClientSession.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/URI.h>
#include <Poco/JSON/Parser.h>
#include <Poco/JSON/Object.h>
#include <Poco/StreamCopier.h>
#include <telegram/api.h>
void TestSingleGetMe(std::string_view url) {
    auto bot = CreateApi("123", url);
    auto info = bot->GetMe();

    REQUIRE(info.id == 1234567);
    REQUIRE(info.is_bot);
    REQUIRE(info.username == "test_bot");
    REQUIRE(info.first_name == "Test Bot");
}

void TestGetMeErrorHandling(std::string_view url) {
    auto bot = CreateApi("123", url);
    try {
        auto info = bot->GetMe();
    } catch (APIError& err) {
        REQUIRE(err.http_code == 500);
        // std::cout << err.http_code << " " << err.details << std::endl;
    }
    try {
        auto info = bot->GetMe();
    } catch (APIError& err) {
        REQUIRE(err.http_code == 401);
        // std::cout << err.http_code << " " << err.details << std::endl;
    }
}

void TestSingleGetUpdatesAndSendMessages(std::string_view url) {
    auto bot = CreateApi("123", url);
    std::vector<Update> updates = bot->GetUpdates();
    bot->SendMessage(updates[0].id, "Hi!");
    bot->SendMessage(updates[1].id, "Reply", updates[1].message_id);
    bot->SendMessage(updates[1].id, "Reply", updates[1].message_id);
}

void TestHandleGetUpdatesOffset(std::string_view url) {
    auto bot = CreateApi("123", url);
    std::vector<Update> updates;
    updates = bot->GetUpdates(5);
    REQUIRE(updates.size() == 2);
    updates.clear();
    updates = bot->GetUpdates(5, bot->UpdateId() + 1);
    updates = bot->GetUpdates(5, bot->UpdateId() + 1);
}