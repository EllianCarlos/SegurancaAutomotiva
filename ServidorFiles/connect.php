<?php
 function Connection(){
  $server="localhost";
  $user="id8762605_gsm_user";
  $pass="gsm_password";
  $db="id8762605_gsm";

  $connection = mysqli_connect($server, $user, $pass);

  if (!$connection) {
      die('MySQL ERROR: ' . mysqli_connect_error($connection));
  }

  mysqli_select_db($connection,$db) or die( 'MySQL ERROR: '. mysqli_error($db) );
  return $connection;
}
?>
