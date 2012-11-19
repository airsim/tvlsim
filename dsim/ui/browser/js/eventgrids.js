Ext.require([
    'Ext.grid.*',
    'Ext.data.*',
    'Ext.util.*',
    'Ext.state.*'
]);

Ext.define('eventsinfo', {
	extend: 'Ext.data.Model',
	fields: [
	{name:'wtp', type: 'float'}, 
	{name:'cha', type:'string'}, 
	{name:'dep_date', type: 'shortdate'},
	{name:'event_type', type:'string'},
	{name:'dep_time', type:'string'}, 
	{name:'pax', type:'int'},
	{name:'time_stamp', type:'longdate'},
	{name:'des', type:'string'},
	{name:'pos', type:'string'},	
	{name:'adv_purchase', type:'int'},
	{name:'pref_carriers', type:'string'},
	{name:'cancel_date', type:'string'},
	{name:'bkg_date', type:'string'},
	{name:'cab', type:'string'},
	{name:'org', type:'string'},
	{name:'return_time', type:'string'},
	{name:'return_date', type:'string'},
	{name:'stay_duration', type:'string'}
	]	
});



Ext.QuickTips.init();
Ext.state.Manager.setProvider(Ext.create('Ext.state.CookieProvider'));

function refreshBookingEvents(serverURL)
{
	var url = serverURL +  "get/events/"
	$.getJSON(url + "callback=?", function(data) {	
	
	//console.log(data);
	/*
	var bkgRequest = [
						['20-Mar-11, 08:15:37','NYC','IEV','Business','3','NYC','In/Off', 2000,'20-Mar-11','60 days','20-Jun-11','30-Jun-11','10 days','25-Apr-11','8AM','10PM','BA, AA']
					 ];
	
	var solList = [
					  ['false', 'AF10/JFK-CDG/H, AF52/CDG-KBP/K', '20-Jun-11', 'AF53/KBP-CDG/K, AF11/CDG-JFK/H', '30-Jun-11', 800],
					  ['true', 'AA56/JFK-KBP/Y', '20-Jun-11', 'AA57/KBP-JFK/Y','30-Jun-11', 700],
					  ['false', 'SV5/JFK-KBP/Q', '20-Jun-11', 'SV6/KBP-JFK/Q','30-Jun-11', 800]
				  ];
	*/
	
	var store_events = Ext.create('Ext.data.Store', {
		model: 'eventsinfo',
		proxy: {
			type: 'memory',
			data: data,
			reader: {
				type: 'json',
				root: 'events'
			}, 
		}   
	});
	store_events.load();
	
	var bkgGrid = Ext.create('Ext.grid.Panel', {
		store: store_events,
		stateful: true,
		columns: [
			{'header': "Time-Stamp", 'width': 180, 'dataIndex': 'time_stamp'},
			{'header': "D. Date", 'width': 100, 'dataIndex': 'dep_date'},
			{'header': "R. Date", 'width': 100, 'dataIndex': 'return_date'},
			{'header': "Orig.", 'width': 40, 'dataIndex': 'org'},
			{'header': "Dest.", 'width': 70, 'dataIndex': 'des'},
			{'header': "Pref. Carrier", 'width': 50, 'dataIndex': 'pref_carriers'},
			{'header': "Channel", 'width': 50, 'dataIndex': 'cha'},
			{'header': "WTP", 'width': 80, 'dataIndex': 'wtp',  'renderer' : 'usMoney'},
			{'header': "Bkg Date", 'width': 100, 'dataIndex': 'bkg_date',},
			{'header': "Cancel?", 'width': 70, 'dataIndex': 'cancel_date'},
			{'header': "Adv. Purchase", 'width': 80, 'dataIndex': 'adv_purchase'},			
			{'header': "Stay D.", 'width': 70, 'dataIndex': 'stay_duration'},
			{'header': "D. Time", 'width': 100, 'dataIndex': 'dep_time'},
			{'header': "R. Time", 'width': 100, 'dataIndex': 'return_time'},			
		],
		renderTo:'bkgEvents',
		
		width:1193,
		height:400,
		viewConfig: {
			stripeRows: true
		}
		});
		
		bkgGrid.getSelectionModel().on('selectionchange', function(sm, selectedRecord) {
		
		if(selectedRecord[0] != undefined)
		{
			showTravelSolutions();
		}
	});
	});

}

