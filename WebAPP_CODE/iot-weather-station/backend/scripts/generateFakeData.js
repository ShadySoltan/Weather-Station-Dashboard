const axios = require('E:/Projects/Weather-Station-Dashboard/WebAPP_CODE/iot-weather-station/backend/node_modules/axios');

const generateFakeData = async () => {
  try {
    const temperature = Math.random() * 30 + 10; // Temperature between 10 and 40
    const humidity = Math.random() * 50 + 50; // Humidity between 50 and 100
    const light = Math.random() * 1000; // Light intensity between 0 and 1000
    const pressure = Math.random() * 10 + 1000; // Pressure between 1000 and 1010

    const data = {
      temperature,
      humidity,
      light,
      pressure,
    };

    const response = await axios.post('http://localhost:5000/api/weather', data);
    console.log('Fake data posted:', response.data);
  } catch (error) {
    console.error('Error posting fake data:', error);
  }
};

// Generate and post fake data every 5 seconds
setInterval(generateFakeData, 5000);
