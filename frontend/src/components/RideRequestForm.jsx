import React, { useState } from 'react';

const RideRequestForm = ({ onRequestRide, loading }) => {
  const [pickup, setPickup] = useState('A');
  const [destination, setDestination] = useState('D');

  const handleSubmit = (e) => {
    e.preventDefault();
    onRequestRide(pickup, destination);
  };

  return (
    <div className="glass-panel fade-in">
      <h2 style={{ marginBottom: '1.5rem', color: '#00d2ff' }}>Request a Ride</h2>
      <form onSubmit={handleSubmit}>

        
        <div className="grid-layout" style={{ gap: '1rem' }}>
          <div>
            <label style={{ display: 'block', marginBottom: '0.5rem', fontSize: '0.9rem' }}>Pickup Node</label>
            <select className="input-field" value={pickup} onChange={e => setPickup(e.target.value)}>
              {['A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J'].map(n => <option key={n} value={n}>{n}</option>)}
            </select>
          </div>
          <div>
            <label style={{ display: 'block', marginBottom: '0.5rem', fontSize: '0.9rem' }}>Destination Node</label>
            <select className="input-field" value={destination} onChange={e => setDestination(e.target.value)}>
              {['A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J'].map(n => <option key={n} value={n}>{n}</option>)}
            </select>
          </div>
        </div>

        <button type="submit" className="primary-btn" disabled={loading || pickup === destination}>
          {loading ? 'Finding Driver...' : 'Request Ride'}
        </button>
      </form>
    </div>
  );
};

export default RideRequestForm;
