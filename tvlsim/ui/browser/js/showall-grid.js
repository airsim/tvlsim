Ext.require([
    'Ext.grid.*',
    'Ext.data.*',
    'Ext.util.*',
    'Ext.state.*'
]);

Ext.define('airlineinfo', {
	extend: 'Ext.data.Model',
	fields: [
	{name:'airline_code', type: 'string'}, 
	{name:'date', type:'string'}, 
	{name:'number', type: 'int'},
	]	
});



Ext.QuickTips.init();
Ext.state.Manager.setProvider(Ext.create('Ext.state.CookieProvider'));

function showAllGrid(serverURL)
{
	var url = serverURL +  "api/get/getairlinelist/"
	$.getJSON(url + "callback=?", function(data) {
	
	//console.log(data);
	
	var selected_AirlineCode;
	var selected_Number;
	var selected_Date;
	
	var store_airlines = Ext.create('Ext.data.Store', {
		model: 'airlineinfo',
		proxy: {
			type: 'memory',
			data: data,
			reader: {
				type: 'json',
				root: 'inventories'
			}, 
		}   
	});
	store_airlines.load();
	
	//console.log(store_airlines);
	
	var grid_AllInventory = Ext.create('Ext.grid.Panel', {
		store: store_airlines,
		stateful: true,
		columns: [
			{'header': "Airline Code", 'width': 70, 'dataIndex': 'airline_code'},
			{'header': "Flight Number", 'width': 80, 'dataIndex': 'number'},
			{'header': "Date", 'width': 80, 'dataIndex': 'date'},		
		],
		renderTo:'allinvgrid',
		
		width:250,
		height:400,
		viewConfig: {
			stripeRows: true
		}
		});
	
	grid_AllInventory.getSelectionModel().on('selectionchange', function(sm, selectedRecord) {
		
		if(selectedRecord[0] != undefined)
		{
			selected_AirlineCode = selectedRecord[0].data.airline_code;	
			selected_Number = selectedRecord[0].data.number;	
			selected_Date = selectedRecord[0].data.date;	
		
			document.getElementById('tbCompanyCode').value = selected_AirlineCode;
			document.getElementById('tbFlightNumber').value = selected_Number;
			document.getElementById('datepicker').value = selected_Date;
		}
	});
	});
}

