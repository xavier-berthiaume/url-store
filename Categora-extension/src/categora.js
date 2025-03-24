// Listen for messages from the background script
browser.runtime.onMessage.addListener((request, sender, sendResponse) => {
  if (request.action === "triggerSaveUI") {
    showSaveUI();
  }
});

// Function to display the save UI
function showSaveUI() {
  // Example: Inject a small form into the page
  const saveDialog = document.createElement('div');
  saveDialog.innerHTML = `
    <div style="position: fixed; top: 10px; right: 10px; background: white; padding: 10px; border: 1px solid #ccc;">
      <input type="text" id="tags" placeholder="Enter tags">
      <button id="saveButton">Save</button>
    </div>
  `;
  document.body.appendChild(saveDialog);

  // Handle save button click
  document.getElementById('saveButton').addEventListener('click', () => {
    const tags = document.getElementById('tags').value;
    saveCurrentPage(tags);
    document.body.removeChild(saveDialog);
  });
}

// Save the current URL with tags
function saveCurrentPage(tags) {
  const url = window.location.href;
  browser.runtime.sendMessage({ action: "saveUrl", url, tags });
}
