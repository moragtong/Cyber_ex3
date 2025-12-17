<?php
session_start();
//$auth = $_COOKIE['dombasedauth'] ?? '';
$gotrightflag = getenv('END_TASK_DOM_FLAG');

if (!isset($_SESSION['loggedin']) || $_SESSION['loggedin'] !== true) {
    echo "<h2>Access Denied</h2>";
    echo "<p>Sorry, you must be logged in as a grader to view this page.</p>";
    exit;
}
?>

<!DOCTYPE html>
<html>
<head>
  <title>Course Management Portal - Enrolled Students</title>
</head>
<body>
  <h2>Course Management Portal - Enrolled Students</h2>
  <p>Welcome! here you can add users to the 67607 course</p>
  <p>No recent users was aded to the course, but here is the flag you need: <?php echo($gotrightflag); ?></p>

  <ul>
    <li>Grade Appeals</li>
    <li>View Grade Statistics</li>
    <li>Students Teaching Survey</li>
  </ul>
</body>
</html>

