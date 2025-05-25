require('dotenv').config();
const express = require('express');
const { MongoClient } = require('mongodb');
const cors = require('cors');
const weatherRoutes = require('./routes/weather');

const app = express();

// Middleware
app.use(cors());
app.use(express.json()); // Built-in body parser

// MongoDB connection
let db;
const client = new MongoClient("mongodb+srv://shadyalysoltan:pSvwXWsQ09W0UvpM@mongodbcluster.eyxjqdo.mongodb.net/");

async function connectToDatabase() {
  try {
    await client.connect();
    db = client.db('weather-station');
    console.log('Connected to MongoDB');

    // Make db available to routes
    app.locals.db = db;
    
    // Routes
    app.use('/api/weather', weatherRoutes);
    
    // Health check endpoint
    app.get('/health', (req, res) => {
      res.json({ status: 'OK', database: 'Connected' });
    });
    
    const PORT = process.env.PORT || 5000;
    app.listen(PORT, () => console.log(`Server running on port ${PORT}`));
    
  } catch (err) {
    console.error('MongoDB connection error:', err);
    process.exit(1);
  }
}

// Graceful shutdown
process.on('SIGINT', async () => {
  console.log('Shutting down gracefully...');
  await client.close();
  process.exit(0);
});

connectToDatabase();
