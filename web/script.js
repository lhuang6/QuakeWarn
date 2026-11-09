
// Listens for when the HTML page is opened up
document.addEventListener("DOMContentLoaded", () => {

    // New Tabulator (External Javascript library) table for shelters
    const table = new Tabulator("#earthquake-table", {

        // URL for C backend to send JSON
        ajaxURL: "/api/shelters",

        layout: "fitColumns",
        columns: [
            { title: "Name", field: "shelter_name" },
            { title: "Status Code", field: "shelter_status_code" },
            { title: "Address", field: "address_1" }
        ]
    });
});


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

