<?php


$input = file_get_contents('php://input');

if (isset($_POST["colorR"]))
{
	echo $_POST["colorR"];
	file_put_contents("rgb.txt", $input);
} else {
	file_put_contents("rgb.txt", $input);
	echo $input;
}


?>
