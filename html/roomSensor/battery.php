<?php
	$batteryPath="battery.txt";
	$file=fopen($batteryPath,"r");
	$content=fread($file,filesize($batteryPath));

	$write1="{\n\t\"air\": ";
	$write2="\n}";

	if($content[0]=='1') echo $write1."5",$write2;
	else echo $write1."1",$write2;
		
	/*
	$battery=(float)$content;

	$write1="{\n\t\"air\": ";
	$write2=",\n\t\"Voltage\": ".$battery."\n}";
	
	if($battery>=4.1) echo $write1."0".$write2;
	else if($battery>=3.9) echo $write1."1".$write2;
	else if($battery>=3.75) echo $write1."2".$write2;
	else if($battery>=3.65) echo $write1."3".$write2;
	else echo $write1."4".$write2;
	*/


	fclose($file);
?>