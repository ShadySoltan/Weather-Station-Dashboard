import { useEffect, useState } from 'react';
import { Grid } from '@mui/material';
import DashboardCard from '../components/DashboardCard';
import LineChart from '../components/LineChart';
import { Thermometer, Droplet, Sun, Gauge } from 'lucide-react';
import axios from 'axios';

export default function Dashboard() {
  const [weatherData, setWeatherData] = useState(null);
  const [chartData, setChartData] = useState(null);

  useEffect(() => {
    fetchLatestData();
    fetchHistoricalData();
    const interval = setInterval(fetchLatestData, 60000); // Update every minute
    return () => clearInterval(interval);
  }, []);

  const fetchLatestData = async () => {
    try {
      const response = await axios.get('http://localhost:5000/api/weather/latest');
      setWeatherData(response.data);
    } catch (error) {
      console.error('Error fetching data:', error);
    }
  };

  const fetchHistoricalData = async () => {
    try {
      const response = await axios.get('http://localhost:5000/api/weather');
      const data = response.data.slice(0, 24).reverse(); // Last 24 readings
      
      setChartData({
        labels: data.map(item => new Date(item.timestamp).toLocaleTimeString()),
        datasets: [
          {
            label: 'Temperature (°C)',
            data: data.map(item => item.temperature),
            borderColor: 'rgb(255, 99, 132)',
            backgroundColor: 'rgba(255, 99, 132, 0.5)',
          },
          {
            label: 'Humidity (%)',
            data: data.map(item => item.humidity),
            borderColor: 'rgb(53, 162, 235)',
            backgroundColor: 'rgba(53, 162, 235, 0.5)',
          },
        ],
      });
    } catch (error) {
      console.error('Error fetching historical data:', error);
    }
  };

  if (!weatherData) return <div>Loading...</div>;

  return (
    <Grid container spacing={3}>
      {/* Current Weather Cards */}
      <Grid item xs={12} sm={6} md={3}>
        <DashboardCard
          title="Temperature"
          value={weatherData.temperature}
          unit="°C"
          icon={<Thermometer size={48} />}
          color="rgba(255, 99, 132, 0.2)"
        />
      </Grid>
      <Grid item xs={12} sm={6} md={3}>
        <DashboardCard
          title="Humidity"
          value={weatherData.humidity}
          unit="%"
          icon={<Droplet size={48} />}
          color="rgba(54, 162, 235, 0.2)"
        />
      </Grid>
      <Grid item xs={12} sm={6} md={3}>
        <DashboardCard
          title="Light"
          value={weatherData.light}
          unit="lux"
          icon={<Sun size={48} />}
          color="rgba(255, 206, 86, 0.2)"
        />
      </Grid>
      <Grid item xs={12} sm={6} md={3}>
        <DashboardCard
          title="Pressure"
          value={weatherData.pressure || 'N/A'}
          unit="hPa"
          icon={<Gauge size={48} />}
          color="rgba(75, 192, 192, 0.2)"
        />
      </Grid>

      {/* Charts */}
      <Grid item xs={12} md={6}>
        {chartData && <LineChart data={chartData} title="Temperature & Humidity (Last 24 readings)" />}
      </Grid>
      <Grid item xs={12} md={6}>
        {/* Additional chart can be added here */}
      </Grid>
    </Grid>
  );
}