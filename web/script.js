
// Waiting for HTML page to open
document.addEventListener("DOMContentLoaded", () => {
    loadEarthquakes();
});

// Sends in JSON data when html is opened
async function loadEarthquakes() {

    // Waits for C response
    const response = await fetch("/api/earthquakes");

    // Gets data from the response the C-HTML web-server made
    const tabledata = await response.json();

    var table = new Tabulator("#shelter-table", {
        data:tabledata,
        autoColumns:true
    })
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

