<?php
// URL: http://hostname:port_num/security_box_api/list_service.php
$request_method = $_SERVER["REQUEST_METHOD"];

if ($request_method == "GET")
{
    echo "Please use POST request.";
}
else
{
    echo "This is list_service serice.";
}