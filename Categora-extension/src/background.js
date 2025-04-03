// import { API_BASE_URL, MAX_RETRIES, DEBUG_MODE } from "./config.js"; you can't use a config.js in a browser extension
console.log("Background script initialized");

browser.runtime.onInstalled.addListener(() => {
  browser.storage.local.set({
    DEBUG_MODE: true,
    API_URL: "http://localhost:12345",
    MAX_RETRIES: 3
  });
});

async function getToken() {
    const { DEBUG_MODE } = await browser.storage.local.get(["DEBUG_MODE"]);
    if (DEBUG_MODE) {
        console.log("getToken() called");
    }

    try {
        const { userToken } = await browser.storage.local.get(["userToken"]);
        if (userToken) {
            console.log("Token found locally:", userToken);
            return userToken;
        }
    } catch (error) {
        console.log("No token found locally. Fetching from server...");
    }

    const { API_URL } = await browser.storage.local.get(["API_URL"]);
    const apiUrl = API_URL || "http://localhost:12345"; // Fallback URL
    
    try {
        const response = await fetch(`${apiUrl}/auth`, {
            method: "POST",
            headers: { "Content-Type": "text/plain" },
        });

        if (!response.ok) throw new Error(`HTTP error! Status: ${response.status}`);
        
        const data = await response.text();
        const token = data.split(" ")[1];

        await browser.storage.local.set({ userToken: token });
        console.log("Token saved locally:", token);
        return token;
    } catch (error) {
        console.error("Failed to fetch token:", error);
        throw error;
    }
}

async function getUrls() {
    const { DEBUG_MODE } = await browser.storage.local.get(["DEBUG_MODE"]);
    if (DEBUG_MODE) {
        console.log("getUrls() called");
    }

    const tokenResult = await browser.storage.local.get(["userToken"]);
    const token = tokenResult.userToken;

    if (!token) {
        throw new Error("No token found. Please authenticate first.");
    }

    const result = await browser.storage.local.get(["urls"]);

    if (result.urls) {
        console.log("URL's found locally:", result.urls);
        return result.urls;
    }
    
    const { API_URL } = await browser.storage.local.get(["API_URL"]);
    const apiUrl = API_URL || "http://localhost:12345"; // Fallback URL

    try {
        const response = await fetch(`${apiUrl}/url`, {
            method: "GET",
            headers: {
                "Authorization": `Bearer ${token}`,
                "Content-Type": "application/json",
            }
        });

        if (!response.ok) {
            throw new Error(`HTTP error. Status: ${response.status}`);
        }

        
        const data = await response.json();
        console.log("URLs fetched from API:", data);

        await browser.storage.local.set({ urls: data });

        return data;
    } catch (error) {
        console.error("Failed to fetch URL's:", error);
        throw error;
    }
}

async function saveUrl(urlString) {
    const { DEBUG_MODE } = await browser.storage.local.get(["DEBUG_MODE"]);
    if (DEBUG_MODE) {
        console.log("saveUrl() called");
    }

    const tokenResult = await browser.storage.local.get(["userToken"]);
    const token = tokenResult.userToken;

    if (!token) {
        throw new Error("No token found. Please authenticate first.");
    }
    
    const { API_URL } = await browser.storage.local.get(["API_URL"]);
    const apiUrl = API_URL || "http://localhost:12345"; // Fallback URL

    try {
        const response = await fetch(`${apiUrl}/url?url=${urlString}`, {
            method: "POST",
            headers: {
                "Authorization": `Bearer ${token}`,
                "Content-Type": "text/plain",
            }
        });

        if (!response.ok) {
            throw new Error(`HTTP error. Status: ${response.status}`);
        }
        
        console.log("URL saved successfully.");
    } catch (error) {
        console.error("Failed to save URL:", error);
        throw error;
    }
}

async function deleteUrl(urlString) {
    const { DEBUG_MODE } = await browser.storage.local.get(["DEBUG_MODE"]);
    if (DEBUG_MODE) {
        console.log("deleteUrl() called");
    }

    const tokenResult = await browser.storage.local.get(["userToken"]);
    const token = tokenResult.userToken;

    if (!token) {
        throw new Error("No token found. Please authenticate first.");
    }
    
    const { API_URL } = await browser.storage.local.get(["API_URL"]);
    const apiUrl = API_URL || "http://localhost:12345"; // Fallback URL

    try {
        const response = await fetch(`${apiUrl}/url?url=${urlString}`, {
            method: "DELETE",
            headers: {
                "Authorization": `Bearer ${token}`,
                "Content-Type": "text/plain",
            }
        });

        if (!response.ok) {
            throw new Error(`HTTP error. Status: ${response.status}`);
        }
    } catch (error) {
        console.error("Failed to delete URL:", error);
        throw error;
    }
}

browser.runtime.onMessage.addListener((request, sender, sendResponse) => {
    console.log("Received message:", request.action);
    (async () => {
        try {
            switch (request.action) {
                case "getToken":
                    const token = await getToken();
                    sendResponse({ success: true, token });
                    break;
                case "getUrls":
                    const urls = await getUrls();
                    sendResponse({ success: true, urls });
                    break;
                case "saveUrl":
                    await saveUrl(request.url);
                    sendResponse({ success: true });
                    break;
                case "deleteUrl":
                    await deleteUrl(request.url);
                    sendResponse({ success: true });
                    break;
                default:
                    sendResponse({ success: false, error: "Unknown action" });
            }
        } catch (error) {
            console.log("Error encountered before sending response:", error);
            sendResponse({ success: false, error: error.message });
        }
    })();

    return true; // Keep the message port open for async response
});
