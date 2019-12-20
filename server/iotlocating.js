var express = require('express');
var app = express();
var port = 4003;

var request = require('request');

var bodyParser = require('body-parser');
var jsonParser = bodyParser.json()

var googleapisKey = '{YOUR-GOOGLE-API-KEY}';

app.get('/', function(req, res){
  res.status(200).jsonp({status:"ok"});
});

app.post('/getaroundwifimacaddress', jsonParser, function(req, res){
  console.log(req.body);
  console.log(req.body.datasets.length);
  var macDatasets = [];
  for(var i in req.body.datasets){
    macDatasets.push({
      macAddress: String(req.body.datasets[i].macAddress),
      signalStrength: Number(req.body.datasets[i].rssi),
      signalToNoiseRatio: Number(req.body.datasets[i].channel)
    });
  }
  findLatLng(macDatasets, res);
});

app.listen(port, function(){
  console.log('IoT locating protocol started !');
});

function findLatLng(_macDatasets, res){
  var _json = {
    "considerIp":"false",
    "wifiAccessPoints": _macDatasets
  };
  console.log(_json);
  request.post(
    {
      url:'https://www.googleapis.com/geolocation/v1/geolocate?key=' + googleapisKey,
      json:_json
    }, function(err, httpResponse, body){
      console.log(body);
      res.status(200).jsonp({status:"get post ok"});
  });

}
