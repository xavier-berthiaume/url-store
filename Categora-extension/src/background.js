import { API_BASE_URL, MAX_RETRIES, DEBUG_MODE } from "./config.js";

const browser = chrome || browser;

async function getToken() {
    if (DEBUG_MODE) {
        console.log("getToken() called");
    }

    const result = await browser.storage.local.get(["userToken"]);

    if (result.userToken) {
        console.log("Token found locally:", result.userToken);
        return result.userToken;
    }

    console.log("No token found locally. Fetching from Categora server...");

    try {
        const response = await fetch(`${API_BASE_URL}/auth`, {
            method: "GET",
            headers: {
                "Content-Type": "application/json",
            },
        });

        if (!response.ok) {
            throw new Error(`HTTP error! Status: ${response.status}`);
        }

        const data = await response.json();
        const token = data.token;

        await browser.storage.local.set({ userToken: token });
        console.log("Token saved locally:", token);

        return token;
    } catch (error) {
        console.error("Failed to fetch token:", error);
        throw error;
    }
}

async function getUrls() {
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

    try {
        const response = await fetch(`${API_BASE_URL}/url`, {
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
    if (DEBUG_MODE) {
        console.log("saveUrl() called");
    }

    const tokenResult = await browser.storage.local.get(["userToken"]);
    const token = tokenResult.userToken;

    if (!token) {
        throw new Error("No token found. Please authenticate first.");
    }

    try {
        const response = await fetch(`${API_BASE_URL}/url`, {
            method: "POST",
            headers: {
                "Authorization": `Bearer ${token}`,
                "Content-Type": "application/json",
            },
            body: JSON.stringify({ url: urlString })
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

browser.runtime.onMessage.addListener((request, sender, sendResponse) => {
    if (request.action === "getToken") {
        getToken()
            .then((token) => sendResponse({ success: true, token }))
            .catch((error) => sendResponse({ success: false, error: error.message }));
    } else if (request.action === "getUrls") {
        getUrls()
            .then((urls) => sendResponse({ success: true, urls }))
            .catch((error) => sendResponse({ success: false, error: error.message }));
    } else if (request.action === "saveUrl" && request.url) {
        saveUrl(request.url)
            .then(() => sendResponse({ success: true }))
            .catch((error) => sendResponse({ success: false, error: error.message }));
    }

    // Return true to indicate that sendResponse will be called asynchronously
    return true;
});
