import React from 'react';

const MapView = ({ drivers }) => {
  // Hardcoded graph visualization logic for the project
  const nodes = {
    A: { x: 10, y: 20 },
    B: { x: 35, y: 15 },
    C: { x: 65, y: 15 },
    D: { x: 90, y: 20 },
    E: { x: 15, y: 50 },
    F: { x: 45, y: 45 },
    G: { x: 65, y: 50 },
    H: { x: 20, y: 85 },
    I: { x: 50, y: 80 },
    J: { x: 90, y: 85 },
  };

  const edges = [
    { from: 'A', to: 'B', distance: 3 },
    { from: 'A', to: 'E', distance: 4 },
    { from: 'B', to: 'C', distance: 3 },
    { from: 'B', to: 'F', distance: 4 },
    { from: 'B', to: 'E', distance: 5 },
    { from: 'C', to: 'D', distance: 3 },
    { from: 'C', to: 'G', distance: 4 },
    { from: 'C', to: 'J', distance: 8 },
    { from: 'D', to: 'G', distance: 5 },
    { from: 'E', to: 'F', distance: 3 },
    { from: 'E', to: 'H', distance: 4 },
    { from: 'E', to: 'I', distance: 6 },
    { from: 'F', to: 'I', distance: 4 },
    { from: 'F', to: 'G', distance: 3 },
    { from: 'G', to: 'J', distance: 5 },
    { from: 'H', to: 'I', distance: 3 },
    { from: 'I', to: 'J', distance: 4 },
  ];

  return (
    <div className="glass-panel" style={{ position: 'relative', height: '400px', width: '100%', overflow: 'hidden' }}>
      <h3 style={{ marginBottom: '1rem' }}>Live Map Activity</h3>

      <div style={{ position: 'relative', width: '100%', height: 'calc(100% - 2rem)' }}>

        {/* SVG Layer for clean, non-intersecting edges */}
        <svg style={{ position: 'absolute', top: 0, left: 0, width: '100%', height: '100%', pointerEvents: 'none' }}>
          {edges.map((e, idx) => {
            const n1 = nodes[e.from];
            const n2 = nodes[e.to];
            const midX = (n1.x + n2.x) / 2;
            const midY = (n1.y + n2.y) / 2;

            return (
              <g key={idx}>
                {/* Line */}
                <line
                  x1={`${n1.x}%`} y1={`${n1.y}%`}
                  x2={`${n2.x}%`} y2={`${n2.y}%`}
                  stroke="rgba(255, 255, 255, 0.2)"
                  strokeWidth="3"
                />

                {/* Distance Label Background to prevent overlap */}
                <rect
                  x={`calc(${midX}% - 14px)`}
                  y={`calc(${midY}% - 10px)`}
                  width="28" height="20"
                  fill="#1e1e2f"
                  rx="6"
                />

                {/* Distance Text */}
                <text
                  x={`${midX}%`} y={`${midY}%`}
                  fill="#aaa" fontSize="11"
                  textAnchor="middle" dominantBaseline="central"
                  fontWeight="bold"
                >
                  {e.distance}KM
                </text>
              </g>
            );
          })}
        </svg>

        {/* HTML Layer for interactive nodes */}
        {Object.entries(nodes).map(([name, pos]) => {
          const driverAtNode = drivers.find(d => d.location === name);
          return (
            <div key={name} className={`map-node ${driverAtNode ? 'active' : ''}`} style={{
              left: `calc(${pos.x}% - 20px)`,
              top: `calc(${pos.y}% - 20px)`,
            }}>
              {name}
              {driverAtNode && (
                <div style={{ position: 'absolute', bottom: '-25px', fontSize: '0.8rem', whiteSpace: 'nowrap', color: '#00d2ff', fontWeight: 'bold' }}>
                  🚗 {driverAtNode.name}
                </div>
              )}
            </div>
          );
        })}
      </div>
    </div>
  );
};

export default MapView;
