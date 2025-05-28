import { Card, CardContent, Typography, Box } from '@mui/material';

export default function DashboardCard({ title, value, unit, icon, color }) {
  return (
    <Card sx={{ minWidth: 275, height: '100%', backgroundColor: color, borderRadius: '25px', border: '1px solid white' }}>
      <CardContent>
        <Typography variant="h5" component="div" gutterBottom>
          {title}
        </Typography>
        <Box display="flex" alignItems="center" justifyContent="space-between">
          <Typography variant="h3">
            {value} {unit}
          </Typography>
          {icon}
        </Box>
      </CardContent>
    </Card>
  );
}
