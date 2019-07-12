<!doctype html>
<html>
<head>
<meta charset="UTF-8">
<title>Token</title>
</head>
<body>
	<?php 
	if($_GET['id']==NULL && $_GET['notification']==NULL)
	{
		echo "error";
	}
	else if($_GET['username']==NULL && $_GET['password']==NULL)
	{
		$file = fopen("Tokens/".$_GET['id'], "w");
		fwrite($file, $_GET['token']);
		fclose($file);
	}
	else
	{
		$file = fopen("Tokens/".$_GET['id'],"r");
		$token = fread($file, filesize("Tokens/".$_GET['id']));
		fclose($file);
		$file = fopen("Tokens/".$_GET['id'],"w");
		fwrite($file,$token.",".$_GET['username'].",".$_GET['password']);
		fclose($file);
	}

	if($_GET['notification']=="1")
	{
		$file = fopen("Tokens/sendNotificationFlag","w");
		fwrite($file, "1");
		fclose($file);
	}


	?>
</body>
</html>