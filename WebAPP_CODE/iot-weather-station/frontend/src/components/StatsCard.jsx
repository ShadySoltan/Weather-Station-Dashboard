import { Card, CardContent, Typography, Box } from '@mui/material';
import './StatsCard.css';

export default function StatsCard({ title, value, unit, icon }) {
  return (
    <Card className="stats-card" style={{ backgroundColor: 'rgba(20, 23, 33, 0.7)', borderRadius: '15px', border: '1px solid rgba(62, 66, 78, 0.5)',
    backgroundImage: 'linear-gradient(to bottom, rgba(20, 23, 33, 0.7), rgba(10, 14, 23, 0.7))',
    transition: 'transform 0.2s, box-shadow 0.2s',
    '&:hover': {
      transform: 'scale(1.05)',
      boxShadow: '0 0 10px rgba(173, 216, 230, 0.5)',
    } }}>
      <CardContent className="stats-card-content">
        <Typography variant="h5" component="div" gutterBottom className="stats-card-title">
          {title}
        </Typography>
        <Box display="flex" alignItems="center" justifyContent="space-between">
          <Typography variant="h3" className="stats-card-value">
            {value} {unit}
          </Typography>
          {icon}
        </Box>
      </CardContent>
    </Card>
  );
}
