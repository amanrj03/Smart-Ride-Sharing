import React from 'react';

const DriverList = ({ drivers }) => {
  return (
    <div className="glass-panel">
      <h3 style={{ marginBottom: '1rem' }}>Active Drivers</h3>
      
      {drivers.length === 0 ? (
        <p style={{ color: '#aaa' }}>No drivers available.</p>
      ) : (
        <div style={{ display: 'flex', flexDirection: 'column', gap: '0.5rem' }}>
          {drivers.map(d => (
            <div key={d.id} className="driver-item fade-in">
              <div>
                <strong>{d.name}</strong> (ID: {d.id})
                <div style={{ fontSize: '0.8rem', color: '#aaa' }}>Location: Node {d.location}</div>
              </div>
              <span className={`badge ${d.available ? 'available' : 'busy'}`}>
                {d.available ? 'Available' : 'Busy'}
              </span>
            </div>
          ))}
        </div>
      )}
    </div>
  );
};

export default DriverList;
