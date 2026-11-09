const refereshButton = document.getElementById("refreshButton");

refereshButton.addEventListener("click", async () => {

    // Requesting a new URL request for the C backend
    const response = await fetch("/api/refresh-shelters");

    // Response from the backend
    const data = await response.json();

    // Update HTML
    //document.getElementById("magnitude").textContent = `Magnitude: ${data.magnitude}`;
    //document.getElementById("location").textContent = `Location: ${data.location}`;
});
