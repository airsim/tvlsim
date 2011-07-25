var infoFlight;
var infoLegs;
var infoSubclasses;
var infoBPV;

function processRequest()
{
	$.getJSON("D:/Development%20Workspace/Dreamweaver/DsimUI/browser/sample/datasample.html",
	  {
		tags: "flight_date",
		tagmode: "any",
		format: "json"
	  },
	  function(data) {
		 infoFlight = data.flight_date;
		 infoLegs = data.flight_date.legs;
		 infoSubclasses = data.flight_date.subclasses;
		 infoBPV = data.flight_date.bid_price_vectors;
		 
		 
	  });
}