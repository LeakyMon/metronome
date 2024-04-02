const apiBaseUrl = 'http://your-rpi-ip-address:port';

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
