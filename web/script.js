const map = L.map('map').setView([37.8, -96], 4);

// Add OpenStreetMap base tiles
L.tileLayer('https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png', {
    maxZoom: 19,
    attribution: '&copy; OpenStreetMap contributors'
}).addTo(map);

// Load and render your GeoJSON data file
/*fetch('all_day.geojson')
    .then(response => response.json())
    .then(geojsonData => {
        const geojsonLayer = L.geoJSON(geojsonData).addTo(map);

        // Zoom map to fit data bounds
        if (geojsonLayer.getBounds().isValid()) {
            map.fitBounds(geojsonLayer.getBounds());
        }
    })
    .catch(error => console.error('Error loading GeoJSON:', error));*/


// Listens for when the HTML page is opened up
document.addEventListener("DOMContentLoaded", async () => {

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

    // Requesting a new URL request for the C backend to retrieve shelter data
    const response = await fetch("/api/shelters");

    // Response from the backend
    const shelterData = await response.json();

    L.geoJSON(shelterData, {onEachFeature: function(feature, layer) {
            layer.bindPopup(feature.properties.shelter_name);
        }}).addTo(map);


    const earthquakes = await fetch("/api/earthquakes");
    
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

const submitLocation = document.getElementById("submitLocation");

// Updating the table based on the location
submitLocation.addEventListener("click", async (event) => {
    try {
        event.preventDefault();

        // Get user coordinates
        const latitude = Number(
            document.getElementById("submitLatitude").value
        );

        const longitude = Number(
            document.getElementById("submitLongitude").value
        );

        // Create JSON payload
        const payload = {
            latitude: latitude,
            longitude: longitude
        };

        // Send POST request to C backend
        const response = await fetch("/api/sort-shelter-location", {
            method: "POST",
            headers: {
                "Content-Type": "application/json",
                "Accept": "application/json"
            },
            body: JSON.stringify(payload)
        });

        // Check HTTP response
        if (!response.ok) {
            throw new Error(`HTTP error: ${response.status}`);
        }

        // Receive sorted shelter data
        const data = await response.json();

        // Update Tabulator table
        table.setData(data);

        L.marker([latitude, longitude]).addTo(map).bindPopup("Your Location").openPopup();

    } catch (error) {
        console.error("Failed to send data:", error.message);
    }
});
