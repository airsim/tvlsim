var defaultSelection="";
var airlines=new String("SV; AF").split(";");
  
$(document).ready(function() {    
$("input#tbCompanyCode").autocomplete({
	delay:1,
	source: function(request,callback){
		 var req = request.term.trim().toUpperCase();				 
		 filteredList=[];	
		 if(req.length<3) { defaultSelection=""; return;}
		 var alreadyFound=false;	
		 for(var i=0;i<airlines.length;i++)
		 {
			currentAirline=airlines[i].toUpperCase();
			l=currentAirline.length;
			index=currentAirline.indexOf(req);
			underLined=airlines[i];
			if(index!=-1)
			{		
				if(currentAirline.substr(l-4,3)==req) index=l-4;						
				underLined=underLined.substr(0,index)+"<u>"+underLined.substr(index,req.length)+"</u>"+underLined.substr(index+req.length);						
			}
							
			if(req.length==2 && currentAirline.substr(l-4,3)==req)
			{				
				var a=new Array({"label":"<font size=2>"+underLined+"</font>","value":airlines[i]});	
				filteredList=a.concat(filteredList);
			}				
			else if(req.length>=2 && filteredList.length<5)//at least 2 chars required to start matching, do not display more than 5 res ??? DOES NOT WORK ???						
			{					
				
				if(index==0  && !alreadyFound)
				{					
					var a=new Array({"label":"<font size=2>"+underLined+"</font>","value":airlines[i]});	
					filteredList=a.concat(filteredList);						
					alreadyFound=true;
				}	
				else if(index!=-1)
				{							
					filteredList.push({"label":"<font size=2>"+underLined+"</font>","value":airlines[i]});											
				}
			}								
		 }	
		 defaultSelection=filteredList[0].value;
		 callback(filteredList);
	},
	select: function( event, ui ){				
		document.mainForm.airline.value=ui.item.value;
		defaultSelection=ui.item.value;					
	},
	close: function( event, ui ){			
	}		
}).data("autocomplete" )._renderItem = function( ul, item ) {
		return $( "<li></li>" ).data( "item.autocomplete", item )
							   .append( "<a>" + item.label + "</a>" )
							   .appendTo( ul );}

	
});		  	

function setDefaultSelection(){	
	document.mainForm.airline.value=defaultSelection;				
	jQuery("input#autocomplete").autocomplete("close");
	jQuery("input#autocomplete").autocomplete("enable");
}

function enter_pressed(e){
	var keycode;
	if (window.event) keycode = window.event.keyCode;
	else if (e) keycode = e.which;
	else return false;
	return (keycode == 13);
}
