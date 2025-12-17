<?php
session_start();
$flag = getenv('STORED_FLAG');
//setcookie('storedauth', $flag, time() + 3600); // simulate login of the victim.

// Database connection using environment variables
$host = getenv('DB_HOST');
$username = getenv('DB_UNAME'); 
$password = getenv('DB_PASSWORD'); 
$database = getenv('DB_NAME'); 

$conn = new mysqli($host, $username, $password, $database);

// Check connection
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}

// Fetch all comments
$result = $conn->query("SELECT comment, created_at FROM comments ORDER BY id DESC");

// Close the connection
$conn->close();
?>

<!DOCTYPE html>
<html>
<head><title>Course Comments Forum</title></head>
<body>
    <h3>Comments</h3>
    <?php while ($row = $result->fetch_assoc()): ?>
        <p><strong><?= $row['created_at'] ?>:</strong><br>
        <?= $row['comment']?></p>
        <hr>
    <?php endwhile; ?>

</body>
</html>
