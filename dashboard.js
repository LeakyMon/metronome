const apiBaseUrl = 'http://rpi-ip-address:port';
//gotta find te arpi ip address 
function refreshBPM() {
    fetch(`${apiBaseUrl}/bpm/`)
        .then(response => response.json())
        .then(data => {
            document.getElementById('current-bpm').textContent = data.bpm;
        });
    fetch(`${apiBaseUrl}/bpm/min/`)
        .then(response => response.json())
        .then(data => {
            document.getElementById('min-bpm').textContent = data.min;
        });
    fetch(`${apiBaseUrl}/bpm/max/`)
        .then(response => response.json())
        .then(data => {
            document.getElementById('max-bpm').textContent = data.max;
        });
}

function setBPM() {
    console.log("setinngs bpm");
    var newBpm = document.getElementById('new-bpm').value; // Get the new BPM value from the input field
    document.getElementById('current-bpm').innerText = newBpm; // Set the new BPM value to the current BPM display

    // Optionally, you can update the min and max BPM based on the new value
    var minBpm = document.getElementById('min-bpm').innerText;
    var maxBpm = document.getElementById('max-bpm').innerText;

    if (newBpm < minBpm || minBpm == 0) {
        document.getElementById('min-bpm').innerText = newBpm; // Update min BPM if the new BPM is lower
    }

    if (newBpm > maxBpm) {
        document.getElementById('max-bpm').innerText = newBpm; // Update max BPM if the new BPM is higher
    }

    const newBPM = document.getElementById('new-bpm').value;
    fetch(`${apiBaseUrl}/bpm/`, {
        method: 'PUT',
        headers: {
            'Content-Type': 'application/json',
        },
        body: JSON.stringify({ bpm: newBPM })
    }).then(() => {
        refreshBPM();
    });



    
      
    
}

function resetMinMax() {
    fetch(`${apiBaseUrl}/bpm/min/`, { method: 'DELETE' });
    fetch(`${apiBaseUrl}/bpm/max/`, { method: 'DELETE' }).then(() => {
        refreshBPM();
    });
}


refreshBPM();
