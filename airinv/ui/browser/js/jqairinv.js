jQuery(document).ready(	
	function() 
	{
		jQuery("#flightlist").jqGrid({
   		url:'sample/datasample.html',
		//url:'http://ncevsediri-fed/api/display/inv/SV/5/2010-Mar-11',

		jsonReader : {root:"flight_date"},
		datatype: "json",
   		colNames:['Flight Date','Airline Code', 'Flight Number'],
   		colModel:[
   			{name:'departure_date',index:'departure_date', width:55},
   			{name:'airline_code',index:'airline_code', width:90},
   			{name:'flight_number',index:'flight_number', width:100},
   		],
   		rowNum:10,
   		rowList:[10,20,30],
   		pager: '#flightpager',
    	viewrecords: true,
    	caption:"Flight Selection"
	});
		
	jQuery("#flightlist").jqGrid('navGrid','#flightpager',{edit:false,add:false,del:false});
});