var chart;
var segmentName='Bid-Price Vector';
$(document).ready(function() {
   chart = new Highcharts.Chart({
      chart: {
         renderTo: 'chart',
         defaultSeriesType: 'line',
         marginRight: 130,
         marginBottom: 25
      },
      title: {
         text: segmentName,
         x: -20 //center
      },
      /*subtitle: {
         text: 'Source: WorldClimate.com',
         x: -20
      },*/
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
         name: 'SV 5',
         data: [3000,2871, 2672, 2546, 2452, 2361, 2292, 2235, 2192, 2153, 2112, 2070, 2027, 1983]
      }]
   });
   
   
});
   