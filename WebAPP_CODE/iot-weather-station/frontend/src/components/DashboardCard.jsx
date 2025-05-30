import { Card, CardContent, Typography, Box } from '@mui/material';

export default function DashboardCard({ title, value, unit, icon, color }) {
  const imageName = title.toLowerCase() + '.jpg';
  const imageUrl = `url('./Photo/${imageName}')`;

  return (
    <Card sx={{ minWidth: 275, height: '100%', backgroundImage: imageUrl, backgroundSize: 'cover', backgroundRepeat: 'no-repeat', color: '#d0d0d0', borderRadius: '25px', border: '1px solid #3e424e',
    transition: 'transform 0.2s, box-shadow 0.2s, background-color 0.3s ease',
    '&:hover': {
      transform: 'scale(1.05)',
      boxShadow: '0 0 20px gold',
      backgroundColor: 'rgba(173, 216, 230, 0.3)',
    } }}>
      <CardContent>
        <Typography variant="h5" component="div" gutterBottom style={{ 
          background: 'linear-gradient(to right, yellow, gold)',
          WebkitBackgroundClip: 'text',
          WebkitTextFillColor: 'transparent',
          transition: 'text-shadow 0.3s ease',
         '&:hover': {
          textShadow: '0 0 5px gold'
         }
         }}>
          {title}
        </Typography>
        <Box display="flex" alignItems="center" justifyContent="space-between">
          <Typography variant="h3" style={{ 
            background: 'linear-gradient(to right, yellow, gold)',
            WebkitBackgroundClip: 'text',
            WebkitTextFillColor: 'transparent',
            transition: 'text-shadow 0.3s ease',
            '&:hover': {
              textShadow: '0 0 5px gold'
             }
           }}>
            {value} {unit}
          </Typography>
          {icon}
        </Box>
      </CardContent>
    </Card>
  );
}
