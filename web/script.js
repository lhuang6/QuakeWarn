// Initialize the map
const map = L.map('map').setView([37.8, -96], 4);

// Add OpenStreetMap base tiles
L.tileLayer('https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png', {
    maxZoom: 19,
    attribution: '&copy; OpenStreetMap contributors'
}).addTo(map);

// Load and render your GeoJSON data file
fetch('data.geojson')
    .then(response => response.json())
    .then(geojsonData => {
        const geojsonLayer = L.geoJSON(geojsonData).addTo(map);

        // Zoom map to fit data bounds
        if (geojsonLayer.getBounds().isValid()) {
            map.fitBounds(geojsonLayer.getBounds());
        }
    })
    .catch(error => console.error('Error loading GeoJSON:', error));
