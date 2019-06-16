<?php
$server="localhost";
$user="id8762605_gsm_user";
$pass="gsm_password";
$db="id8762605_gsm";

$link = mysqli_connect($server, $user, $pass,$db);

mysqli_select_db($link,$db) or die( 'MySQL ERROR: '. mysqli_error($db) );

if($link === false){
    die("ERROR: Could not connect. " . mysqli_connect_error());
}
$check = $_POST['key'];
if(isset($_POST['number'])){
  $number = $_POST['number'];
} else {
  $number = 10;
}
if(isset($check) && $check == "18da45dc6a5"){
  //$query = "SELECT * FROM `sms_recebidos`";
  $query = "SELECT * FROM `sms_recebidos` ORDER BY TimeStampBD DESC LIMIT ".$number;
  $result = mysqli_query($link,$query) or die("Error in Selecting " . mysqli_error($link));
  $emparray = array();
  while($row =mysqli_fetch_assoc($result))
  {
    $emparray[] = $row;
  }
  echo json_encode($emparray);
} else {
  echo "Not published key or wrong key.";
}
$check = '0';

mysqli_close($link);
?>
