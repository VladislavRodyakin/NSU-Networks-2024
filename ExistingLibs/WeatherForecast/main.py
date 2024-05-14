import requests
import matplotlib.pyplot as plt
from datetime import datetime

location_api_url = {
    'Novosibirsk': 'https://api.open-meteo.com/v1/forecast?latitude=55.0415&longitude=82.9346&past_days=7&hourly=temperature_2m&forecast_days=1',
    'Moscow': 'https://api.open-meteo.com/v1/forecast?latitude=55.7522&longitude=37.6156&past_days=7&hourly=temperature_2m&forecast_days=1',
    'London': 'https://api.open-meteo.com/v1/forecast?latitude=51.5074&longitude=-0.1278&past_days=7&hourly=temperature_2m&forecast_days=1',
    'Rostov-on-Don': 'https://api.open-meteo.com/v1/forecast?latitude=47.2357&longitude=39.7186&past_days=7&hourly=temperature_2m&forecast_days=1'
}

def getData(location_api_url_list):
    api_num = 1
    json_data = ''
    for location in location_api_url_list.keys():
        try:
            req_response = requests.get(location_api_url_list[location])
            if req_response.status_code != 200:
                print('Unable to get data. Response error occurred:', req_response.status_code)
            json_data = req_response.json()
        except requests.exceptions.RequestException as req_err:
            print('Request error occurred:', req_err)

        dates_times = json_data['hourly']['time']
        temp = json_data['hourly']['temperature_2m']
        modified_dates = []

        for date_time in dates_times:
            date_obj = datetime.strptime(date_time, '%Y-%m-%dT%H:%M')
            modified_dates.append(date_obj.strftime('%d.%m'))

        modified_dates = sorted(list(set(modified_dates)))


        plt.subplot(1, len(location_api_url.keys()), api_num)
        plt.plot(dates_times, temp)
        plt.xticks(dates_times[::24], modified_dates)
        plt.xlabel('Date', fontsize=12)
        plt.ylabel('Temperature, C', fontsize=12)
        plt.title(location, fontsize=12)
        api_num += 1


fig = plt.figure(figsize=(15, 10))

getData(location_api_url)

plt.show()