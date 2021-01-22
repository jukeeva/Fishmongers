<?php
// Import PHPMailer classes into the global namespace
// These must be at the top of your script, not inside a function
use PHPMailer\PHPMailer\PHPMailer;
use PHPMailer\PHPMailer\Exception;
use PHPMailer\PHPMailer\SMTP;
require '../phpmailer/PHPMailer.php';
require '../phpmailer/SMTP.php';
require '../phpmailer/Exception.php';
require_once "pdo.php";

// Instantiation and passing `true` enables exceptions
$mail = new PHPMailer(true);

// Now we check if the data was submitted, isset() function will check if the data exists.
if (!isset($_POST['username'], $_POST['password'], $_POST['email'])) {
	// Could not get the data that should have been sent.
	exit('Please complete the registration form!');
}
// Make sure the submitted registration values are not empty.
if (empty($_POST['username']) || empty($_POST['password']) || empty($_POST['email'])) {
	// One or more values are empty.
	exit('Please complete the registration form');
} else {
    $sql = "SELECT id, password FROM accounts WHERE username = :un";
    //echo "<pre>\n$sql\n</pre>\n";
    $stmt = $pdo->prepare($sql);
    $stmt->execute(array(
        ':un' => htmlentities($_POST['username'])));
    $row = $stmt->fetch(PDO::FETCH_ASSOC);
    
    if (isset($row['id'])) {
		// Username already exists
		echo 'Username exists, please choose another!';
    } else {
        // Username doesnt exists, insert new account
        if (!filter_var($_POST['email'], FILTER_VALIDATE_EMAIL)) {
            exit('Email is not valid!');
        }
        if (preg_match('/^[a-zA-Z0-9]+$/', $_POST['username']) == 0) {
            exit('Username is not valid!');
        }
        if (strlen($_POST['password']) > 20 || strlen($_POST['password']) < 5) {
            exit('Password must be between 5 and 20 characters long!');
        } else {
            $sql = "INSERT INTO accounts (username, password, email, activation_code) VALUES (:name, :password, :email, :code)";
            $password = password_hash(htmlentities($_POST['password']), PASSWORD_DEFAULT);
            $uniqid = uniqid();

            //echo "<pre>\n$sql\n</pre>\n";
            $stmt = $pdo->prepare($sql);
            $stmt->execute(array(
            ':name' => htmlentities($_POST['username']), //html entities used to prevent php injection.
            ':password' => $password,
            ':email' => htmlentities($_POST['email']),            
            ':code' => $uniqid));

            $activate_link = '127.0.0.1/phplogin/php/activate.php?email=' . htmlentities($_POST['email']) . '&code=' . $uniqid;

            try {
                //Server settings
                $mail->SMTPDebug = SMTP::DEBUG_OFF;                      // Enable verbose debug output
                $mail->isSMTP();                                            // Send using SMTP
                $mail->Host       = "smtp.gmail.com";                       // Set the SMTP server to send through
                $mail->SMTPAuth   = true;                                   // Enable SMTP authentication
                $mail->Username   = 'email@gmail.com';                     // SMTP username
                $mail->Password   = 'password';                               // SMTP password
                $mail->SMTPSecure = PHPMailer::ENCRYPTION_STARTTLS;         // Enable TLS encryption; `PHPMailer::ENCRYPTION_SMTPS` encouraged
                $mail->Port       = 587;                                    // TCP port to connect to, use 465 for `PHPMailer::ENCRYPTION_SMTPS` above
            
                //Recipients
                $mail->setFrom('from@example.com', 'Kalansat.com');
                $mail->addAddress(htmlentities($_POST['email']));               // Name is optional
                
                $mail->isHTML(true);                                  // Set email format to HTML
                $mail->Subject = 'Account Activation Required';
                $mail->Body    = '<p>Please click the following link to activate your account: <a href="' . $activate_link . '">' . $activate_link . '</a></p>';
                $mail->AltBody = 'Thanks for joining us! Below you can find you account activation code which you can use to confirm your account. ABC123';
            
                $mail->send();
                echo 'Please check your email to activate your account!';
            } catch (Exception $e) {
                error_log("Message could not be sent. Mailer Error: {$mail->ErrorInfo}");
            }

        }

    }

}

$pdo->connection = null;

?>
