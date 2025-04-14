var ThingSpeakClient = require('thingspeakclient');
var express = require('express');
var request = require('request');
var app = express();
var http = require('http').Server(app);

var client = new ThingSpeakClient();

var channel1Id = 2693997; 
var channel2Id = 2696600; 
var channel3Id = 2839726; 
var channel4Id = 2839724; 

var channelIds = [channel1Id, channel2Id];

var apiKey1 = '63PWHN5ITJJHAB6C'; 
var apiKey2 = 'MTMUXF2RAK82VRYG'; 
var apiKey3 = '4EUXJH2K5OEJJGF9'; 
var apiKey4 = 'CMQZAC935LKTH1CO'; 

var apiKeys = [apiKey1, apiKey2];

client.attachChannel(channel1Id, { writeKey: apiKey1 });
client.attachChannel(channel2Id, { writeKey: apiKey2 });



// IP addresses of the two nodes
var ip1 = 'http://[fd00::f6ce:3672:259d:c614]/';
var ip2 = 'http://[fd00::f6ce:3662:d8f7:7273]/';
var ip3 = 'http://[fd00::f6ce:3693:add5:240a]/';
var ip4 = 'http://[fd00::f6ce:3662:3598:62cc]/';

var ips = [ip1, ip2];
function fetchDataAndSendToThingSpeak(ip, channelId) {
  request.get(ip, function (err, res, body) {
    if (err) {
      console.log(`Error fetching data from ${ip}:`, err);
      return;
    }
    try {
      var obj = JSON.parse(body);
      console.log(`Data from ${ip}:`, obj);

      var temperature = obj.temp;
      var humidity = obj.hum;

      client.updateChannel(channelId, { field1: temperature, field2: humidity }, function (err, resp) {
        if (!err && resp > 0) {
          console.log(`Data sent to ThingSpeak channel ${channelId} successfully`);
        } else {
          console.log(`Error sending data to ThingSpeak channel ${channelId}:`, err);
        }
      });
    } catch (parseError) {
      console.log(`Error parsing response from ${ip}:`, parseError);
    }
  });
}

// Set up an interval to fetch data from both IPs every 16 seconds
// setInterval(function () {
//   fetchDataAndSendToThingSpeak(ip1, channel1Id, apiKey1); // Fetch from first IP and send to first channel
//   fetchDataAndSendToThingSpeak(ip2, channel2Id, apiKey2); // Fetch from second IP and send to second channel
//   fetchDataAndSendToThingSpeak(ip3, channel3Id, apiKey3); // Fetch from third IP and send to third channel
//   fetchDataAndSendToThingSpeak(ip4, channel4Id, apiKey4); // Fetch from fourth IP and send to fourth channel
// }, 16000);


setInterval(function () {
  fetchDataAndSendToThingSpeak(ip1, channel1Id, apiKey1); // First call

  setTimeout(() => {
    fetchDataAndSendToThingSpeak(ip2, channel2Id, apiKey2); // Second call after 16s
  }, 16000);

  setTimeout(() => {
    fetchDataAndSendToThingSpeak(ip3, channel3Id, apiKey3); // Third call after 32s
  }, 32000);

  setTimeout(() => {
    fetchDataAndSendToThingSpeak(ip4, channel4Id, apiKey4); // Fourth call after 48s
  }, 48000);
}, 64000); // Restart the cycle every 64 seconds
