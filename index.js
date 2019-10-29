var express = require("express");
var app = express();

app.get("/data", (req, res, next) => {
	console.log("Esta es mi temperatura: ");
	console.log(req.params.temp);
 res.json(["Tony","Lisa","Michael","Ginger","Food"]);
});

app.listen(3000, () => {
	console.log("Server running on port 3000");
});