var express = require("express");
var app = express();
var DBConfig = require ('./databaseConnection.json')
const { Client } = require('pg')

const DBClient = new Client(DBConfig.roomTemp);

DBClient.connect()
	.then((data, err)=>{
		console.log('Conected to database: '.green)
		logToFile('Conected to database: '.green)
	})
	.catch((data, err) => {
		console.log(err);
		console.log(data)
	})

app.get("/data", (req, res, next) => {
	console.log("Esta es mi temperatura: ");
	console.log(req.query.temp);
	console.log(req.query);
 res.json(["Tony","Lisa","Michael","Ginger","Food"]);
});

app.listen(3000, () => {
	console.log("Server running on port 3000");
});