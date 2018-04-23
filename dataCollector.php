<?php
// Code for saving data from get request sended by microcontroller in MySQL database

// get measurement values from request
$var1 = $_GET['Temperature'];
$var2 = $_GET['Humidity'];
$var3 = $_GET['Luminance'];
$var4 = $_GET['Light'];
$var5 = $_GET['Fan'];
$var6 = $_GET['Automation'];
// access details to database
define('DB_USER', "root"); // db user
define('DB_PASSWORD', ""); // db password (mention your db password here)
define('DB_DATABASE', "smarthome"); // database name
define('DB_SERVER', "localhost"); // db server
 
// Connecting to mysql database
$con = mysqli_connect(DB_SERVER, DB_USER, DB_PASSWORD) or die(mysqli_error());
$db = mysqli_select_db($con, DB_DATABASE) or die(mysqli_error()) or die(mysqli_error());
 
// mysql inserting a new row with data
    $result = mysqli_query($con, "INSERT INTO measurements(temperature, humidity, luminance, lightstatus, fanstatus, automationstatus) VALUES('$var1', '$var2', '$var3', '$var4', '$var5', '$var6')");
mysqli_close($con);

?>