
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

    console.log("Hey");

    await table.setData("/api/refresh-shelters");

    // Requesting a new URL request for the C backend
    // const response = await fetch("/api/refresh-shelters");

    // Response from the backend
    // const data = await response.json();

    // Update HTML
    //document.getElementById("magnitude").textContent = `Magnitude: ${data.magnitude}`;
    // document.getElementById("location").textContent = `Location: ${data.location}`;
});

const submitLocation = document.getElementById("submitLocation");

// Updating the table based on the location
submitLocation.addEventListener("click", async () => {
    console.log("Hey");

    /*console.log("Form submitted!");

    // Fetching latitude and longitude from the inputted values in html
    const latitude = document.getElementById("latitude").value;
    const longitude = document.getElementById("longitude").value;

    const payload = {
        latitude: Number(latitude),
        longitude: Number(longitude)
    };

    console.log("Hey");
    console.log("Sending:", payload);*/

    /*// Send the value to the C backend
    const response = await fetch("/api/sort-shelter-location", {
        method: "POST",

        // Sending it as a JSON
        headers: {
            "Content-Type": "application/json"
        },

        // Turning it into a JSON
        body: JSON.stringify({
            latitude: Number(latitude),
            longitude: Number(longitude)
        })
    });

    const data = await response.json();

    table.setData(data);*/
});
