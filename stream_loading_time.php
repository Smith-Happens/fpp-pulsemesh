<?php

if ($_SERVER["REQUEST_METHOD"] == "POST") {
    $streamLoadingTime = intval($_POST["streamLoadingTime"]);
    $settingsFile = "settings.json";
    $settings = json_decode(file_get_contents($settingsFile), true);
    $settings["settings"]["StreamLoadingTime"]["default"] = $streamLoadingTime;
    file_put_contents($settingsFile, json_encode($settings, JSON_PRETTY_PRINT));
    echo "Stream loading time updated successfully.";
} else {
    $settingsFile = "settings.json";
    $settings = json_decode(file_get_contents($settingsFile), true);
    $streamLoadingTime = $settings["settings"]["StreamLoadingTime"]["default"];
    echo '<form action="stream_loading_time.php" method="post">';
    echo '<label for="streamLoadingTime">Stream Loading Time (seconds):</label>';
    echo '<input type="number" id="streamLoadingTime" name="streamLoadingTime" min="0" max="300" value="' . htmlspecialchars($streamLoadingTime) . '">';
    echo '<input type="submit" value="Save">';
    echo '</form>';
}
?>
