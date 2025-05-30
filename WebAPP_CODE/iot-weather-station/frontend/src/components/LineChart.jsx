import { Line } from 'react-chartjs-2';
import {
  Chart as ChartJS,
  CategoryScale,
  LinearScale,
  PointElement,
  LineElement,
  Title,
  Tooltip,
  Legend,
} from 'chart.js';

ChartJS.register(
  CategoryScale,
  LinearScale,
  PointElement,
  LineElement,
  Title,
  Tooltip,
  Legend
);

export default function LineChart({ data, title }) {
  const options = {
    responsive: true,
    plugins: {
      legend: {
        position: 'top',
        labels: {
          color: '#d0d0d0',
        }
      },
      title: {
        display: true,
        text: title,
        color: '#d0d0d0',
      },
    },
    scales: {
      y: {
        ticks: {
          color: '#d0d0d0',
        },
      },
      x: {
        ticks: {
          color: '#d0d0d0',
        },
      },
    },
  };

  return (
    <div className="weather-gauge-box">
      <div className="weather-gauge">
        <Line options={options} data={data} />
      </div>
    </div>
  );
}
