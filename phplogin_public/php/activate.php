<?php
// Change this to your connection info.
require_once "pdo.php";



if (isset($_GET['email'], $_GET['code'])) {	
	$sql = "SELECT * FROM accounts WHERE email = :em AND activation_code = :ac";
	//echo "<pre>\n$sql\n</pre>\n";
	$stmt = $pdo->prepare($sql);
	$stmt->execute(array(
		':em' => $_GET['email'],
		':ac' => $_GET['code']));
	$row = $stmt->fetch(PDO::FETCH_ASSOC);

	if ($row !== false) {
		$sql = "UPDATE accounts SET activation_code = :activated WHERE email = :em AND activation_code = :ac";
		//echo "<pre>\n$sql\n</pre>\n";
		$newcode = 'activated';
		$stmt = $pdo->prepare($sql);
		$stmt->execute(array(
			':activated' => $newcode,
			':em' => $_GET['email'],
			':ac' => $_GET['code']));
		echo 'Your account is now activated! You can now <a href="../index.html">login</a>!';
		
	} else {
		echo 'The account is already activated or doesn\'t exist!';
	}

	$pdo->connection = null;
}
?>

