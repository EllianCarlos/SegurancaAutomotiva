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

$timeStampSend =date("Y-m-d H:i:s",$_POST['timeStampSend']);
$tempoAtivo = $_POST['tempoAtivo'];
$tempoUmaMaoVolante = $_POST['tempoUmaMaoVolante'];
$tempoDuasMaoVolante = $_POST['tempoDuasMaoVolante'];
$emerg = $_POST['emerg'];
$alerta= $_POST['alerta'];
$airbag = $_POST['airbag'];
$velocidade = $_POST['Velocidade'];
$tempMotor = $_POST['tempMotor'];

if ((isset($_POST['AirBag']) == "")&(isset($_POST['emerg']) == "")&(isset($_POST['Velocidade']) == "")&(isset($_POST['tempoUmaMao']) == "")){
  printf("Error getting Message (POST Method Wrong)");
  //$query = "INSERT INTO `id8762605_gsm`.`sms_recebidos` (`timeStamp`,`MaoVolante`,`Velocidade`,`AirBag`,`Botao`) VALUES (current_timestamp,2,3,1,1)";
  //mysqli_query($link,$query,MYSQLI_USE_RESULT);

}
else {
  // echo 'Tempo de Leitura: '.$timeStampSend;
  // echo '\n<br />';
  // echo 'Tempo Ativo: '.$tempoAtivo;
  // echo '\n<br />';
  // echo 'Velocidade: '.$velocidade;
  // echo '\n<br />';
  // echo 'AirBag: '.$airbag;
  // echo '\n<br />';
  // echo 'Emerg: '.$emerg;
  // echo '\n<br />';
  if(!isset($_POST['timeStampSend'])){
      $query = "INSERT INTO `sms_recebidos` (`tempoAtivo`,`tempoUmaMaoVolante`,`tempoDuasMaoVolante`,`emerg`,`alerta`,`airbag`,`Velocidade`,`tempMotor`,`id`)
       VALUES ('".$tempoAtivo."','".$tempoUmaMaoVolante."','".$tempoDuasMaoVolante."','".$emerg."','".$alerta."','".$airbag."','".$velocidade."','".$tempMotor."',NULL)";
  } else {
     $query = "INSERT INTO `sms_recebidos` (`timeStampSend`,`tempoAtivo`,`tempoUmaMaoVolante`,`tempoDuasMaoVolante`,`emerg`,`alerta`,`airbag`,`Velocidade`,`tempMotor`,`id`)
      VALUES ('".$timeStampSend."','".$tempoAtivo."','".$tempoUmaMaoVolante."','".$tempoDuasMaoVolante."','".$emerg."','".$alerta."','".$airbag."','".$velocidade."','".$tempMotor."',NULL)";
  }
  //$query = "INSERT INTO `id8762605_gsm`.`sms_recebidos` (`timeStamp`,`MaoVolante`,`Velocidade`,`AirBag`,`Botao`,`id`) VALUES (current_timestamp, 1,2,3,1,1)";
  $result = mysqli_query($link,$query);
  echo "success";
  // echo 'Query: '.$query;
  // echo '\n<br />';
  // echo 'Result: '.$result;
}

mysqli_close($link);
//header("Location: index.php");// Aviso Aqui  <b>Warning</b>:  Cannot modify header information - headers already sent by (output started at /storage/ssd2/605/8762605/public_html/add.php:16) in <b>/storage/ssd2/605/8762605/public_html/add.php</b> on line <b>32</b><br />
?>
