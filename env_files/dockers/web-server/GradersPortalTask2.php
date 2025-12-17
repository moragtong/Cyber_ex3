<?php
session_start();
//$auth = $_COOKIE['storedauth'] ?? '';
$gotrightflag = getenv('END_TASK_STORED_FLAG');

if (!isset($_SESSION['loggedin']) || $_SESSION['loggedin'] !== true) {
    echo "<h2>Access Denied</h2>";
    echo "<p>Sorry, you must be logged in as a grader to view this page.</p>";
    exit;
}
?>

<!DOCTYPE html>
<html>
<head>
  <title>Exercise Question Forum - Graders Portal</title>
</head>
<body>
  <h2>Exercise Question Forum - Graders Portal</h2>
  <p>Welcome! here you can edit questions and answers about the 67607 course assigments</p>
  <p>No recent changes was made, but here is the flag you need: <?php echo($gotrightflag); ?></p>

  <ul>
    <li>Grade Appeals</li>
    <li>View Grade Statistics</li>
    <li>Students Teaching Survey</li>
  </ul>
</body>
</html>

