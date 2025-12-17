<?php
session_start();
//$auth = $_COOKIE['refauth'] ?? '';
$gotrightflag = getenv('END_TASK_REF_FLAG');

if (!isset($_SESSION['loggedin']) || $_SESSION['loggedin'] !== true) {
    echo "<h2>Access Denied</h2>";
    echo "<p>Sorry, you must be logged in to view this page.</p>";
    exit;
}
?>

<!DOCTYPE html>
<html>
<head>
  <title>Grades Portal - Grades</title>
</head>
<body>
  <h2>Student Grades Portal</h2>
  <p>Welcome! here you can see your grades for the 67607 course assigments</p>
  <p>No grades found, but here is the flag you need: <?php echo($gotrightflag); ?></p>

  <ul>
    <li>Grade Appeals</li>
    <li>View Grade Statistics</li>
    <li>Students Teaching Survey</li>
  </ul>
</body>
</html>

