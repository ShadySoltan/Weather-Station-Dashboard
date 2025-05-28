require('dotenv').config();
const express = require('express');
const mongoose = require('mongoose');
const cors = require('cors');
const weatherRoutes = require('./routes/weather');

const app = express();

// Middleware
app.use(cors());
app.use(express.json()); // Built-in body parser

// MongoDB connection
const DB_URI = "mongodb+srv://shadyalysoltan:pSvwXWsQ09W0UvpM@mongodbcluster.eyxjqdo.mongodb.net/weather-station";

mongoose.connect(DB_URI, {
  useNewUrlParser: true,
  useUnifiedTopology: true,
})
.then(() => {
  console.log('Connected to MongoDB');
})
.catch(err => {
  console.error('MongoDB connection error:', err);
  process.exit(1);
});

const db = mongoose.connection;

// Make db available to routes
app.locals.db = db;

// Routes
app.use('/api/weather', weatherRoutes);

// Health check endpoint
app.get('/health', (req, res) => {
    if (mongoose.connection.readyState === 1) {
        res.json({ status: 'OK', database: 'Connected' });
    } else {
        res.status(500).json({ status: 'Error', database: 'Disconnected' });
    }
});

const PORT = process.env.PORT || 5000;
app.listen(PORT, () => console.log(`Server running on port ${PORT}`));

// Graceful shutdown
process.on('SIGINT', async () => {
  console.log('Shutting down gracefully...');
  await mongoose.connection.close();
  process.exit(0);
});
