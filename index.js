var express = require("express");
var app = express();
var colors = require('colors')
var DBConfig = require ('./databaseConnection.json')
const { Client } = require('pg')
const http = require('http');

const DBClient = new Client(DBConfig.roomTemp);

let port = 80

DBClient.connect()
	.then((data, err)=>{
		console.log('Conected to database: '.green)
	})
	.catch((data, err) => {
		console.log(err)
		console.log(data)
	})

app.get("/data", (req, res, next) => {
	console.log("Esta es mi temperatura: ");
	console.log(req.query.temp);
	//console.log(req.query);
	http.get('http://dataservice.accuweather.com/currentconditions/v1/307144?apikey=6MNOrGmD9u5PSldlVn5Kgyijp4BCQ5lX&details=true', (resp) => {
		let data = '';

		// A chunk of data has been recieved.
		resp.on('data', (chunk) => {
			data += chunk;
		});
		// The whole response has been received. Print out the result.
		resp.on('end', () => {
			console.log('Tengo respuesta')
			//console.log(JSON.parse(data));
			respuesta = JSON.parse(data)
			console.log(respuesta[0].LocalObservationDateTime)
			DBClient.query('insert into register ' + '(heating, roomtemp, isdaytime, humidity, temp, windDir, windSpeed, UV, precipitation, cloudCover, moment) values ($1, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11)', [req.query.temp, false, respuesta[0].IsDayTime, respuesta[0].RelativeHumidity, respuesta[0].Temperature.Metric.Value, respuesta[0].Wind.Direction.Degrees, respuesta[0].Wind.Speed.Metric.Value, respuesta[0].UVIndex, respuesta[0].PrecipitationSummary.Precipitation.Metric.Value, respuesta[0].CloudCover, respuesta[0].LocalObservationDateTime], (err, res) => {
				if(err){
					logToFile('Error insertando en la base de datos: ' + err + '|' + 'insert into ' + product_id.replace('-', '_') + ' (id, price, moment, amount, side) values ($1, $2, $3, $4, $5)',trade_id, price, time, size, side)
					console.log('Error insertando en la base de datos: ' + err)
					console.log('insert into ' + product_id.replace('-', '_') + ' (id, price, moment, amount, side) values ($1, $2, $3, $4, $5)', trade_id, price, time, size, side)
				}
				else {
					console.log('No problemo')
				    //console.log(res.rows)
				//client.end()
				}
			})
		});

	}).on("error", (err) => {
		console.log("Error: " + err.message);
	});
	res.json("La cosa funciona");
});

app.listen(port, () => {
	console.log("Server running on port " + port);
});
