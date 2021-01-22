<?php
session_start();
require_once "pdo.php";

// Now we check if the data from the login form was submitted, isset() will check if the data exists.
if ( !isset($_POST['username'], $_POST['password']) ) {
	// Could not get the data that should have been sent.
	exit('Please fill both the username and password fields!');
} else {
    echo("Handling POST data...\n");
    $sql = "SELECT id, password FROM accounts WHERE username = :un";
    echo "<pre>\n$sql\n</pre>\n";
    $stmt = $pdo->prepare($sql);
    $stmt->execute(array(
        ':un' => htmlentities($_POST['username'])));
    $row = $stmt->fetch(PDO::FETCH_ASSOC);

    //Let's verify that such account exists:
    if ($row !== false) {
        if (password_verify($_POST['password'], $row['password'])) {
            session_regenerate_id();
            $_SESSION['loggedin'] = TRUE;
            $_SESSION['name'] = htmlentities($_POST['username']);
            $_SESSION['id'] = $row['id'];
            header('Location: home.php');
        } else {
            // Incorrect password
            echo 'Incorrect username and/or password!';
        }
    } else {
        // Incorrect username
        echo 'Incorrect username and/or password!';
    }

    $pdo->connection = null;

}
?>

