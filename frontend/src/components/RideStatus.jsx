import React from 'react';

const RideStatus = ({ status }) => {
  if (!status) return null;

  const isSuccess = status.status === 'success';

  return (
    <div className="glass-panel fade-in" style={{
      borderLeft: `4px solid ${isSuccess ? '#2ed573' : '#ff4757'}`,
      background: isSuccess ? 'rgba(46, 213, 115, 0.05)' : 'rgba(255, 71, 87, 0.05)'
    }}>
      <h3 style={{ color: isSuccess ? '#2ed573' : '#ff4757', marginBottom: '1rem' }}>
        {isSuccess ? 'Ride Confirmed!' : 'Ride Failed'}
      </h3>

      {isSuccess ? (
        <div style={{ display: 'flex', flexDirection: 'column', gap: '0.5rem' }}>
          <div><strong>Driver:</strong> {status.driverName} (ID: {status.driverId})</div>
          <div><strong>Distance:</strong> {status.distance} km</div>
          {status.path && status.path.length > 0 && (
            <div><strong>Path:</strong> {status.path.join(" -> ")}</div>
          )}
          <div style={{ fontSize: '1.5rem', color: '#00d2ff', marginTop: '0.5rem' }}>
            <strong>Fare: Rs. {status.fare}</strong>
          </div>
          <p style={{ fontSize: '0.8rem', color: '#aaa', marginTop: '1rem' }}>
            {status.message}
          </p>
        </div>
      ) : (
        <p>{status.message}</p>
      )}
    </div>
  );
};

export default RideStatus;
