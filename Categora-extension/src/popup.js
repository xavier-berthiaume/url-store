// Automatically call fetchToken at the opening of the script/page
document.addEventListener("DOMContentLoaded", async () => {
    try {
        await fetchToken();
    } catch (error) {
        console.error("Initial token fetch failed:", error);
    }
});

function showStatus(message, isError = false) {
    const statusEl = document.getElementById('statusMessage');
    statusEl.textContent = message;
    statusEl.className = `status-message visible ${isError ? 'error' : 'success'}`;
    setTimeout(() => statusEl.classList.remove('visible'), 3000);
}

async function fetchToken() {
   try {
    const response = await browser.runtime.sendMessage({ action: "getToken" });
    
    if (!response) {
      throw new Error("No response from background script");
    }

    if (response.success) {
      console.log("Token retrieved:", response.token);
      showStatus("Token retrieved");
    } else {
      console.error("Failed to retrieve token:", response.error);
      showStatus(`Error: ${response.error}`, true);
    }
  } catch (error) {
    console.error("fetchToken error:", error);
    showStatus("Connection failed", true);
  }
}

async function fetchUrls() {
    const response = await browser.runtime.sendMessage({ action: "getUrls"});

    if (response.success) {
        console.log("URLs retrieved:", response.urls);
        showStatus("URLs retrieved: " + response.urls);
    } else {
        console.error("Failed to retrieve URLs:", response.error);
        showStatus("Failed to retrieve URLs: " + response.error, true);
    }
}

async function saveUrl() {
    try {
        // Get current active tab
        const [tab] = await browser.tabs.query({ 
            active: true, 
            currentWindow: true 
        });

        if (!tab || !tab.url) {
            console.error("No active tab found or URL unavailable");
            showStatus("No active tab found or URL unavailable", true);
            return;
        }

        const url = tab.url;
        const response = await browser.runtime.sendMessage({ action: "saveUrl", url });

        if (response.success) {
            console.log("URL saved successfully:", url);
            showStatus("URL saved successfully: " + response.url);
            fetchUrls();
        } else {
            console.error("Failed to save URL:", response.error);
            showStatus("Failed to save URL: " + response.error, true);
        }
    } catch (error) {
        console.error("Error getting current tab:", error);
    }
}

async function deleteUrl() {
    try {
        // Get current active tab
        const [tab] = await browser.tabs.query({ 
            active: true, 
            currentWindow: true 
        });

        if (!tab || !tab.url) {
            console.error("No active tab found or URL unavailable");
            showStatus("No active tab found or URL unavailable", true);
            return;
        }

        const url = tab.url;
        const response = await browser.runtime.sendMessage({ action: "deleteUrl", url});

        if (response.success) {
            console.log("URL deleted successfully!");
            showStatus("URL deleted successfully: " + response.url);
        } else {
            console.error("Failed to delete URL:", response.error);
            showStatus("Failed to delete URL: " + response.error, true);
        }
    } catch (error) {
        console.error("Error getting current tab:", error);
    }
}

// Attach event listeners
document.getElementById("authButton").addEventListener("click", fetchToken);
document.getElementById("getUrlsButton").addEventListener("click", fetchUrls);
document.getElementById("saveUrlButton").addEventListener("click", saveUrl);
document.getElementById("deleteUrlButton").addEventListener("click", deleteUrl);
