#pragma once

#include <vector>
#include <iostream>
#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
#include <chrono>
#include <future>

using namespace std;
using namespace web;
using namespace web::http;
using namespace web::http::client;
using namespace utility::conversions;
using namespace Concurrency;

struct Data
{
	string cityName;
	string weather;
	float temperature;
	float feelsLike;
	float humidity;
};

const string BASE_API_URL = "https://api.openweathermap.org/data/2.5";
const string API_KEY = "c6ffc4968b9b41b6ac0a45764763ee9f";

class UserInterface
{
private:
	vector<string> cities;
	vector<future<void>> loadFutures;
	vector<Data> data;
	mutex loadMutex;
	http_client httpClient = http_client(to_string_t(BASE_API_URL));



public:
	UserInterface();
	~UserInterface();

	void GetCitiesNames();
	void GetWeatherConditions(string cityName, Data& actualData);
	void LoadFutures();
	void ShowData();
};

