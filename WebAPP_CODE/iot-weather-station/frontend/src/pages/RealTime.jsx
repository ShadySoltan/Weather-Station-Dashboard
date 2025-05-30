import { useEffect, useState } from 'react';
import { Grid, Typography } from '@mui/material';
import { Thermometer, Droplet, Sun, Gauge } from 'lucide-react';
import axios from 'axios';
import DashboardCard from '../components/DashboardCard';

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
        <DashboardCard
          title="Temperature"
          value={weatherData.temperature}
          unit="°C"
          icon={<Thermometer size={48} />}
        />
      </Grid>

      <Grid item xs={12} sm={6} md={3}>
        <DashboardCard
          title="Humidity"
          value={weatherData.humidity}
          unit="%"
          icon={<Droplet size={48} />}
        />
      </Grid>

      <Grid item xs={12} sm={6} md={3}>
        <DashboardCard
          title="Light"
          value={weatherData.light}
          unit="lux"
          icon={<Sun size={48} />}
        />
      </Grid>

      <Grid item xs={12} sm={6} md={3}>
        <DashboardCard
          title="Pressure"
          value={weatherData.pressure || 'N/A'}
          unit="hPa"
          icon={<Gauge size={48} />}
        />
      </Grid>
    </Grid>
  );
}
