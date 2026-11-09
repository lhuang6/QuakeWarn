
// Listens for when the HTML page is opened up
document.addEventListener("DOMContentLoaded", () => {

    // New Tabulator (External Javascript library) table for shelters
    table = new Tabulator("#earthquake-table", {

        // URL for C backend to send JSON
        ajaxURL: "/api/shelters",

        ajaxResponse: function(url, params, response) {
            return response;
        },

        layout: "fitColumns",
        columns: [
            { title: "Name", field: "properties.shelter_name" },
            { title: "Status Code", field: "properties.shelter_status_code" },
            { title: "Address", field: "properties.address_1" },
            { title: "State", field: "properties.state" },
        ]
    });
});


const refereshButton = document.getElementById("refreshButton");

refereshButton.addEventListener("click", async () => {

    await table.setData("/api/refresh-shelters");

    // Requesting a new URL request for the C backend
    // const response = await fetch("/api/refresh-shelters");

    // Response from the backend
    // const data = await response.json();

    // Update HTML
    //document.getElementById("magnitude").textContent = `Magnitude: ${data.magnitude}`;
    // document.getElementById("location").textContent = `Location: ${data.location}`;
});

const form = document.getElementById("shelterForm");

form.addEventListener("submit", async (event) => {

    const latitude = document.getElementById("latitude").value;
    const longitude = document.getElementById("longitude").value;

    // Send the value to the C backend
    const response = await fetch("/api/search-shelters", {
        method: "POST",
        headers: {
            "Content-Type": "application/json"
        },
        body: JSON.stringify({
            latitude: latitude,
            longitude: longitude
        })
    });

    const data = await response.json();
});
