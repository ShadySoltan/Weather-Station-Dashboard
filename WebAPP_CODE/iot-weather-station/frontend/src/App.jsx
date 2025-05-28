import { BrowserRouter as Router, Routes, Route } from 'react-router-dom';
import { Container } from '@mui/material';
import Navbar from './components/Navbar';
import Dashboard from './pages/Dashboard';
import RealTime from './pages/RealTime';
import Historical from './pages/Historical';

function App() {
  return (
    <Router>
      <Navbar />
      <Container maxWidth="xl" sx={{ mt: 4 }}>
        <Routes>
          <Route path="/" element={<Dashboard />} />
          <Route path="/realtime" element={<RealTime />} />
          <Route path="/historical" element={<Historical />} />
        </Routes>
      </Container>
    </Router>
  );
}

export default App;