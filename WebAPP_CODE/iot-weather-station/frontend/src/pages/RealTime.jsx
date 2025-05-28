import { useEffect, useState } from 'react';
import { Grid, Typography, Paper } from '@mui/material';
import { Thermometer, Droplet, Sun, Gauge } from 'lucide-react';
import axios from 'axios';

export default function RealTime() {
  const [weatherData, setWeatherData] = useState(null);
  const [lastUpdated, setLastUpdated] = useState(null);

  useEffect(() => {
    fetchData();
    const interval = setInterval(fetchData, 5000); // Update every 5 seconds
    return () => clearInterval(interval);
  }, []);

  const fetchData = async () => {
    try {
      const response = await axios.get('http://localhost:5000/api/weather/latest');
      setWeatherData(response.data);
      setLastUpdated(new Date().toLocaleTimeString());
    } catch (error) {
      console.error('Error fetching data:', error);
    }
  };

  if (!weatherData) return <div>Loading...</div>;

  return (
    <Grid container spacing={3}>
      <Grid item xs={12}>
        <Typography variant="h4" gutterBottom>
          Real-Time Weather Data
        </Typography>
        <Typography variant="subtitle1" color="text.secondary">
          Last updated: {lastUpdated}
        </Typography>
      </Grid>

      <Grid item xs={12} sm={6} md={3}>
        <Paper sx={{ p: 3, display: 'flex', alignItems: 'center' }}>
          <Thermometer size={48} style={{ marginRight: 16 }} />
          <div>
            <Typography variant="h6">Temperature</Typography>
            <Typography variant="h4">{weatherData.temperature} °C</Typography>
          </div>
        </Paper>
      </Grid>

      <Grid item xs={12} sm={6} md={3}>
        <Paper sx={{ p: 3, display: 'flex', alignItems: 'center' }}>
          <Droplet size={48} style={{ marginRight: 16 }} />
          <div>
            <Typography variant="h6">Humidity</Typography>
            <Typography variant="h4">{weatherData.humidity} %</Typography>
          </div>
        </Paper>
      </Grid>

      <Grid item xs={12} sm={6} md={3}>
        <Paper sx={{ p: 3, display: 'flex', alignItems: 'center' }}>
          <Sun size={48} style={{ marginRight: 16 }} />
          <div>
            <Typography variant="h6">Light</Typography>
            <Typography variant="h4">{weatherData.light} lux</Typography>
          </div>
        </Paper>
      </Grid>

      <Grid item xs={12} sm={6} md={3}>
        <Paper sx={{ p: 3, display: 'flex', alignItems: 'center' }}>
          <Gauge size={48} style={{ marginRight: 16 }} />
          <div>
            <Typography variant="h6">Pressure</Typography>
            <Typography variant="h4">{weatherData.pressure || 'N/A'} hPa</Typography>
          </div>
        </Paper>
      </Grid>
    </Grid>
  );
}