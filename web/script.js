// Waiting for HTML page to open
document.addEventListener("DOMContentLoaded", () => {
    loadEarthquakes();
});

// Sends in JSON data when html is opened
async function loadEarthquakes() {

    const response = await fetch("/api/earthquakes");

    const data = await response.json();

    populateTable(data);
}

function populateTable(data) {

    const table = document.getElementById("earthquakeTable");

    data.forEach(earthquake => {

        const row = table.insertRow();

        row.insertCell().textContent = earthquake.magnitude;
        row.insertCell().textContent = earthquake.location;
    });
}

const refereshButton = document.getElementById("refreshButton");

refereshButton.addEventListener("click", async () => {

    // Requesting a new URL request for the C backend
    // const response = await fetch("/api/refresh-shelters");

    // Response from the backend
    // const data = await response.json();

    // Update HTML
    //document.getElementById("magnitude").textContent = `Magnitude: ${data.magnitude}`;
    // document.getElementById("location").textContent = `Location: ${data.location}`;
});

