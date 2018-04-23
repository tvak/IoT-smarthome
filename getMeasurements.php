<?php
// Code for fetching newest measurement data from MySQL database and returning it to the html page in JSON format

// access details to database
define('DB_USER', "root"); // db user
define('DB_PASSWORD', ""); // db password (mention your db password here)
define('DB_DATABASE', "smarthome"); // database name
define('DB_SERVER', "localhost"); // db server
 
// Connecting to mysql database
$con = mysqli_connect(DB_SERVER, DB_USER, DB_PASSWORD) or die(mysqli_error());
$db = mysqli_select_db($con, DB_DATABASE) or die(mysqli_error()) or die(mysqli_error());

// array for JSON response
$response = array();
 
// get newest measurement from measurements table
$result = mysqli_query($con, "SELECT *FROM measurements ORDER BY timestamp DESC LIMIT 1") or die(mysqli_error($con));
 
// check for empty result
if (mysqli_num_rows($result) > 0) {
    // looping through all results
    // data node
    $response["measurements"] = array();
 
    while ($row = mysqli_fetch_array($result)) {
        // temp data array
        $data = array();
		// add needed data to data array
        $data["id"] = $row["id"];
        $data["temperature"] = $row["temperature"];
        $data["humidity"] = $row["humidity"];
        $data["luminance"] = $row["luminance"];
		$data["lightstatus"] = $row["lightstatus"];
		$data["fanstatus"] = $row["fanstatus"];
		$data["automationstatus"] = $row["automationstatus"];
        $data["timestamp"] = $row["timestamp"];
 
        // push single data array into final response array
        array_push($response["measurements"], $data);
    }
    // add success tag
    $response["success"] = 1;
 
    // echoing JSON response
    echo json_encode($response);
} else {
    // no data found
    $response["success"] = 0;
    $response["message"] = "No data found";
 
    // echo no data JSON
    echo json_encode($response);
}
mysqli_close($con);
?>