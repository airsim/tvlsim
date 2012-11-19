var bidpricechart;
var segmentName='Bid-Price Vector';


function buildBPV(chartData, legName, flightName) {
   bidpricechart = new Highcharts.Chart({
      chart: {
         renderTo: 'bidpricechartcontainer',
         defaultSeriesType: 'line',
         marginRight: 130,
         marginBottom: 25
      },
      credits: {
            enabled: false
        },
      title: {
         text: segmentName,
         x: -20 //center
      },
      subtitle: {
         text: legName,
         x: -20
      },
      xAxis: {
         title: {
            text: 'Seat Index'
         },
      },
      yAxis: {
         title: {
            text: 'Bid-Price'
         },
         plotLines: [{
            value: 0,
            width: 1,
            color: '#808080'
         }]
      },
      tooltip: {
         formatter: function() {
                   return '<b>'+ this.series.name +'</b><br/>'+
               this.x +': $'+ this.y;
         }
      },
      legend: {
         layout: 'vertical',
         align: 'right',
         verticalAlign: 'top',
         x: -10,
         y: 100,
         borderWidth: 0
      },
      series: [{
         name: flightName,
         data: chartData
      }]
   });
   
   
}
   
