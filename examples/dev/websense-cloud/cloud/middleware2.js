var express = require('express');
var request = require('request');
var fs = require('fs');
var path = require('path');
var app = express();
var http = require('http').Server(app);

// IP addresses of the nodes
var ip1 = 'http://[fd00::f6ce:3672:259d:c614]/';
var ip2 = 'http://[fd00::f6ce:3662:d8f7:7273]/';
var ip3 = 'http://[fd00::f6ce:3693:add5:240a]/';
var ip4 = 'http://[fd00::f6ce:3662:3598:62cc]/';

// Helper function to format the date
function formatDate(date) {
  const isoString = date.toISOString();
  return isoString.replace(/\.\d{3}Z$/, '+00:00'); // Remove milliseconds and add timezone offset
}

function fetchDataAndPutInCSV(ip) {
  request.get(ip, function (err, res, body) {
    if (err) {
      console.log(`Error fetching data from ${ip}:`, err);
      return;
    }
    try {
      var obj = JSON.parse(body);
      console.log(`Data from ${ip}:`, obj);

      // Extract necessary fields
      var createdAt = obj.created_at || formatDate(new Date());
      var entryId = obj.entry_id || '';
      var field1 = obj.field1 || obj.temp || '';
      var field2 = obj.field2 || obj.hum || '';
      var latitude = obj.latitude || '';
      var longitude = obj.longitude || '';
      var elevation = obj.elevation || '';
      var status = obj.status || '';

      // Create CSV row
      var csvRow = `${createdAt},${entryId},${field1},${field2},${latitude},${longitude},${elevation},${status}\n`;

      // Generate a unique filename for each IP
      var fileName = `data_${ip.replace(/[:\/\[\]]/g, '_')}.csv`;
      var filePath = path.join(__dirname, fileName);

      // Check if the file exists; if not, create it with headers
      if (!fs.existsSync(filePath)) {
        var headers = 'created_at,entry_id,field1,field2,latitude,longitude,elevation,status\n';
        fs.writeFileSync(filePath, headers);
      }

      // Append the new row to the CSV file
      fs.appendFileSync(filePath, csvRow);

    } catch (parseError) {
      console.log(`Error parsing response from ${ip}:`, parseError);
    }
  });
}

// Fetch data from all IPs at regular intervals
setInterval(function () {
  fetchDataAndPutInCSV(ip1);
  fetchDataAndPutInCSV(ip2);
  fetchDataAndPutInCSV(ip3);
  fetchDataAndPutInCSV(ip4);
}, 16000); // Restart the cycle every 16 seconds
