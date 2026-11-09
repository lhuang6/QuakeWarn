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

    // New Tabulator (External Javascript library) table for SHELTERS
    table = new Tabulator("#shelter-table", {

        // URL for C backend to send JSON
        ajaxURL: "/api/shelters",

        // Taking URL and responses to put on the table to update
        ajaxResponse: function(url, params, response) {
            return response;
        },

        // Fit the columns to the right size
        layout: "fitColumns",

        // Adding the fields from the geojson into the table
        columns: [
            { title: "Name", field: "properties.shelter_name" },
            { title: "Status Code", field: "properties.shelter_status_code" },
            { title: "Address", field: "properties.address_1" },
            { title: "State", field: "properties.state" },
        ]
    });

    // New Tabulator (External Javascript library) table for EATHQUAKES
    table = new Tabulator("#earthquake-table", {

        // URL for C backend to send JSON
        ajaxURL: "/api/earthquakes",

        // Taking URL and responses to put on the table to update
        ajaxResponse: function(url, params, response) {
            return response;
        },

        // Fit the columns to the right size
        layout: "fitColumns",
        height: "500px",

        // Adding the fields from the geojson into the table
        columns: [
            { title: "Place", field: "properties.place", widthGrow: 3 },
            { title: "Mag", field: "properties.mag", width: 150, hozAlign: "center" },
        ]
    });

    // Requesting a new URL request for the C backend to retrieve shelter data
    const response = await fetch("/api/shelters");

    // Response from the backend
    const shelterData = await response.json();

    // Adding each shelter to the map
    const shelterIcon = L.icon({
        iconUrl: "icons/house.png",
        iconSize: [32, 32],
        iconAnchor: [16, 32],
        popupAnchor: [0, -32]
    });

    // Adding all shelter icons
    L.geoJSON(shelterData, {
        // Pointing to the latitude and longitude from GeoJSON
        pointToLayer: function(feature, latlng) {
            return L.marker(latlng, {
                icon: shelterIcon
            });
        },

        // Labeling the marker with the shelter name
        onEachFeature: function(feature, layer) {
            layer.bindPopup(feature.properties.shelter_name);

            layer.on("click", function(event) {

            });
        }
    }).addTo(map);

    // Requesting a URL request for C backend to recieve earthquake data
    const earthquakes = await fetch("/api/earthquakes");

    // Response from backend
    const earthquakeData = await earthquakes.json();

    // Adding each earthquake to the map
    L.geoJSON(earthquakeData, {
        pointToLayer: function(feature, latlng) {
            // Plotting a red marker for all earthquakes
            return L.circleMarker(latlng, {
                radius: 8,
                color: "black",
                fillColor: "red",
                fillOpacity: 1,
                stroke: true,
            });
        },

        // Function for each marker
        onEachFeature: function(feature, layer) {
            const latlng = layer.getLatLng();

            // Labeling the marker with the place where it is located
            layer.bindPopup(feature.properties.place);

            layer.on("click", function(event) {
                // Changing the values of the earthquake information tab
                document.getElementById("magnitude").textContent = `Magnitude: ${feature.properties.mag}`;
                document.getElementById("location").textContent = `Location: ${feature.properties.place}`;
                document.getElementById("latitude").textContent = `Latitude: ${latlng[0]}`;
                document.getElementById("longitude").textContent = `Longitude: ${latlng[1]}`;
            });
        }
    }).addTo(map);

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
