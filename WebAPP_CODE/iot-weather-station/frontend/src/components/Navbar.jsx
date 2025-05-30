import { AppBar, Toolbar, Typography, Button, Box } from '@mui/material';
import { Link } from 'react-router-dom';
import './Navbar.css';

export default function Navbar() {
  return (
    <AppBar position="static" className="navbar">
      <Toolbar>
        <Typography variant="h6" component="div" sx={{ flexGrow: 1, color: '#d0d0d0', fontFamily: 'Roboto', fontWeight: 700, textShadow: '2px 2px 4px rgba(0, 0, 0, 0.5)' }} className="navbar-title">
          IoT Weather Station
        </Typography>
        <Box sx={{ display: 'flex', gap: 2 }}>
          <Button color="inherit" component={Link} to="/" className="navbar-link">Dashboard</Button>
          <Button color="inherit" component={Link} to="/realtime" className="navbar-link">Real-Time</Button>
          <Button color="inherit" component={Link} to="/historical" className="navbar-link">Historical</Button>
        </Box>
      </Toolbar>
      <div className="raindrops"></div>
    </AppBar>
  );
}
