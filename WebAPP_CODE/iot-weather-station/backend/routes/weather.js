const express = require('express');
const router = express.Router();
const WeatherData = require('../models/WeatherData');

// Add new weather data
router.post('/', async (req, res) => {
  try {
    const { temperature, humidity, light, pressure } = req.body;
    const newData = new WeatherData({ temperature, humidity, light, pressure });
    await newData.save();
    res.status(201).json(newData);
  } catch (err) {
    res.status(400).json({ message: err.message });
  }
});

// Get all weather data
router.get('/', async (req, res) => {
  try {
    const data = await WeatherData.find().sort({ timestamp: -1 });
    res.json(data);
  } catch (err) {
    res.status(500).json({ message: err.message });
  }
});

// Get latest data
router.get('/latest', async (req, res) => {
  try {
    const data = await WeatherData.findOne().sort({ timestamp: -1 });
    res.json(data);
  } catch (err) {
    res.status(500).json({ message: err.message });
  }
});

module.exports = router;