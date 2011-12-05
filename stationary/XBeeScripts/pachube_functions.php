<?php

# Pachube functions
#
#**********************************************************************
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# ( at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# ERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
# Online: http://www.gnu.org/licenses/gpl.txt

# *****************************************************************


// This class is used for manipulating the API of www.pachube.com, a service 
// that enables you to connect, tag and share real time sensor data from 
// objects, devices, buildings and environments around the world. 
//
// Currently only supports updating a manual feed. More API functionality to 
// be added over time.
// Requires PHP 5

// Version 0.2 
// Updated: May 4, 2010


class Pachube
{
		
	private $Api;
	private $Pachube_headers;
	private $Pachube;
	
	function __construct ($api) 
	{
		$this->Api = $api;
		$this->Pachube = "www.pachube.com";
		$this->Pachube_headers  = array("X-PachubeApiKey: $this->Api");
	}
	
	
	
	
	 		
/********************* Methods *****************************/

// Update a Pachube manual feed

	public function updatePachube ( $url='', $data='' )
	{ 
		if(empty($this->Api))
		{
			return 401;
		} else 
		{
			if(!empty($url))
			{
				$request = $this->putRequestToPachube($url, $data);
				return $request;
				
			}else
			{
				return 998;
			}
		}
	}
		
	public function updatePachubeDatastream ( $feed='', $datastream='', $value='' )
	{ 
		if(empty($this->Api))
		{
			return 401;
		} else 
		{
			if(is_numeric($feed) && is_numeric($datastream) && is_numeric($value))
			{
				$url = "http://$this->Pachube/api/feeds/$feed/datastreams/$datastream.csv";
				$request = $this->putRequestToPachube($url, $value);
				return $request;
				
			}else
			{
				return 998;
			}
		}
	}
		
// Retrieve a Pachube feed

	public function retrieveData ( $url='', $type='' )
	{ 
		if(empty($this->Api))
		{
			//trigger_error(999);
			return "No API present";
		} else 
		{
			if(!empty($url))
			{
			
				if (!empty($type)) {
					if ((strcmp(strtolower($type), "xml") == 0) || (strcmp(strtolower($type), "json") == 0) || (strcmp(strtolower($type), "csv") == 0)){
						$url = "http://$this->Pachube/api/feeds/$url.$type";
					} else {
						$url = "";
					}
				}
				$request = $this->getRequestToPachube($url);
				return $request;
				
			}
		}
	}
		
	

// Retrieve history

	public function retrieveHistory ( $url='' )
	{ 
			if(!empty($url))
			{
				if(function_exists(curl_init))
				{	
					$ch = curl_init();	
					curl_setopt($ch, CURLOPT_URL, $url);
					curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);

					$data = curl_exec($ch);
		
					curl_close($ch);
	
					$data_array = explode(",",$data);
					return $data_array;
				} 
			}
			else return "Invalid history URL";		
	}
		


// Create feed

	public function createFeed ( $title='' )
	{ 
		if(empty($this->Api))
		{
			return -401;
		} else 
		{
			if(!empty($title))
			{			
				$eeml = "<eeml xmlns=\"http://www.eeml.org/xsd/005\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xsi:schemaLocation=\"http://www.eeml.org/xsd/005 http://www.eeml.org/xsd/005/005.xsd\"><environment><title>$title</title></environment></eeml>";
				
				$request = $this->createPachube( $eeml );
				return $request;
				
			}else
			{
				return -1;
			}
		}
	}
		

// delete a Pachube feed

	public function deletePachube ( $feed_id='' )
	{ 
		if(empty($this->Api))
		{
			return 401;
		} else 
		{
			if(is_numeric($feed_id) )
			{
				$url = "http://$this->Pachube/api/feeds/".$feed_id;
				$request = $this->deleteRequestToPachube($url);
				return $request;
				
			}else
			{
				return 998;
			}
		}
	}
		


// debug status codes returned by updatePachube()

	
	public function debugStatusCode ( $status_code ){
		
		switch ($status_code){			
			case 200:
				$msg = "Pachube feed successfully updated";	
				break;
		
			case 401:
				$msg = "Pachube API key was incorrect";
				break;
		
			case 404:
				$msg = "Feed ID does not exist";
				break;
				
			case 422:
				$msg = "Unprocessable Entity, semantic errors (CSV instead of XML?)";
				break;
				
			case 997:
				$msg = "Could not create resource: no title provided.";
				break;
		
			case 998:
				$msg = "Error in feed ID, data type or data";
				break;
		
			case 999:
				$msg = "curl library not installed";
				break;	
			
			default:
				$msg = "Status code not recognised: ".$status_code;
				break;
		}		
		echo $msg;		
	}



	public function showGraph ( $feed_id='', $datastream_id, $width='300', $height='200', $colour='FF0066', $label=true, $grid=true, $title='', $legend='', $stroke='4' ){

		if(is_numeric($feed_id)){
		
			if(is_numeric($feed_id) && is_numeric($datastream_id)){
							
				$legend_param = empty($legend)? "" : "&l=$legend";
				$grid_param = $grid? "&g=$grid" : "";
				$label_param = $label? "&b=$label" : "";
				$title_param = (strcmp($title,"")==0)? "" : "&t=$title";
							
				echo "<img src=\"http://$this->Pachube/feeds/$feed_id/datastreams/$datastream_id/history.png?w=$width&h=$height&c=$colour$label_param$grid_param$title_param$legend_param&s=$stroke\" width=\"$width\" height=\"$height\" border=\"1\" alt=\"powered by Pachube.com\">";
			
			}		
		}
		
	}



	public function getLatLon ( $search='') 
	{
		if (!empty($search))
		{
			if(function_exists(curl_init))
			{	
			
				$search = urlencode($search);
				$url = "http://$this->Pachube/api/feeds.json?order=created_at&q=$search";

				$ch = curl_init();	
				curl_setopt($ch, CURLOPT_URL, $url);
				curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
				curl_setopt($ch, CURLOPT_HTTPHEADER, $this->Pachube_headers);
						
				$data = curl_exec($ch);
	
				$headers = curl_getinfo($ch);
				curl_close($ch);
				
				$json_data = json_decode($data,true);
				
				//var_dump ($json_data);
				
				$reported_locations = Array();
				
				foreach ( $json_data["results"] as $result ) 
				{				
					//print_r ($result);					
					$location = $result["location"];
					
					$this_location = Array();
					
					$this_location["lat"] = $location["lat"];
					$this_location["lon"] = $location["lon"];
					
					array_push($reported_locations, $this_location);
					
				}
				
				return $reported_locations;
			} 							
		}
	}

        private function _fputc($url,$data)
	{
		$_err = __CLASS__.'::'.__FUNCTION__."($url): ";		
		// Create a stream
		$opts['http']['method'] = "PUT";
		$opts['http']['header'] = "X-PachubeApiKey: ".$this->Api."\r\n";
		$opts['http']['header'] .= "Content-Length: " . strlen($data) . "\r\n";
		$opts['http']['content'] = $data;	
        
		$context = stream_context_create($opts);

		// Open the file using the HTTP headers set above
		$ret=file_get_contents($url, false, $context);
                if($ret==false) die('<br>'.$_err.' failed file_get_contents('.$url.')');
		return $ret;
	}

	private function putRequestToPachube ( $url, $data)
	{	
		if(function_exists('curl_init'))
		{

			$putData = tmpfile();
			fwrite($putData, $data);
			fseek($putData, 0);

			$ch = curl_init();
			curl_setopt($ch, CURLOPT_URL, $url);
			curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
			curl_setopt($ch, CURLOPT_HTTPHEADER, $this->Pachube_headers);
			curl_setopt($ch, CURLOPT_INFILE, $putData);
			curl_setopt($ch, CURLOPT_INFILESIZE, strlen($data));
			curl_setopt($ch, CURLOPT_PUT, true);

			curl_exec($ch);

			$headers = curl_getinfo($ch);
			fclose($putData);
			curl_close($ch);

			return $headers['http_code'];

		}
		
		if(function_exists('file_put_contents') && ini_get('allow_url_fopen'))
		{
			return $this->_fputc($url,$data);
		}

		return 999;
	}

        private function _fgetc($url)
	{
		// Create a stream
		$opts['http']['method'] = "GET";
		$opts['http']['header'] = "X-PachubeApiKey: ".$this->Api."\r\n";
		$context = stream_context_create($opts);

		// Open the file using the HTTP headers set above
		return file_get_contents($url, false, $context);
	}
	
	private function _curl($url)
	{
		$ch = curl_init();
		curl_setopt($ch, CURLOPT_URL, $url);
		curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
		curl_setopt($ch, CURLOPT_HTTPHEADER, $this->Pachube_headers);

		$ret = curl_exec($ch);
		$headers = curl_getinfo($ch);
		curl_close($ch);

		return $ret;
	}
	
	// Actual request that retrieved Pachube data
	private function getRequestToPachube ( $url )
	{		
		if(function_exists('curl_init'))
		{
			return $this->_curl($url);
		}
		
		if(function_exists('file_get_contents') && ini_get('allow_url_fopen'))
		{
			return $this->_fgetc($url);		
		}
		
		return 999;
	}




// Actual request that creates the Pachube feed

	private function createPachube ( $eeml='' )
	{
		$ret = -1;
		{
			if(function_exists(curl_init))
			{	
				$url = "http://$this->Pachube/api.xml";				
				
				$ch = curl_init();	
				curl_setopt($ch, CURLOPT_URL, $url);
				curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
				curl_setopt($ch, CURLOPT_HTTPHEADER, $this->Pachube_headers);
				curl_setopt($ch, CURLOPT_FOLLOWLOCATION, false);
				curl_setopt($ch, CURLOPT_POST, true);
				curl_setopt($ch, CURLOPT_HEADER, true);
				curl_setopt($ch, CURLOPT_POSTFIELDS, $eeml);
						
				$return = curl_exec($ch);
	
				$headers = curl_getinfo($ch);

				curl_close($ch);
				
				$status = $headers['http_code'];				
				
				if ($status != 201) {
					$ret = -$status;
				} else {
					$ret = trim($this->stringBetween($return,"Location: http://$this->Pachube/api/feeds/","\n"));			
				}																
			} 
			else
			{
				$ret = -999;
			}				
		}
		
	return $ret;
   } 


// Actual request that makes the Pachube delete

	private function deleteRequestToPachube ( $url='')
	{
		$ret = -1;
		{
			if(function_exists(curl_init))
			{	
				$ch = curl_init();	
				curl_setopt($ch, CURLOPT_URL, $url);
				curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
				curl_setopt($ch, CURLOPT_HTTPHEADER, $this->Pachube_headers);
				curl_setopt($ch, CURLOPT_CUSTOMREQUEST, "DELETE");
						
				curl_exec($ch);
	
				$headers = curl_getinfo($ch);

				curl_close($ch);
												
				$ret = $headers['http_code'];
								
			} 
			else
			{
				$ret = 999;
			}				
		}
		
	return $ret;
   } 



	private function stringBetween($content,$start,$end)
	
	{
	
		$r = explode($start, $content);
		if (isset($r[1])){
			$r = explode($end, $r[1]);
			return $r[0];
		}
		return '';
	
	}
	
	public function environment($feed_id)
	
	{	
		$data = json_decode($this->retrieveData ( $feed_id, "json" ), true);		
		//print_r ($data);
		$return_data = (strcmp($data,"Unable to find specified resource.")==0) ? json_decode("{\"location\":{}\"datastreams\":[{}]}") : $data;
		return $return_data;
	}
	
	public function showEnvironmentGraph($environment, $datastream_id, $width='300', $height='200', $colour='FF0066', $label=true, $grid=true, $title='', $legend='', $stroke='4' )
	
	{	
		$feed_id = $environment['id'];
		$this->showGraph($feed_id, $datastream_id, $width, $height, $colour, $label, $grid, $title, $legend, $stroke );
	}


	public function showEnvironmentMap( $environment, $width, $height, $google_map_key)
	
	{	
	
	if (strcmp($environment, "") !=0){
	
		$width.="px";
		$height.="px";
		
		$lat = $environment['location']['lat'];
		$lon = $environment['location']['lon'];
		$gmap_text = <<<END
<div id="pachube_map" style="width: $width; height: $height"></div><script src="http://maps.google.com/maps?file=api&amp;v=2&amp;key=$google_map_key" type="text/javascript"></script><script type="text/javascript"> function initialize() { if (GBrowserIsCompatible()) { var map = new GMap2(document.getElementById("pachube_map")); map.setCenter(new GLatLng($lat, $lon), 13); map.setUIToDefault(); var point = new GLatLng($lat, $lon); map.addOverlay(new GMarker(point)); }}initialize();</script>
END;

		echo $gmap_text;
	
		}
	
	}


}?>