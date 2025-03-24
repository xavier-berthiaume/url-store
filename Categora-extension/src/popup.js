// Automatically call fetchToken at the opening of the script/page
fetchToken();

function fetchToken() {
    browser.runtime.sendMessage({ action: "getToken" }, (response) => {
        if (response.success) {
            console.log("Token retrieved:", response.token);
        } else {
            console.error("Failed to retrieve token:", response.error);
        }
    });
}

function fetchUrls() {
    browser.runtime.sendMessage({ action: "getUrls" }, (response) => {
        if (response.success) {
            console.log("URLs retrieved:", response.urls);
        } else {
            console.error("Failed to retrieve URLs:", response.error);
        }
    });
}

function saveUrl() {
    const url = document.getElementById("urlInput").value;
    browser.runtime.sendMessage({ action: "saveUrl", url }, (response) => {
        if (response.success) {
            console.log("URL saved successfully!");
        } else {
            console.error("Failed to save URL:", response.error);
        }
    });
}

// Attach event listeners
document.getElementById("getTokenButton").addEventListener("click", fetchToken);
document.getElementById("getUrlsButton").addEventListener("click", fetchUrls);
document.getElementById("saveUrlButton").addEventListener("click", saveUrl);
