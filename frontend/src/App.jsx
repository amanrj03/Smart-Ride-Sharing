import { useState, useEffect } from 'react';
import './index.css';
import MapView from './components/MapView';
import DriverList from './components/DriverList';
import RideRequestForm from './components/RideRequestForm';
import RideStatus from './components/RideStatus';

const API_BASE = '/api';

function App() {
  const [drivers, setDrivers] = useState([]);
  const [rideStatus, setRideStatus] = useState(null);
  const [loading, setLoading] = useState(false);

  const fetchState = async () => {
    try {
      const res = await fetch(`${API_BASE}/drivers`);
      const data = await res.json();
      setDrivers(data.drivers || []);
    } catch (err) {
      console.error("Failed to fetch drivers", err);
    }
  };

  useEffect(() => {
    fetchState();
    const interval = setInterval(fetchState, 3000);
    return () => clearInterval(interval);
  }, []);

  const handleRequestRide = async (pickup, destination) => {
    setLoading(true);
    setRideStatus(null);
    try {
      const res = await fetch(`${API_BASE}/request_ride`, {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ pickup, destination })
      });
      const data = await res.json();
      setRideStatus(data);
      fetchState(); // refresh drivers instantly
    } catch (err) {
      setRideStatus({ status: 'error', message: 'Failed to connect to the server.' });
    }
    setLoading(false);
  };

  return (
    <div className="App">
      <div style={{ textAlign: 'center', marginBottom: '2rem' }}>
        <h1 style={{ fontSize: '2.5rem', background: 'linear-gradient(to right, #00d2ff, #3a7bd5)', WebkitBackgroundClip: 'text', WebkitTextFillColor: 'transparent' }}>
          Smart Ride-Sharing
        </h1>
        <p style={{ color: '#aaa' }}>Powered by C++ Dijkstra & React</p>
      </div>

      <div className="grid-layout">
        <div style={{ display: 'flex', flexDirection: 'column', gap: '2rem' }}>
          <RideRequestForm onRequestRide={handleRequestRide} loading={loading} />
          {rideStatus && <RideStatus status={rideStatus} />}
        </div>
        
        <div style={{ display: 'flex', flexDirection: 'column', gap: '2rem' }}>
          <MapView drivers={drivers} />
          <DriverList drivers={drivers} />
        </div>
      </div>
    </div>
  );
}

export default App;
