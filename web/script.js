
// Listens for when the HTML page is opened up
document.addEventListener("DOMContentLoaded", () => {

    // New Tabulator (External Javascript library) table for shelters
    const table = new Tabulator("#earthquake-table", {

        // URL for C backend to send JSON
        ajaxURL: "/api/shelters",

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

    table.setData("/api/refresh-shelters");

    // Requesting a new URL request for the C backend
    // const response = await fetch("/api/refresh-shelters");

    // Response from the backend
    // const data = await response.json();

    // Update HTML
    //document.getElementById("magnitude").textContent = `Magnitude: ${data.magnitude}`;
    // document.getElementById("location").textContent = `Location: ${data.location}`;
});

