#include "weather.h"
#include <iostream>
#include <sstream>
#include <Poco/URI.h>
#include <Poco/Net/ServerSocket.h>
#include <Poco/Net/HTTPClientSession.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/Dynamic/Var.h>
#include <Poco/Dynamic/Struct.h>
#include <Poco/JSON/Parser.h>
#include <Poco/JSON/Object.h>

class Forecast : public IForecaster {
public:
    Forecast(std::string api_key, Poco::URI& uri) : api_key_(api_key), uri_(uri){};
    WeatherForecast ForecastWeather(std::optional<Location> where = std::nullopt) override;

private:
    std::string api_key_;
    Poco::URI uri_;
};

WeatherForecast Forecast::ForecastWeather(std::optional<Location> where) {

    if (where != std::nullopt && where.has_value()) {
        Location loc = where.value();
        uri_.addQueryParameter("lat", std::to_string(loc.lat));
        uri_.addQueryParameter("lon", std::to_string(loc.lon));
    }

    Poco::Net::HTTPClientSession session{uri_.getHost(), uri_.getPort()};
    session.setKeepAlive(true);

    Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_GET, uri_.toString(),
                                   Poco::Net::HTTPMessage::HTTP_1_1);
    request.add("X-Yandex-API-Key", api_key_);
    // std::cout << "URI: " << request.getURI() << std::endl;
    session.sendRequest(request);

    Poco::Net::HTTPResponse response;
    std::istream& responseStream = session.receiveResponse(response);
    std::string responseData;

    if (response.getStatus() == Poco::Net::HTTPResponse::HTTP_OK) {
        // std::cout << "GET request succeeded (200 OK)" << std::endl;
        std::stringstream ss;
        ss << responseStream.rdbuf();
        responseData = ss.str();
        Poco::JSON::Parser parser;
        Poco::Dynamic::Var result = parser.parse(responseData);
        Poco::JSON::Object::Ptr object = result.extract<Poco::JSON::Object::Ptr>();
        Poco::DynamicStruct ds = *object;

        double temp = ds["fact"]["temp"];
        double feels_like = ds["fact"]["feels_like"];

        return WeatherForecast{temp, feels_like};
    } else if (response.getStatus() == Poco::Net::HTTPResponse::HTTP_BAD_REQUEST) {
        // std::cout << "GET request failed with status code: " << response.getStatus() <<
        // std::endl;
        throw YandexAPIError(response.getStatus(), "Bad Request");
    }
    return WeatherForecast{0, 0};
}

std::unique_ptr<IForecaster> CreateYandexForecaster(const std::string& api_key,
                                                    const std::string& api_endpoint) {

    Poco::URI uri{api_endpoint};
    uri.setScheme(Poco::Net::HTTPRequest::HTTP_GET);
    uri.setPath("/v1/forecast");

    return std::make_unique<Forecast>(api_key, uri);
}