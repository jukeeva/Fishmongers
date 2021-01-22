
var isRain = false;
var isWindy3 = false;
var isWindy8 = false;
var currentTemp = 0;

async function getData() {
    result = await fetch("https://elecdesign.org.aalto.fi/nr1/weatherjson")
        .then(response => response.json());
    
    return result;
}

function parseChartData(weatherData) {
    let timeSeries = { times: [], temps: [], rains: [] };
    for (let i=0; i<weatherData.length; i++) {
        let instant = { time: '', temperature: '', rain: '', windSpeed: '', windDirection: '' }
        // [3, 11, 2020, 23, 10.1, 0, 3.9, 246]
        let dateString = "";
        let month = '0';
        let day = '0';
        let hour = '0';
        let year = weatherData[i][2];
        
        if (weatherData[i][0]<10)  //if day is one digit, add 0 in front
            day = day + weatherData[i][0];
        else day = weatherData[i][0];
        if (weatherData[i][1]<10)   // month
            month = month + weatherData[i][1];
        else month = weatherData[i][1];
        if (weatherData[i][3]<10)   // hour
            hour = hour + weatherData[i][3];
        else hour = weatherData[i][3];
        dateString = day + '.' + month + '.' + year + ' ' + hour + ':00';
                    
        let temp = weatherData[i][4];
        let rain =  weatherData[i][5];
        let windSpeed =  weatherData[i][6];
        let windDirection =  weatherData[i][7];

        
        if (windSpeed > 2 && i <9) { // the 9 means that we are within 8 hours into the forecast data
            isWindy3 = true;  
        }
        if (windSpeed > 7 && i <9) { // the 9 means that we are within 8 hours into the forecast data
            isWindy3 = false;
            isWindy8 = true;
        }
        if (rain > 0 && i <9) { 
            isRain = true;
        }
        if (i==0) {
            if (temp > 0) {
                currentTemp = '+' + temp + '°C';
            } else {
                currentTemp = temp + '°C';
            }
        }

        timeSeries.times.push(dateString);
        timeSeries.temps.push(temp);
        timeSeries.rains.push(rain);
    }
    return timeSeries;
}

async function drawChart() {
    let weatherData = await getData();
    let chartData = parseChartData(weatherData);

    //console.log(chartData);

    var timeFormat = 'DD.MM.YYYY HH:mm';


    Chart.defaults.global.elements.point.radius = 0;

    var ctx = 'myChart';

    var myChart = new Chart(ctx, {
        type: 'bar',
        data: {
            labels: chartData.times,
            datasets: [{
                yAxisID: 'temp-axis',
                type: 'line',
                label: 'Temperatures',
                backgroundColor: 'rgba(180, 5, 5, 0.05)',
                borderColor: 'rgba(180, 5, 5, 0.8)',
                borderWidth: 2,
                fill: false,
                data: chartData.temps
            },{
                yAxisID: 'rain-axis',
                type: 'bar',
                label: 'Rain',
                backgroundColor: 'rgba(5, 5, 225, 0.05)',
                borderColor: 'rgba(5, 5, 225, 0.5)',
                borderWidth: 1,
                fill: true,
                data: chartData.rains
            }]
        },
        options: {
            responsive: false,
            legend: {
                display: false
            },
            scales: {
                xAxes: [{
                    type: 'time',
                    time: {
                        displayFormats: {
                            day: 'DD.MM.',
                            hour: 'HH:mm'
                        },
                        parser: timeFormat,
                        stepSize: 4,
                        minUnit: 'hour'
                    }
                },{
                    type: 'time',
                    major: {
                        display: true
                    },
                    gridLines: {
                        display: true
                    },
                    time: {
                        displayFormats: {
                            day: 'dddd',
                            hour: 'HH:mm'
                        },
                        parser: timeFormat,
                        stepSize: 1,
                        minUnit: 'day'
                    }
                }],
                yAxes: [{
                    scaleLabel: {
                        labelString: 'Temperature °C',
                        fontColor: 'rgba(180, 5, 5, 0.8)',
                        display: true
                    },
                    id: 'temp-axis',
                    type: 'linear'
                    /*ticks: {
                        min: -10,
                        max: 20
                    }*/
                },{
                    scaleLabel: {
                        labelString: 'Rain (mm)',
                        fontColor: 'rgba(5, 5, 225, 0.5)',
                        display: true
                    },
                    gridLines: {
                        display: false
                    },
                    id: 'rain-axis',
                    position: 'right',
                    type: 'linear',
                    ticks: { 
                        suggestedMin: 0,
                        suggestedMax: 2
                    }
                }]
            },
        }
    });

    drawSymbols();

}

function drawSymbols() {
    if (isWindy8 || isWindy3)
        document.getElementById('wind-container').style.display  = 'inline';
        document.getElementById('wind-icons-view').style.display  = 'inline';
    if (isWindy8) 
        document.getElementById('wind-symbol8').style.display  = 'inline';
    if (isWindy3) 
        document.getElementById('wind-symbol3').style.display  = 'inline';
    if (isRain)
        document.getElementById('rain-container').style.display  = 'inline';
        document.getElementById('rain-symbol').style.display  = 'inline';

    document.getElementById('temp-container').innerHTML = currentTemp;
}

drawChart();

