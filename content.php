<div id="global" class="settings">
<?
        $protocol = (!empty($_SERVER['HTTPS']) && $_SERVER['HTTPS'] !== 'off') ? "https://" : "http://";
        $host = $_SERVER['SERVER_NAME'] ?? 'localhost';
        $url = $protocol . $host . ":8089";
        echo '<h1>PulseMesh Settings</h1><p><a href="' . htmlspecialchars($url) . '">Click Here</a> to visit the PulseMesh settings page to configure the token or debug the PulseMesh connector.</p><p>Note - don\'t use the legacy settings below unless specifically directed to do so.</p>';
        
        PrintSettingGroup("PulseMeshSettings", "", "", 1, "fpp-pulsemesh");
?>

<h2>Stream Loading Time Configuration</h2>
<form action="stream_loading_time.php" method="post">
    <label for="streamLoadingTime">Stream Loading Time (seconds):</label>
    <input type="number" id="streamLoadingTime" name="streamLoadingTime" min="0" max="300" value="0">
    <input type="submit" value="Save">
</form>
</div>
