<?php
session_start();
?>

<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>67607 XSS Lab</title>
</head>
<body>
  <?php if (!empty($_SESSION['username'])): ?>
    <h1>Welcome, <?php echo htmlspecialchars($_SESSION['username'], ENT_QUOTES | ENT_SUBSTITUTE, 'UTF-8'); ?>!</h1>
  <?php else: ?>
    <h1>Welcome to 67607 XSS Lab!</h1>
  <?php endif; ?>
</body>
</html>

