#include "UserInterface.h"

#include <string>
#include <conio.h>


UserInterface::UserInterface()
{
	cities = vector<string>();
	loadFutures = vector<future<void>>();

	
}

UserInterface::~UserInterface()
{
}

void UserInterface::GetCitiesNames()
{
	bool keepAsking = true;

	while (keepAsking)
	{
		string cityName;

		cout << "Enter the name of the city: ";

		getline(cin, cityName);

		cities.push_back(cityName);

		system("cls");

		char key{};

		cout << "Do you want to add another city? Y/N: " << endl;

		while (key != 'Y' && key != 'N')
		{
			key = _toupper(_getch());
		}

		if (key == 'N')
		{
			keepAsking = false;
		}

		system("cls");
	}
}

void UserInterface::GetWeatherConditions(string cityName)
{
	uri_builder uriBuilder = uri_builder(to_string_t("/weather"));

	uriBuilder.append_query(to_string_t("q"), to_string_t(cityName));
	uriBuilder.append_query(to_string_t("appid"), to_string_t(API_KEY));

	http_request request = http_request(methods::GET);

	request.set_request_uri(uriBuilder.to_string());

	task<http_response> responseTask = httpClient.request(request);
	
	responseTask.wait();
	http_response response = responseTask.get();

	cout << "Loading weather..." << endl;

	if (response.status_code() != 200)
	{
		cout << "Can't find city: " << cityName << endl;
	}
	else
	{
		json::value json = response.extract_json().get();

		string cityName = to_utf8string(json[to_string_t("name")].as_string());

		string weather = to_utf8string(json[to_string_t("weather")][0][to_string_t("main")].as_string());

		float temperature = json[to_string_t("main")][to_string_t("temp")].as_double();
		temperature -= 273.15f;

		float feelsLike = json[to_string_t("main")][to_string_t("feels_like")].as_double();
		feelsLike -= 273.15f;

		float humidity = json[to_string_t("main")][to_string_t("humidity")].as_double();

		cout << "Weather in " << cityName << endl << endl;

		cout << "Weather: " << weather << endl;
		cout << "Temperature: " << temperature << " C" << endl;
		cout << "Feels like: " << feelsLike << " C" << endl;
		cout << "Humidity : " << humidity << "%" << endl << endl << endl;
	}

	this_thread::sleep_for(chrono::milliseconds(1000));
}

void UserInterface::LoadFutures()
{
	
	for (int i = 0; i < cities.size(); i++)
	{
		loadFutures.push_back(async(launch::async, [this, i] {this->GetWeatherConditions(cities[i]);}));
	}
}


