import React from 'react';
import './WeatherGauge.css';

function WeatherGauge({ title, value }) {
  return (
    <div className="weather-gauge-box">
      <div className="weather-gauge">
        <h3 className="weather-gauge-title">{title}</h3>
        {/* Add gauge component here */}
        <p>Value: {value}</p>
      </div>
    </div>
  );
}

export default WeatherGauge;
