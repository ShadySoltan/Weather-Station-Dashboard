import { useEffect, useState } from 'react';
import { Grid, Typography, Select, MenuItem, FormControl, InputLabel } from '@mui/material';
import LineChart from '../components/LineChart';
import axios from 'axios';

export default function Historical() {
  const [timeRange, setTimeRange] = useState('24h');
  const [chartData, setChartData] = useState(null);

  useEffect(() => {
    fetchData();
  }, [timeRange]);

  const fetchData = async () => {
    try {
      const response = await axios.get('http://localhost:5000/api/weather');
      let filteredData = [];
      
      if (timeRange === '24h') {
        filteredData = response.data.slice(0, 24).reverse();
      } else if (timeRange === '7d') {
        // For demo, we'll just take 7 points - in real app you'd filter by date
        filteredData = response.data.filter((_, index) => index % 10 === 0).slice(0, 7).reverse();
      }

      setChartData({
        labels: filteredData.map(item => 
          timeRange === '24h' 
            ? new Date(item.timestamp).toLocaleTimeString() 
            : new Date(item.timestamp).toLocaleDateString()
        ),
        datasets: [
          {
            label: 'Temperature (°C)',
            data: filteredData.map(item => item.temperature),
            borderColor: 'rgb(255, 99, 132)',
            backgroundColor: 'rgba(255, 99, 132, 0.5)',
          },
          {
            label: 'Humidity (%)',
            data: filteredData.map(item => item.humidity),
            borderColor: 'rgb(53, 162, 235)',
            backgroundColor: 'rgba(53, 162, 235, 0.5)',
          },
          {
            label: 'Light (lux)',
            data: filteredData.map(item => item.light),
            borderColor: 'rgb(255, 206, 86)',
            backgroundColor: 'rgba(255, 206, 86, 0.5)',
          },
        ],
      });
    } catch (error) {
      console.error('Error fetching historical data:', error);
    }
  };

  return (
    <Grid container spacing={3}>
      <Grid item xs={12}>
        <Typography variant="h4" gutterBottom>
          Historical Weather Data
        </Typography>
        <FormControl sx={{ minWidth: 120 }} size="small">
          <InputLabel>Time Range</InputLabel>
          <Select
            value={timeRange}
            label="Time Range"
            onChange={(e) => setTimeRange(e.target.value)}
          >
            <MenuItem value="24h">Last 24 Hours</MenuItem>
            <MenuItem value="7d">Last 7 Days</MenuItem>
          </Select>
        </FormControl>
      </Grid>

      <Grid item xs={12}>
        {chartData && <LineChart data={chartData} title={`Weather Data (Last ${timeRange === '24h' ? '24 Hours' : '7 Days'})`} />}
      </Grid>
    </Grid>
  );
}