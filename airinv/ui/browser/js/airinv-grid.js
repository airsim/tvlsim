Ext.require([
    'Ext.grid.*',
    'Ext.data.*',
    'Ext.util.*',
    'Ext.state.*'
]);

Ext.define('flightinfo', {
    extend: 'Ext.data.Model',
    fields: [
	{
        name: 'departure_date',
        type: 'shortdate'
    }, 
	{
		name:'airline_code',
		type:'string'
	}, 
	{ 
		name:'flight_number',
		type:'string'
	},
	],
	
	hasMany: {model: 'legsinfo', name: 'legs'}
});


Ext.define('legsinfo', {
    extend: 'Ext.data.Model',
    fields: [
	{
        name: 'board_date',
        type: 'shortdate'
    }, 
	{
		name:'capacity',
		type:'int'
	}, 
	{ 
		name:'off_date',
		type:'shortdate'
	},
	{
		name:'distance',
		type:'int'
	},
	{
		name:'off_point',
		type:'string'
	},
	{
		name:'board_time',
		type:'string'
	},
	{
		name:'elapsed_time',
		type:'string'
	},
	{
		name:'date_offset',
		type:'string'
	},
	{
		name:'off_time',
		type:'string'
	},
	{
		name:'board_point',
		type:'string'
	},
	{
		name:'time_offset',
		type:'string'
	}
	],
	
	belongsTo: 'flightinfo'
});

var flightURL;
function queryBuild(companyCode, flightNumber, date)
{
	var queryURL="http://ncevsediri-fed/api/display/inv/" + companyCode + "/" + flightNumber + "/" + date;
	//flightURL = queryURL;
	onFlightCall(queryURL);
}
 

function onFlightCall(url){
    var store_flights = Ext.create('Ext.data.Store', {
        autoLoad: true,
        autoSync: true,
        model: 'flightinfo',
        proxy: {
            type: 'rest',
            url: url,
            reader: {
                type: 'json',
                root: 'flight_date'
            }, 
        },    
    });

    // create the grid
    var grid_flight = new Ext.grid.GridPanel({
        store: store_flights,
        columns: [
            {header: "Departure Date", width: 200, dataIndex: 'departure_date', sortable: true},
            {header: "Airline Code", width: 100, dataIndex: 'airline_code', sortable: true},
            {header: "Flight Number", width: 100, dataIndex: 'flight_number', sortable: true},
            //{header: "", width: 100, dataIndex: 'legs', sortable: false, renderer: renderSelect},
            
        ],
        title: 'Found Flights',
		renderTo:'flights-GridDisplay',
        width:400,
        height:300,
		viewConfig: {
			stripeRows:true
		},
		listeners: {
			itemclick : function() {
				var data=grid_flight.getSelectionModel().selected.items[0].data;
				grid_legs.setTitle('Legs List');
				store_legs.clearFilter();
				//store_legs.filter('company_id', data.id);
				store_legs.load();
			}
		}	
    });
	store_flights.load();
	
	var store_legs = Ext.create('Ext.data.Store', {
        autoLoad: true,
        autoSync: true,
        model: 'legsinfo',
        proxy: {
            type: 'rest',
            url: url,
            reader: {
                type: 'json',
                root: 'flight_date.legs'
            }, 
        },    
    });

    // create the grid
    var grid_legs = new Ext.grid.GridPanel({
        store: store_legs,
        columns: [
            {header: "Board Date", width: 100, dataIndex: 'board_date', sortable: true},
            {header: "Capacity", width: 100, dataIndex: 'capacity', sortable: true},
            {header: "Off Date", width: 100, dataIndex: 'off_date', sortable: true},
            {header: "Distance", width: 100, dataIndex: 'distance', sortable: true},
            {header: "Off Point", width: 100, dataIndex: 'off_point', sortable: true},
			{header: "Board Time", width: 100, dataIndex: 'board_time', sortable: true},
			{header: "Elapsed Time", width: 100, dataIndex: 'elapsed_time', sortable: true},
			{header: "Date Offset", width: 100, dataIndex: 'date_offset', sortable: true},
			{header: "Off Time", width: 100, dataIndex: 'off_time', sortable: true},
			{header: "Board Point", width: 100, dataIndex: 'board_point', sortable: true},
			{header: "Time Offset", width: 100, dataIndex: 'time_offset', sortable: true},
			
        ],
        title: 'Flight Legs',
		renderTo:'legs-GridDisplay',
        width:1100,
        height:300,
		viewConfig: {
			stripeRows: true
		}
    });
}

/*var IMG_GO = "assets/icons/iconDeparture.png";
function renderSelect(legsArray) {
		//return Ext.String.format(
            return '<img src=' + IMG_GO + ' /><input type="button" value="Display Legs" id="bt"onClick="showLegs()"/>'
        //<a href="http://www.espn.com" target="_blank">);
		//<input type="button" value="toto" id="'+val+'"/>
    }*/