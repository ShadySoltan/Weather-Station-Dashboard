const mongoose = require('mongoose');

const weatherDataSchema = new mongoose.Schema({
  temperature: { type: Number, required: true },
  humidity: { type: Number, required: true },
  light: { type: Number, required: true },
  pressure: { type: Number },
  timestamp: { type: Date, default: Date.now }
});

module.exports = mongoose.model('WeatherData', weatherDataSchema);