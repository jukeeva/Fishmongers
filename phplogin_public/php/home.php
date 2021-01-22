<?php
require_once "pdotraps.php";
// We need to use sessions, so you should always start sessions using the below code.
session_start();
// If the user is not logged in redirect to the login page...
if (!isset($_SESSION['loggedin'])) {
	header('Location: index.html');
	exit;
}
		
$stmt = $pdo->query("SELECT * FROM vbnhzm");
$rows = $stmt->fetchAll(PDO::FETCH_ASSOC);
$row_count = $stmt->rowCount();
$amount = 0;
$amountlarge = 0;
$amountsmall = 0;
$chosendivider = 45;
$totallength = 0;
$averagelength = 0; 

foreach ( $rows as $row ) {
	$amount++;
	$totallength += $row['length'];
	if ($row['length'] >= $chosendivider){
		$amountlarge++;
	} else {
		$amountsmall++;
	}
}
$averagelength = $totallength / $amount;

?>

<!DOCTYPE html>
<html>
	<head>
		<meta charset="utf-8">
		<title>Home Page</title>
        <link href=".././css/style.css" rel="stylesheet" type="text/css">
		<link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.7.1/css/all.css">
		<script src="https://cdnjs.cloudflare.com/ajax/libs/moment.js/2.13.0/moment.min.js"></script>
    	<script src="https://cdn.jsdelivr.net/npm/chart.js@2.9.4/dist/Chart.min.js"></script>
    	<script src="../scripts/weather2.js"></script>
	</head>
	<body class="loggedin">
		<nav class="navtop">
			<div>
				<h1>Kalansat.fi</h1>
				<a href="profile.php"><i class="fas fa-user-circle"></i>Profile</a>
				<a href="logout.php"><i class="fas fa-sign-out-alt"></i>Logout</a>
			</div>
		</nav>
		<div class="content">
			<h2>Home Page</h2>
			<p>Welcome back, <?=$_SESSION['name']?>!</p>
		</div>
		<section class="page-content">
			<section class="grid">
				<article><h3><b>Here are some words</b></h3></article>
				<article>					
					<div id="day-view">
						<div id="location-text-view"><b>Location: Helsinki</b></div>
						<div id="temp-container">Error</div>					
						<div id="icons-view">
							<div id="rain-container"><img id="rain-symbol" src="../images/rain-wind.svg" alt="rain icon"></div>
							<div id="wind-icons-view">
								<div id="wind-container"><img id="wind-symbol8" src="../images/wind-8.svg" alt="wind8 icon"></div>
								<div id="wind-container"><img id="wind-symbol3" src="../images/wind-3.svg" alt="wind3 icon"></div>
							</div>                
						</div>
					
					</div>
					<div id="chart-view">
						<canvas id="myChart" width="600px" height="250px"></canvas>
					</div>	
				</article>
				<article>
					<div id="trap-view">
						<div id="trap-info-view"><b>Trap ID: vbnhzm</b></div>
						<p><br></p>
						<div id="trap-details-view"><b></b>
							<div id="text-group-view">
								<div id="total-amount-view"><b>Total amount: <?php echo $row_count?> </b>
								<div id="average-length-view"><b>Average length: <?php echo $averagelength?> cm</b>
							</div>
							<div id="text-group-view">
								<div id="amount-large-view"><b>Amount large: <?php echo $amountlarge?></b>
								<div id="amount-small-view"><b>Amount small: <?php echo $amountsmall?></b>
							</div>
						</div>
					</div>
				</article>
				<article></article>
				<article></article>
				<article></article>
				<article></article>
				<article></article>
			</section>
			</section>
	</body>
</html>