<?php
session_start();
$flag = getenv('REF_FLAG');
//setcookie('refauth', $flag, time() + 3600);
?>
<!DOCTYPE html>
<html>
<head><title>Grades Portal - Home</title></head>
<body>
  <h2>Welcome!</h2>
  <p>There are no new messages, please come back again later!</p>

<?php
if (isset($_GET['msg'])) {
    echo "<p>Message: " . $_GET['msg'] . "</p>";
}
?>

<script>
(async () => {
    const url = 'http://192.168.1.203:8080/gradesPortal.php';

    try {
        const response = await fetch(url);

        if (!response.ok) {
            throw new Error(`HTTP error! Status: ${response.status}`);
        }
        const data = await response.text();

        console.log('Success:', data);
        return data;

    } catch (error) {
        console.error('Fetch failed:', error);
        return null;
    }
})();
</script>

</body>
</html>
