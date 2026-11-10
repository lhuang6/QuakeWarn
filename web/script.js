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

// Submit coordinates to C backend
async function submitCoordinates(url, latitude, longitude) {

    // Create JSON payload
    const payload = {
        latitude: Number(latitude),
        longitude: Number(longitude)
    };

    // Send coordinates to C backend
    const response = await fetch(url, {
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

    return response;
}

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
            { title: "Address", field: "properties.address_1" },
            { title: "State", field: "properties.state", width: 100},
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
            { title: "Mag", field: "properties.mag", width: 150},
        ]
    });

    // Requesting a new URL request for the C backend to retrieve shelter data
    const response = await fetch("/api/shelters");

    // Response from the backend
    const shelterData = await response.json();

    // Adding each shelter to the map
    const shelterIcon = L.icon({
        iconUrl: "icons/house.png",
        iconSize: [25, 25],
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

// Selecting tables dropdown variable
const tableSelect = document.getElementById("table-select");

// Contains map of shelter and earthquake
const shelterContainer = document.getElementById("shelter-table-container");
const earthquakeContainer = document.getElementById("earthquake-table-container");

// Dropdown for sorting methods
const sortSelect = document.getElementById("sort-select");

// Changing the sorting methods based on which table visible
tableSelect.addEventListener("change", function() {

    // Redrawing the table and changing dropdowns
    if (tableSelect.value === "shelters") {
        shelterContainer.style.display = "block";
        earthquakeContainer.style.display = "none";

        sortSelect.innerHTML = `
            <option value="distance">Distance</option>
        `;

        shelterTable.redraw(true);
    }

    // Redrawing the table and changing dropdowns
    else if (tableSelect.value === "earthquakes") {
        shelterContainer.style.display = "none";
        earthquakeContainer.style.display = "block";

        sortSelect.innerHTML = `
            <option value="magnitude">Magnitude</option>
            <option value="distance">Distance</option>
            <option value="impact">Impact</option>
        `;

        quakeTable.redraw(true);
    }
});

// Sorting the table based on different dropdowns
sortSelect.addEventListener("change", async () => {

    // If the user wants to sort any table by closest distance
    if (sortSelect.value === "Distance") {

        // Latitude and Longitude specified by the user
        const latitude = Number(
            document.getElementById("submitLatitude").value
        );
        const longitude = Number(
            document.getElementById("submitLongitude").value
        );

        // If the user is on a shelter table and wants to sort by distance
        if (tableSelect.value === "shelters") {

            // Response from C backend
            const shelterResponse = await submitCoordinates("/api/sort-shelter-location", latitude, longitude);
            const shelterData = await shelterResponse.json();

            shelterTable.setData(shelterData);
        }

        // If the user is on an earthquake table and wants to sort by distance
        if (tableSelect.value === "earthquakes") {

            // Response from C backend
            const quakeResponse = await submitCoordinates("/api/sort-earthquake-location", latitude, longitude);
            const quakeData = await quakeResponse.json();

            quakeTable.setData(quakeData);
        }
    }

    if (sortSelect.value === "Distance")
});

const refreshButton = document.getElementById("refreshButton");

refreshButton.addEventListener("click", async () => {

    await table.setData("/api/refresh-shelters");
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

        // Receive sorted shelter data
        const shelterResponse = await submitCoordinates("/api/sort-shelter-location", latitude, longitude);
        const shelterData = await shelterResponse.json();

        // Send coordinates to sort earthquakes
        const quakeResponse = await submitCoordinates("/api/sort-earthquake-location", latitude, longitude);
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
