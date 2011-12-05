<?php
require_once( 'pachube_functions.php' );


# *****************************************************************
#
# Create a Pachube object and pass your API key
#
# *****************************************************************


$api_key = "mFgy2cgsyv4GKhw4G4EzNcpgpfhQJkg4JvCrNpOskwo";



$pachube = new Pachube($api_key);


// note that you must own the feed listed below in order to update it!
$url = "http://www.pachube.com/api/25375.csv";
$data = $_GET["value"];
$myFile = "phptest.txt";
$fh = fopen($myFile, 'w') or die("0");
fwrite($fh, $data);
fclose($fh);
ehco "1";


// this next line makes the actual update attempt and returns a status code

$update_status = $pachube->updatePachube ( $url, $data );	
$pachube->debugStatusCode($update_status);

?>