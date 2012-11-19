var tl;

jQuery(document).ready(function(){

   var eventSource = new Timeline.DefaultEventSource();
   
   jQuery("#frise div.event").each(function (){
   var instant = 'true'
   if (jQuery(this).attr("end")){
      instant = 'false';
     }
   if ( instant == 'true' ) {
   var evt = new Timeline.DefaultEventSource.Event( {
                       start: new Date(jQuery(this).attr("start")),
                     instant: instant,
                        text: jQuery(this).attr("title"),
                 description: jQuery(this).html()
            });      
   }
   else {
   var evt = new Timeline.DefaultEventSource.Event( {
                       start: new Date(jQuery(this).attr("start")),
                         end: new Date(jQuery(this).attr("end")),
                 isDuration : 'true',
                        text: jQuery(this).attr("title"),
                 description: jQuery(this).html()
            });
      }
      eventSource.add(evt);		   	
   });

  nb_bandeaux=jQuery("#frise div.bandeau").size();
  var bandInfos = new Array(nb_bandeaux);
  var index_bandeau = 0;
  jQuery("#frise div.bandeau").each(function (){
  var iunit;
  switch (jQuery(this).attr("intervalUnit")) {
  case "DAY":
    iunit = Timeline.DateTime.DAY;
    break;
  case "YEAR":
    iunit = Timeline.DateTime.YEAR;
    break;
  case "MONTH":
    iunit = Timeline.DateTime.MONTH;
    break;
  default:
    iunit = Timeline.DateTime.DAY;
  }

  var overview = false;
  if (jQuery(this).attr("overview") == "true"){
    overview = true;}

  bandInfos[index_bandeau]=Timeline.createBandInfo({
        overview:       overview,
        width:          jQuery(this).attr("width"),
        intervalUnit:   iunit,
        intervalPixels: jQuery(this).attr("intervalPixels"),
        eventSource: eventSource
    })
  index_bandeau ++;
  });

  if (nb_bandeaux > 1){
    for(var idx=1 ;idx < nb_bandeaux ; idx++) {
    bandInfos[idx].syncWith = idx - 1;
    bandInfos[idx].highlight = true;
    }
  }

  tl = Timeline.create(document.getElementById("my-timeline"), bandInfos);



});

var resizeTimerID = null;
function onResize() {
    if (resizeTimerID == null) {
        resizeTimerID = window.setTimeout(function() {
            resizeTimerID = null;
            tl.layout();
        }, 500);
    }
}


 
