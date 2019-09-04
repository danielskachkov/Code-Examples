from flask import Flask, render_template, request, redirect
import matplotlib.pyplot as plt
import requests
from datetime import datetime
import os

key_for_maps = 'AIzaSyDzuRmXsdWTdstUB7wdu7cYyJUUA2EpraA'
key_for_weather = 'a3127c5ff6121e6ff1b9f698bec823ea'

app = Flask(__name__)
app.config['TEMPLATES_AUTO_RELOAD'] = True

def F_to_C(F):
    return (F-32)*5/9

@app.route('/forecast', methods=['POST'])
def forecast():
    weekdays = ['Mon', 'Tue', 'Wns', 'Th', 'Fr', 'Sat', 'Sn']

    city_name = request.form['city']
    clear = request.form.get('clear')
    
    try:
        r1 = requests.get('https://maps.googleapis.com/maps/api/geocode/json?address={}&key={}'.format(city_name, key_for_maps))

        map_json = r1.json()
        assert map_json['status'] != 'ZERO_RESULTS'
        
        lat = map_json["results"][0]["geometry"]["location"]["lat"]
        lng = map_json["results"][0]["geometry"]["location"]["lng"]
    
    except AssertionError:
        return render_template('incorrect_input.html')
    
    r2 = requests.get('https://api.darksky.net/forecast/{}/{},{}'.format(key_for_weather, str(lat), str(lng)))
    json_obj = r2.json()
    
    max_tmp = [round(F_to_C(json_obj["daily"]["data"][i]["temperatureMax"]), 1) for i in range(1, 8)]
    min_tmp = [round(F_to_C(json_obj["daily"]["data"][i]["temperatureMin"]), 1) for i in range(1, 8)]
    dt = [datetime.fromtimestamp(json_obj["daily"]["data"][i]["time"]).strftime('%Y-%m-%d') for i in range(1, 8)]
        
    daily_values = zip(max_tmp, min_tmp, dt)
    
    plt.clf()
    y1 = max_tmp
    y2 = min_tmp
    x = []
    for i in range(0, 7):
        d = dt[i].split('-')
        x.append(weekdays[datetime(int(d[0]), int(d[1]), int(d[2])).weekday()])
    
    plt.plot(x, y1)
    plt.plot(x, y2)
    plt.ylim(0, 50)

    plt.xlabel('day')
    plt.ylabel('temp')
    plt.title('Temperature Graphic')
    plt.grid(False)
    plt.savefig('static/temperature.png', fmt='png')
    
    mapsrc = 'https://static-maps.yandex.ru/1.x/?ll={},{}&z=7&l=map'.format(str(lng), str(lat))
    
    return render_template('forecast.html', daily_val = daily_values, city=city_name, map_src=mapsrc)
    

@app.route('/')
def search():
    return render_template('search.html')

if __name__ == "__main__":
    if not os.path.exists('static'):
        os.makedirs('static')
    app.run(host='0.0.0.0', debug=True)
