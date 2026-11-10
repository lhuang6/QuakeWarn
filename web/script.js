let shelterLayer;
let quakeLayer;
let quakeTable;
let shelterTable;
let locationMarker;

// Custom color table for earthquake markers
function getMagnitudeColor(magnitude) {
    return magnitude >= 7 ? "#8000ff" :
           magnitude >= 6 ? "#ff0000" :
           magnitude >= 5 ? "#ff8000" :
           magnitude >= 4 ? "#ffff00" :
           magnitude >= 3 ? "#00ff00" :
           magnitude >= 2 ? "#00bfff" :
                           "#0000ff";
}

const map = L.map('map').setView([37.8, -96], 4);

// Add OpenStreetMap base tiles
L.tileLayer('https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png', {
    maxZoom: 19,
    attribution: '&copy; OpenStreetMap contributors'
}).addTo(map);

const tableSelect = document.getElementById("table-select");

const shelterContainer = document.getElementById("shelter-table-container");
const earthquakeContainer = document.getElementById("earthquake-table-container");

tableSelect.addEventListener("change", function() {

    if (tableSelect.value === "shelters") {
        shelterContainer.style.display = "block";
        earthquakeContainer.style.display = "none";

        shelterTable.redraw(true);
    }

    else if (tableSelect.value === "earthquakes") {
        shelterContainer.style.display = "none";
        earthquakeContainer.style.display = "block";

        earthquakeTable.redraw(true);
    }
});

// Listens for when the HTML page is opened up
document.addEventListener("DOMContentLoaded", async () => {

    // New Tabulator (External Javascript library) table for SHELTERS
    shelterTable = new Tabulator("#shelter-table", {

        // URL for C backend to send JSON
        ajaxURL: "/api/shelters",

        // Taking URL and responses to put on the table to update
        ajaxResponse: function(url, params, response) {
            return response;
        },

        // Fit the columns to the right size
        layout: "fitColumns",
        height: "650px",

        // Adding the fields from the geojson into the table
        columns: [
            { title: "Name", field: "properties.shelter_name" },
            { title: "Status Code", field: "properties.shelter_status_code" },
            { title: "Address", field: "properties.address_1" },
            { title: "State", field: "properties.state" },
        ]
    });

    // New Tabulator (External Javascript library) table for EATHQUAKES
    quakeTable = new Tabulator("#earthquake-table", {

        // URL for C backend to send JSON
        ajaxURL: "/api/earthquakes",

        // Taking URL and responses to put on the table to update
        ajaxResponse: function(url, params, response) {
            return response;
        },

        // Fit the columns to the right size
        layout: "fitColumns",
        height: "650px",

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
    shelterLayer = L.geoJSON(shelterData, {
        // Pointing to the latitude and longitude from GeoJSON
        pointToLayer: function(feature, latlng) {
            return L.marker(latlng, {
                icon: shelterIcon
            });
        },

        // Labeling the marker with the shelter name
        onEachFeature: function(feature, layer) {

            // Getting address and coordinates
            const latlng = layer.getLatLng();
            let fullAddress = `${feature.properties.address_1}, ${feature.properties.city}, ${feature.properties.state} ${feature.properties.zip}`;

            // Checking if distance is null then rounding the number
            // Can't round a null number
            let distance = null;
            if (feature.geometry.distance != null) {
                distance = feature.geometry.distance.toFixed(2);
            }

            // Shelter information when the marker is clicked
            const popupContent = `
                <div class="earthquake-popup">
                    <h2>${feature.properties.shelter_name}</h2>

                    <hr>

                    <p style="font-size: 14px;"><strong>Address:</strong> ${fullAddress}</p>
                    <p style="font-size: 14px;"><strong>Latitude:</strong> ${latlng.lat.toFixed(2)}</p>
                    <p style="font-size: 14px;"><strong>Longitude:</strong> ${latlng.lng.toFixed(2)}</p>
                    <p style="font-size: 14px;"><strong>Status:</strong> ${feature.properties.shelter_status_code}</p>

                    <hr>

                    <p style="font-size: 14px;"><strong>Distance:</strong> ${distance} km</p>
                </div>
            `;

            layer.bindPopup(popupContent);
        }
    }).addTo(map);

    // Requesting a URL request for C backend to recieve earthquake data
    const earthquakes = await fetch("/api/earthquakes");

    // Response from backend
    const earthquakeData = await earthquakes.json();

    // Adding each earthquake to the map
    quakeLayer = L.geoJSON(earthquakeData, {
        pointToLayer: function(feature, latlng) {
            let markerRadius = feature.properties.mag * 3 + 2;

            // Plotting a red marker for all earthquakes
            return L.circleMarker(latlng, {
                radius: markerRadius,
                color: "black",
                fillColor: getMagnitudeColor(feature.properties.mag),
                fillOpacity: 1,
                stroke: true,
            });
        },

        // Function for each marker
        onEachFeature: function(feature, layer) {
            // Getting latitude and longitude
            const latlng = layer.getLatLng();

            // Checking if distance is null then rounding the number
            // Can't round a null number
            let distance = null;
            if (feature.geometry.distance != null) {
                distance = feature.geometry.distance.toFixed(2);
            }

            // Earthquake information when the marker is clicked
            const popupContent = `
                <div class="earthquake-popup">
                    <h2>${feature.properties.place}</h2>

                    <hr>

                    <p style="font-size: 14px;"><strong>Magnitude:</strong> ${feature.properties.mag}</p>
                    <p style="font-size: 14px;"><strong>Latitude:</strong> ${latlng.lat.toFixed(2)}</p>
                    <p style="font-size: 14px;"><strong>Longitude:</strong> ${latlng.lng.toFixed(2)}</p>

                    <hr>

                    <p style="font-size: 14px;"><strong>Distance:</strong> ${distance} km</p>
                </div>
            `;

            layer.bindPopup(popupContent);
        }
    }).addTo(map);

    L.control.layers(null, {
        "Earthquakes": quakeLayer,
        "Shelters": shelterLayer
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

        // Send coordinates to sort out shelters
        const shelterResponse = await fetch("/api/sort-shelter-location", {
            method: "POST",
            headers: {
                "Content-Type": "application/json",
                "Accept": "application/json"
            },
            body: JSON.stringify(payload)
        });

        // Check HTTP response
        if (!shelterResponse.ok) {
            throw new Error(`HTTP error: ${shelterResponse.status}`);
        }

        // Receive sorted shelter data
        const shelterData = await shelterResponse.json();

        // Send coordinates to sort earthquakes
        const quakeResponse = await fetch("/api/sort-earthquake-location", {
            method: "POST",
            headers: {
                "Content-Type": "application/json",
                "Accept": "application/json"
            },
            body: JSON.stringify(payload)
        });

        // Check HTTP response
        if (!quakeResponse.ok) {
            throw new Error(`HTTP error: ${response.status}`);
        }

        const quakeData = await quakeResponse.json();

        // Update Tabulator tables
        shelterTable.setData(shelterData);
        quakeTable.setData(quakeData);

        // Updating layers with distances
        shelterLayer.clearLayers();
        shelterLayer.addData(shelterData);

        quakeLayer.clearLayers();
        quakeLayer.addData(quakeData);

        // Removing the old marker if already set prior
        if (locationMarker) {
            locationMarker.remove();
        }
        locationMarker = L.marker([latitude, longitude]).addTo(map).bindPopup("Your Location").openPopup();

    } catch (error) {
        console.error("Failed to send data:", error.message);
    }
});
