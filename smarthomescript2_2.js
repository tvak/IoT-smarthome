window.onload = pageLoad; // setting function that is executed when window is loading
// function that is executed when page is loaded
function pageLoad(){
  var client = new XMLHttpRequest(); // creating new httprequest
  var url = "http://192.168.43.55/smarthome/getMeasurements.php"; // with getMeasurements url
  client.open("GET", url, true);
  client.send();
  client.onreadystatechange = function() {
    if(client.readyState == 4) {
      var response = JSON.parse(client.responseText);
      var measurements = response["measurements"][0];
      document.getElementById("automationstatus").innerHTML="<h>Automation: "+measurements["automationstatus"]+"</h>"; // checking current automation status and updating it in interface
      if(measurements["automationstatus"]=="OFF"){
        document.getElementById("switches").style.visibility="visible"; // if automation=off, manual switches are visible
      }
    }
  }
}
// function that is executed for getting measurement results from server
function getMeasurements(){
  var client = new XMLHttpRequest();
  var url = "http://192.168.43.55/smarthome/getMeasurements.php"; // php that fetches data in MySQL and return it in JSON format
  client.open("GET", url, true);
  client.send();
  client.onreadystatechange = function() {
    if(client.readyState == 4) {
      var response = JSON.parse(client.responseText);
      var measurements = response["measurements"][0];
      // fetching all data and inserting it in html
      document.getElementById("temperature").innerHTML="Temperature: "+measurements["temperature"];
      document.getElementById("humidity").innerHTML="Humidity: "+measurements["humidity"];
      document.getElementById("luminance").innerHTML="Luminance: "+measurements["luminance"];
      document.getElementById("timestamp").innerHTML="Last measurement: "+measurements["timestamp"];
      if(document.getElementById("automationstatus").innerHTML.indexOf("ON")!== -1){
        document.getElementById("lightstatus").innerHTML="<h>Lights: "+measurements["lightstatus"]+"</h>";
        document.getElementById("fanstatus").innerHTML="<h>Fan: "+measurements["fanstatus"]+"</h>";
      }
    }
  }
}
var id = setInterval('getMeasurements();', 5000); // measurement data is updated in html every 5seconds(eventhough measurements are done just in every 30s)
// function for sending http request to microcontroller for turning lights on
function lightOn(){
  var client = new XMLHttpRequest();
  var url = "http://192.168.43.78/LED=ON";
  client.open("GET", url, true);
  client.send();
  client.onreadystatechange = function() {
    if(client.readyState == 4) {
      document.getElementById("lightstatus").innerHTML="<h>Lights: ON</h>";
    }
  }
}
// function for sending http request to microcontroller for turning lights off
function lightOff(){
  var client = new XMLHttpRequest();
  var url = "http://192.168.43.78/LED=OFF";
  client.open("GET", url, true);
  client.send();
  client.onreadystatechange = function() {
    if(client.readyState == 4) {
      document.getElementById("lightstatus").innerHTML="<h>Lights: OFF</h>";
    }
  }
}
// function for sending http request to microcontroller for turning fan on
function fanOn(){
  var client = new XMLHttpRequest();
  var url = "http://192.168.43.78/FAN=ON";
  client.open("GET", url, true);
  client.send();
  client.onreadystatechange = function() {
    if(client.readyState == 4) {
      document.getElementById("fanstatus").innerHTML="<h>Fan: ON</h>";
    }
  }
}
// function for sending http request to microcontroller for turning fan off
function fanOff(){
  var client = new XMLHttpRequest();
  var url = "http://192.168.43.78/FAN=OFF";
  client.open("GET", url, true);
  client.send();
  client.onreadystatechange = function() {
    if(client.readyState == 4) {
      document.getElementById("fanstatus").innerHTML="<h>Fan: OFF</h>";
    }
  }
}
// function for sending http request to microcontroller for turning automation on
function autoOn(){
  var client = new XMLHttpRequest();
  var url = "http://192.168.43.78/AUTOMATION=ON";
  client.open("GET", url, true);
  client.send();
  client.onreadystatechange = function() {
    if(client.readyState == 4) {
      document.getElementById("automationstatus").innerHTML="<h>Automation: ON</h>";
      document.getElementById("switches").style.visibility="hidden"; // if automation=on, hiding manual switches
    }
  }
}
// function for sending http request to microcontroller for turning automation off
function autoOff(){
  var client = new XMLHttpRequest();
  var url = "http://192.168.43.78/AUTOMATION=OFF";
  client.open("GET", url, true);
  client.send();
  client.onreadystatechange = function() {
    if(client.readyState == 4) {
      document.getElementById("automationstatus").innerHTML="<h>Automation: OFF</h>";
      document.getElementById("switches").style.visibility="visible"; // if automation=off, manual switches are visible
    }
  }
}
function stop() { // call this to stop your interval.
   clearInterval(id);
}
