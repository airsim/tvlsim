Ext.require([
    'Ext.grid.*',
    'Ext.data.*',
    'Ext.util.*',
    'Ext.state.*'
]);

Ext.define('flightinfo', {
	extend: 'Ext.data.Model',
	fields: ['departure_date','airline_code','flight_number'],
	hasMany: {model: 'segmentsinfo', name: 'segments'},
	hasMany: {model: 'legsinfo', name: 'legs'}
});

Ext.define('legsinfo', {
	extend: 'Ext.data.Model',
	fields: [
	{name: 'distance', type: 'string'}, 
	{name:'capacity', type:'int'}, 
	{name: 'board_date', type: 'shortdate'},
	{name:'off_point', type:'string'},
	{name:'board_time', type:'string'}, 
	{name:'elapsed_time', type:'string'},
	{name:'date_offset', type:'string'},
	{name:'off_time', type:'string'},
	{name:'off_dDate', type:'shortdate'},	
	{name:'board_point', type:'string'},
	{name:'time_offset', type:'string'}
	],
	hasMany: {model: 'cabinsinfo', name: 'cabins'},
	belongsTo: 'flightinfo'
	
});

Ext.define('cabinsinfo', {
	extend: 'Ext.data.Model',
	fields: [
	{name: 'avl', type: 'int'},
	{name: 'code', type: 'string'}, 
	{name: 'offed_cap', type: 'int'}, 
	{name: 'ss', type: 'int'}, 
	{name: 'gav', type: 'float'},
	{name: 'phy_cap', type: 'int'},
	{name: 'etb', type: 'int'},
	{name: 'acp', type: 'int'},
	{name: 'au', type: 'float'},
	{name: 'comm_space', type: 'float'},
	{name: 'rgd_adj', type: 'int'},
	{name: 'wl', type: 'int'},
	{name: 'group', type: 'int'},
	{name: 'bid_price', type: 'float'},
	{name: 'nav', type: 'float'},
	{name: 'upr', type: 'float'},
	{name: 'av_pool', type: 'int'},
	{name: 'staff', type: 'int'}
	],
	
	belongsTo: 'legsinfo',
	hasMany: {model: 'bucketsinfo', name: 'buckets'}
});

Ext.define('bucketsinfo', {
	extend: 'Ext.data.Model',
	fields: [
	{name: 'ss', type: 'int'},
	{name: 'si', type: 'string'}, 
	{name: 'yield', type: 'int'}, 
	{name: 'av', type: 'float'}	
	],
	
	belongsTo: 'cabinsinfo'
});

Ext.define('segmentsinfo', {
	extend: 'Ext.data.Model',
	fields: ['segment'],
	
	hasMany: {model: 'subclassesinfo', name: 'sub_classes'},
	belongsTo: 'flightinfo'
});

Ext.define('subclassesinfo', {
	extend: 'Ext.data.Model',
	fields: [
	{name: 'grp_bks_pdg', type: 'int', //mapping:'sub_classes.grp_bks_pdg'
	}, 
	{name:'ob%', type:'float', //mapping:'sub_classes.ob%'
	}, 
	{name:'seg_avl', type:'int', //mapping:'sub_classes.seg_avl'
	},
	{name:'cabin_code', type:'string', //mapping:'sub_classes.cabin_code'
	},
	{name:'class_avl', type:'int', //mapping:'sub_classes.class_avl'
	},
	{name:'ns%', type:'int', //mapping:'sub_classes.ns%'
	},
	{name:'stf_bkgs', type:'int', //mapping:'sub_classes.stf_bkgs'
	},
	{name:'etb', type:'int', //mapping:'sub_classes.etb'
	},
	{name:'rev_avl', type:'int', //mapping:'sub_classes.rev_avl'
	},
	{name:'min_au_prot', type:'string', //mapping:'sub_classes.min_au_prot'
	},
	{name:'nego', type:'int', //mapping:'sub_classes.nego'
	},
	{name:'class_code',	type:'string', //mapping:'sub_classes.class_code'
	},
	{name:'bkgs', type:'int', //mapping:'sub_classes.bkgs'
	},
	{name:'wl_bkgs', type:'int', //mapping:'sub_classes.wl_bkgs'
	},
	{name:'family_code', type:'string', //mapping:'sub_classes.family_code'
	}
	],
	
	belongsTo: 'segmentsinfo'
});

function loadGrids(rawJSON)
{
	var selectedSegment;
	var selectedLegBoard;
	var selectedLegOff;
	var BPVData;
	var chartLegName;
	var chartFlightName;
	
	//rawJSON={"totalCount":"1","flight_date":{"airline_code":"SV","flight_number":"5","departure_date":"2010-Mar-11","legs":[{"board_point":"KBP","off_point":"JFK","board_date":"2010-Mar-11","off_dDate":"2010-Mar-11","board_time":"14:10:00","off_time":"15:55:00","elapsed_time":"00:00:00","date_offset":"0","time_offset":"01:45:00","distance":"0","capacity":"100","cabins":[{"code":"J","offed_cap":"45","phy_cap":"45","rgd_adj":"0","au":"5.339837468899179e-315","upr":"6.895968255258481e-314","ss":"0","staff":"0","wl":"0","group":"0","comm_space":"0","av_pool":"45","avl":"9","nav":"1.520810541237596e-314","gav":"5.317941111879522e-315","acp":"0","etb":"0","bid_price":"0","buckets":[{"yield":"11","si":"47","ss":"0","av":"-19"},{"yield":"11","si":"46","ss":"0","av":"-18"},{"yield":"11","si":"45","ss":"0","av":"-17"},{"yield":"11","si":"44","ss":"0","av":"-16"},{"yield":"11","si":"43","ss":"0","av":"-15"},{"yield":"11","si":"42","ss":"0","av":"-14"},{"yield":"11","si":"41","ss":"0","av":"-13"},{"yield":"11","si":"40","ss":"0","av":"-12"},{"yield":"11","si":"39","ss":"0","av":"-11"},{"yield":"11","si":"38","ss":"0","av":"-10"},{"yield":"11","si":"37","ss":"0","av":"-9"},{"yield":"11","si":"36","ss":"0","av":"-8"},{"yield":"11","si":"35","ss":"0","av":"-7"},{"yield":"11","si":"34","ss":"0","av":"-6"},{"yield":"11","si":"33","ss":"0","av":"-5"},{"yield":"11","si":"32","ss":"0","av":"-4"},{"yield":"11","si":"31","ss":"0","av":"-3"},{"yield":"11","si":"30","ss":"0","av":"-2"},{"yield":"11","si":"29","ss":"0","av":"-1"},{"yield":"11","si":"28","ss":"0","av":"0"},{"yield":"11","si":"27","ss":"0","av":"1"},{"yield":"11","si":"26","ss":"0","av":"2"},{"yield":"11","si":"25","ss":"0","av":"3"},{"yield":"11","si":"24","ss":"0","av":"4"},{"yield":"11","si":"23","ss":"0","av":"5"},{"yield":"11","si":"22","ss":"0","av":"6"},{"yield":"69","si":"21","ss":"0","av":"7"},{"yield":"127","si":"20","ss":"0","av":"8"},{"yield":"184","si":"19","ss":"0","av":"9"},{"yield":"242","si":"18","ss":"0","av":"10"},{"yield":"300","si":"17","ss":"0","av":"11"},{"yield":"478","si":"16","ss":"0","av":"12"},{"yield":"656","si":"15","ss":"0","av":"13"},{"yield":"833","si":"14","ss":"0","av":"14"},{"yield":"1011","si":"13","ss":"0","av":"15"},{"yield":"1071","si":"12","ss":"0","av":"16"},{"yield":"1131","si":"11","ss":"0","av":"17"},{"yield":"1191","si":"10","ss":"0","av":"18"},{"yield":"1251","si":"9","ss":"0","av":"19"},{"yield":"1431","si":"8","ss":"0","av":"20"},{"yield":"1611","si":"7","ss":"0","av":"21"},{"yield":"1790","si":"6","ss":"0","av":"22"},{"yield":"1970","si":"5","ss":"0","av":"23"},{"yield":"1983","si":"4","ss":"0","av":"24"},{"yield":"1995","si":"3","ss":"0","av":"25"},{"yield":"2008","si":"2","ss":"0","av":"26"},{"yield":"2020","si":"1","ss":"0","av":"27"}]},{"code":"Y","offed_cap":"218","phy_cap":"218","rgd_adj":"0","au":"0","upr":"0","ss":"0","staff":"0","wl":"0","group":"0","comm_space":"0","av_pool":"218","avl":"9","nav":"0","gav":"0","acp":"0","etb":"0","bid_price":"0","buckets":[{"yield":"42","si":"225","ss":"0","av":"-200"},{"yield":"42","si":"224","ss":"0","av":"-199"},{"yield":"42","si":"223","ss":"0","av":"-198"},{"yield":"43","si":"222","ss":"0","av":"-197"},{"yield":"44","si":"221","ss":"0","av":"-196"},{"yield":"45","si":"220","ss":"0","av":"-195"},{"yield":"45","si":"219","ss":"0","av":"-194"},{"yield":"46","si":"218","ss":"0","av":"-193"},{"yield":"47","si":"217","ss":"0","av":"-192"},{"yield":"48","si":"216","ss":"0","av":"-191"},{"yield":"48","si":"215","ss":"0","av":"-190"},{"yield":"49","si":"214","ss":"0","av":"-189"},{"yield":"50","si":"213","ss":"0","av":"-188"},{"yield":"51","si":"212","ss":"0","av":"-187"},{"yield":"51","si":"211","ss":"0","av":"-186"},{"yield":"52","si":"210","ss":"0","av":"-185"},{"yield":"53","si":"209","ss":"0","av":"-184"},{"yield":"54","si":"208","ss":"0","av":"-183"},{"yield":"54","si":"207","ss":"0","av":"-182"},{"yield":"55","si":"206","ss":"0","av":"-181"},{"yield":"56","si":"205","ss":"0","av":"-180"},{"yield":"56","si":"204","ss":"0","av":"-179"},{"yield":"57","si":"203","ss":"0","av":"-178"},{"yield":"58","si":"202","ss":"0","av":"-177"},{"yield":"59","si":"201","ss":"0","av":"-176"},{"yield":"59","si":"200","ss":"0","av":"-175"},{"yield":"60","si":"199","ss":"0","av":"-174"},{"yield":"61","si":"198","ss":"0","av":"-173"},{"yield":"62","si":"197","ss":"0","av":"-172"},{"yield":"62","si":"196","ss":"0","av":"-171"},{"yield":"63","si":"195","ss":"0","av":"-170"},{"yield":"64","si":"194","ss":"0","av":"-169"},{"yield":"65","si":"193","ss":"0","av":"-168"},{"yield":"65","si":"192","ss":"0","av":"-167"},{"yield":"66","si":"191","ss":"0","av":"-166"},{"yield":"67","si":"190","ss":"0","av":"-165"},{"yield":"67","si":"189","ss":"0","av":"-164"},{"yield":"68","si":"188","ss":"0","av":"-163"},{"yield":"69","si":"187","ss":"0","av":"-162"},{"yield":"70","si":"186","ss":"0","av":"-161"},{"yield":"70","si":"185","ss":"0","av":"-160"},{"yield":"71","si":"184","ss":"0","av":"-159"},{"yield":"72","si":"183","ss":"0","av":"-158"},{"yield":"73","si":"182","ss":"0","av":"-157"},{"yield":"73","si":"181","ss":"0","av":"-156"},{"yield":"74","si":"180","ss":"0","av":"-155"},{"yield":"75","si":"179","ss":"0","av":"-154"},{"yield":"76","si":"178","ss":"0","av":"-153"},{"yield":"76","si":"177","ss":"0","av":"-152"},{"yield":"77","si":"176","ss":"0","av":"-151"},{"yield":"78","si":"175","ss":"0","av":"-150"},{"yield":"78","si":"174","ss":"0","av":"-149"},{"yield":"79","si":"173","ss":"0","av":"-148"},{"yield":"80","si":"172","ss":"0","av":"-147"},{"yield":"81","si":"171","ss":"0","av":"-146"},{"yield":"81","si":"170","ss":"0","av":"-145"},{"yield":"82","si":"169","ss":"0","av":"-144"},{"yield":"83","si":"168","ss":"0","av":"-143"},{"yield":"84","si":"167","ss":"0","av":"-142"},{"yield":"84","si":"166","ss":"0","av":"-141"},{"yield":"85","si":"165","ss":"0","av":"-140"},{"yield":"86","si":"164","ss":"0","av":"-139"},{"yield":"87","si":"163","ss":"0","av":"-138"},{"yield":"87","si":"162","ss":"0","av":"-137"},{"yield":"88","si":"161","ss":"0","av":"-136"},{"yield":"89","si":"160","ss":"0","av":"-135"},{"yield":"89","si":"159","ss":"0","av":"-134"},{"yield":"90","si":"158","ss":"0","av":"-133"},{"yield":"91","si":"157","ss":"0","av":"-132"},{"yield":"92","si":"156","ss":"0","av":"-131"},{"yield":"92","si":"155","ss":"0","av":"-130"},{"yield":"93","si":"154","ss":"0","av":"-129"},{"yield":"94","si":"153","ss":"0","av":"-128"},{"yield":"95","si":"152","ss":"0","av":"-127"},{"yield":"95","si":"151","ss":"0","av":"-126"},{"yield":"96","si":"150","ss":"0","av":"-125"},{"yield":"97","si":"149","ss":"0","av":"-124"},{"yield":"98","si":"148","ss":"0","av":"-123"},{"yield":"98","si":"147","ss":"0","av":"-122"},{"yield":"99","si":"146","ss":"0","av":"-121"},{"yield":"100","si":"145","ss":"0","av":"-120"},{"yield":"100","si":"144","ss":"0","av":"-119"},{"yield":"101","si":"143","ss":"0","av":"-118"},{"yield":"102","si":"142","ss":"0","av":"-117"},{"yield":"103","si":"141","ss":"0","av":"-116"},{"yield":"103","si":"140","ss":"0","av":"-115"},{"yield":"104","si":"139","ss":"0","av":"-114"},{"yield":"105","si":"138","ss":"0","av":"-113"},{"yield":"106","si":"137","ss":"0","av":"-112"},{"yield":"106","si":"136","ss":"0","av":"-111"},{"yield":"107","si":"135","ss":"0","av":"-110"},{"yield":"108","si":"134","ss":"0","av":"-109"},{"yield":"109","si":"133","ss":"0","av":"-108"},{"yield":"109","si":"132","ss":"0","av":"-107"},{"yield":"110","si":"131","ss":"0","av":"-106"},{"yield":"111","si":"130","ss":"0","av":"-105"},{"yield":"111","si":"129","ss":"0","av":"-104"},{"yield":"112","si":"128","ss":"0","av":"-103"},{"yield":"113","si":"127","ss":"0","av":"-102"},{"yield":"114","si":"126","ss":"0","av":"-101"},{"yield":"114","si":"125","ss":"0","av":"-100"},{"yield":"115","si":"124","ss":"0","av":"-99"},{"yield":"116","si":"123","ss":"0","av":"-98"},{"yield":"117","si":"122","ss":"0","av":"-97"},{"yield":"117","si":"121","ss":"0","av":"-96"},{"yield":"118","si":"120","ss":"0","av":"-95"},{"yield":"119","si":"119","ss":"0","av":"-94"},{"yield":"120","si":"118","ss":"0","av":"-93"},{"yield":"120","si":"117","ss":"0","av":"-92"},{"yield":"121","si":"116","ss":"0","av":"-91"},{"yield":"122","si":"115","ss":"0","av":"-90"},{"yield":"122","si":"114","ss":"0","av":"-89"},{"yield":"123","si":"113","ss":"0","av":"-88"},{"yield":"124","si":"112","ss":"0","av":"-87"},{"yield":"125","si":"111","ss":"0","av":"-86"},{"yield":"125","si":"110","ss":"0","av":"-85"},{"yield":"126","si":"109","ss":"0","av":"-84"},{"yield":"127","si":"108","ss":"0","av":"-83"},{"yield":"128","si":"107","ss":"0","av":"-82"},{"yield":"128","si":"106","ss":"0","av":"-81"},{"yield":"129","si":"105","ss":"0","av":"-80"},{"yield":"130","si":"104","ss":"0","av":"-79"},{"yield":"131","si":"103","ss":"0","av":"-78"},{"yield":"131","si":"102","ss":"0","av":"-77"},{"yield":"132","si":"101","ss":"0","av":"-76"},{"yield":"133","si":"100","ss":"0","av":"-75"},{"yield":"133","si":"99","ss":"0","av":"-74"},{"yield":"134","si":"98","ss":"0","av":"-73"},{"yield":"135","si":"97","ss":"0","av":"-72"},{"yield":"136","si":"96","ss":"0","av":"-71"},{"yield":"136","si":"95","ss":"0","av":"-70"},{"yield":"137","si":"94","ss":"0","av":"-69"},{"yield":"138","si":"93","ss":"0","av":"-68"},{"yield":"139","si":"92","ss":"0","av":"-67"},{"yield":"139","si":"91","ss":"0","av":"-66"},{"yield":"140","si":"90","ss":"0","av":"-65"},{"yield":"141","si":"89","ss":"0","av":"-64"},{"yield":"142","si":"88","ss":"0","av":"-63"},{"yield":"142","si":"87","ss":"0","av":"-62"},{"yield":"143","si":"86","ss":"0","av":"-61"},{"yield":"144","si":"85","ss":"0","av":"-60"},{"yield":"144","si":"84","ss":"0","av":"-59"},{"yield":"145","si":"83","ss":"0","av":"-58"},{"yield":"146","si":"82","ss":"0","av":"-57"},{"yield":"147","si":"81","ss":"0","av":"-56"},{"yield":"147","si":"80","ss":"0","av":"-55"},{"yield":"148","si":"79","ss":"0","av":"-54"},{"yield":"149","si":"78","ss":"0","av":"-53"},{"yield":"150","si":"77","ss":"0","av":"-52"},{"yield":"150","si":"76","ss":"0","av":"-51"},{"yield":"151","si":"75","ss":"0","av":"-50"},{"yield":"152","si":"74","ss":"0","av":"-49"},{"yield":"153","si":"73","ss":"0","av":"-48"},{"yield":"153","si":"72","ss":"0","av":"-47"},{"yield":"154","si":"71","ss":"0","av":"-46"},{"yield":"155","si":"70","ss":"0","av":"-45"},{"yield":"155","si":"69","ss":"0","av":"-44"},{"yield":"156","si":"68","ss":"0","av":"-43"},{"yield":"157","si":"67","ss":"0","av":"-42"},{"yield":"158","si":"66","ss":"0","av":"-41"},{"yield":"158","si":"65","ss":"0","av":"-40"},{"yield":"159","si":"64","ss":"0","av":"-39"},{"yield":"160","si":"63","ss":"0","av":"-38"},{"yield":"161","si":"62","ss":"0","av":"-37"},{"yield":"161","si":"61","ss":"0","av":"-36"},{"yield":"162","si":"60","ss":"0","av":"-35"},{"yield":"163","si":"59","ss":"0","av":"-34"},{"yield":"164","si":"58","ss":"0","av":"-33"},{"yield":"164","si":"57","ss":"0","av":"-32"},{"yield":"165","si":"56","ss":"0","av":"-31"},{"yield":"166","si":"55","ss":"0","av":"-30"},{"yield":"166","si":"54","ss":"0","av":"-29"},{"yield":"167","si":"53","ss":"0","av":"-28"},{"yield":"168","si":"52","ss":"0","av":"-27"},{"yield":"169","si":"51","ss":"0","av":"-26"},{"yield":"169","si":"50","ss":"0","av":"-25"},{"yield":"170","si":"49","ss":"0","av":"-24"},{"yield":"171","si":"48","ss":"0","av":"-23"},{"yield":"172","si":"47","ss":"0","av":"-22"},{"yield":"172","si":"46","ss":"0","av":"-21"},{"yield":"173","si":"45","ss":"0","av":"-20"},{"yield":"174","si":"44","ss":"0","av":"-19"},{"yield":"175","si":"43","ss":"0","av":"-18"},{"yield":"175","si":"42","ss":"0","av":"-17"},{"yield":"176","si":"41","ss":"0","av":"-16"},{"yield":"177","si":"40","ss":"0","av":"-15"},{"yield":"177","si":"39","ss":"0","av":"-14"},{"yield":"178","si":"38","ss":"0","av":"-13"},{"yield":"179","si":"37","ss":"0","av":"-12"},{"yield":"180","si":"36","ss":"0","av":"-11"},{"yield":"180","si":"35","ss":"0","av":"-10"},{"yield":"181","si":"34","ss":"0","av":"-9"},{"yield":"182","si":"33","ss":"0","av":"-8"},{"yield":"183","si":"32","ss":"0","av":"-7"},{"yield":"183","si":"31","ss":"0","av":"-6"},{"yield":"184","si":"30","ss":"0","av":"-5"},{"yield":"185","si":"29","ss":"0","av":"-4"},{"yield":"186","si":"28","ss":"0","av":"-3"},{"yield":"186","si":"27","ss":"0","av":"-2"},{"yield":"187","si":"26","ss":"0","av":"-1"},{"yield":"188","si":"25","ss":"0","av":"0"},{"yield":"188","si":"24","ss":"0","av":"1"},{"yield":"197","si":"23","ss":"0","av":"2"},{"yield":"206","si":"22","ss":"0","av":"3"},{"yield":"215","si":"21","ss":"0","av":"4"},{"yield":"224","si":"20","ss":"0","av":"5"},{"yield":"233","si":"19","ss":"0","av":"6"},{"yield":"242","si":"18","ss":"0","av":"7"},{"yield":"251","si":"17","ss":"0","av":"8"},{"yield":"260","si":"16","ss":"0","av":"9"},{"yield":"269","si":"15","ss":"0","av":"10"},{"yield":"305","si":"14","ss":"0","av":"11"},{"yield":"341","si":"13","ss":"0","av":"12"},{"yield":"378","si":"12","ss":"0","av":"13"},{"yield":"414","si":"11","ss":"0","av":"14"},{"yield":"450","si":"10","ss":"0","av":"15"},{"yield":"454","si":"9","ss":"0","av":"16"},{"yield":"458","si":"8","ss":"0","av":"17"},{"yield":"521","si":"7","ss":"0","av":"18"},{"yield":"583","si":"6","ss":"0","av":"19"},{"yield":"761","si":"5","ss":"0","av":"20"},{"yield":"762","si":"4","ss":"0","av":"21"},{"yield":"762","si":"3","ss":"0","av":"22"},{"yield":"1005","si":"2","ss":"0","av":"23"},{"yield":"1248","si":"1","ss":"0","av":"24"}]}]}],"segments":[{"segment":"KBP,JFK","sub_classes":[{"cabin_code":"J","family_code":"1","class_code":"J","min_au_prot":"45(0)","nego":"0","ns%":"0","ob%":"0","bkgs":"0","grp_bks_pdg":"0","stf_bkgs":"0","wl_bkgs":"0","etb":"0","class_avl":"0","seg_avl":"0","rev_avl":"0"},{"cabin_code":"J","family_code":"1","class_code":"C","min_au_prot":"45(0)","nego":"0","ns%":"0","ob%":"0","bkgs":"0","grp_bks_pdg":"0","stf_bkgs":"0","wl_bkgs":"0","etb":"0","class_avl":"0","seg_avl":"0","rev_avl":"0"},{"cabin_code":"J","family_code":"1","class_code":"D","min_au_prot":"45(0)","nego":"0","ns%":"0","ob%":"0","bkgs":"0","grp_bks_pdg":"0","stf_bkgs":"0","wl_bkgs":"0","etb":"0","class_avl":"0","seg_avl":"0","rev_avl":"0"},{"cabin_code":"J","family_code":"1","class_code":"I","min_au_prot":"45(0)","nego":"0","ns%":"0","ob%":"0","bkgs":"0","grp_bks_pdg":"0","stf_bkgs":"0","wl_bkgs":"0","etb":"0","class_avl":"0","seg_avl":"0","rev_avl":"0"},{"cabin_code":"J","family_code":"1","class_code":"F","min_au_prot":"45(0)","nego":"0","ns%":"0","ob%":"0","bkgs":"0","grp_bks_pdg":"0","stf_bkgs":"0","wl_bkgs":"0","etb":"0","class_avl":"0","seg_avl":"0","rev_avl":"0"},{"cabin_code":"J","family_code":"1","class_code":"U","min_au_prot":"45(0)","nego":"0","ns%":"0","ob%":"0","bkgs":"0","grp_bks_pdg":"0","stf_bkgs":"0","wl_bkgs":"0","etb":"0","class_avl":"0","seg_avl":"0","rev_avl":"0"},{"cabin_code":"Y","family_code":"1","class_code":"Y","min_au_prot":"218(0)","nego":"0","ns%":"0","ob%":"0","bkgs":"0","grp_bks_pdg":"0","stf_bkgs":"0","wl_bkgs":"0","etb":"0","class_avl":"0","seg_avl":"0","rev_avl":"0"},{"cabin_code":"Y","family_code":"1","class_code":"B","min_au_prot":"218(0)","nego":"0","ns%":"0","ob%":"0","bkgs":"0","grp_bks_pdg":"0","stf_bkgs":"0","wl_bkgs":"0","etb":"0","class_avl":"0","seg_avl":"0","rev_avl":"0"},{"cabin_code":"Y","family_code":"1","class_code":"H","min_au_prot":"218(0)","nego":"0","ns%":"0","ob%":"0","bkgs":"0","grp_bks_pdg":"0","stf_bkgs":"0","wl_bkgs":"0","etb":"0","class_avl":"0","seg_avl":"0","rev_avl":"0"},{"cabin_code":"Y","family_code":"1","class_code":"K","min_au_prot":"218(0)","nego":"0","ns%":"0","ob%":"0","bkgs":"0","grp_bks_pdg":"0","stf_bkgs":"0","wl_bkgs":"0","etb":"0","class_avl":"0","seg_avl":"0","rev_avl":"0"},{"cabin_code":"Y","family_code":"1","class_code":"M","min_au_prot":"218(0)","nego":"0","ns%":"0","ob%":"0","bkgs":"0","grp_bks_pdg":"0","stf_bkgs":"0","wl_bkgs":"0","etb":"0","class_avl":"0","seg_avl":"0","rev_avl":"0"},{"cabin_code":"Y","family_code":"1","class_code":"P","min_au_prot":"218(0)","nego":"0","ns%":"0","ob%":"0","bkgs":"0","grp_bks_pdg":"0","stf_bkgs":"0","wl_bkgs":"0","etb":"0","class_avl":"0","seg_avl":"0","rev_avl":"0"},{"cabin_code":"Y","family_code":"1","class_code":"T","min_au_prot":"218(0)","nego":"0","ns%":"0","ob%":"0","bkgs":"0","grp_bks_pdg":"0","stf_bkgs":"0","wl_bkgs":"0","etb":"0","class_avl":"0","seg_avl":"0","rev_avl":"0"},{"cabin_code":"Y","family_code":"1","class_code":"L","min_au_prot":"218(0)","nego":"0","ns%":"0","ob%":"0","bkgs":"0","grp_bks_pdg":"0","stf_bkgs":"0","wl_bkgs":"0","etb":"0","class_avl":"0","seg_avl":"0","rev_avl":"0"},{"cabin_code":"Y","family_code":"1","class_code":"V","min_au_prot":"218(0)","nego":"0","ns%":"0","ob%":"0","bkgs":"0","grp_bks_pdg":"0","stf_bkgs":"0","wl_bkgs":"0","etb":"0","class_avl":"0","seg_avl":"0","rev_avl":"0"},{"cabin_code":"Y","family_code":"1","class_code":"S","min_au_prot":"218(0)","nego":"0","ns%":"0","ob%":"0","bkgs":"0","grp_bks_pdg":"0","stf_bkgs":"0","wl_bkgs":"0","etb":"0","class_avl":"0","seg_avl":"0","rev_avl":"0"},{"cabin_code":"Y","family_code":"1","class_code":"N","min_au_prot":"218(0)","nego":"0","ns%":"0","ob%":"0","bkgs":"0","grp_bks_pdg":"0","stf_bkgs":"0","wl_bkgs":"0","etb":"0","class_avl":"0","seg_avl":"0","rev_avl":"0"},{"cabin_code":"Y","family_code":"1","class_code":"Q","min_au_prot":"218(0)","nego":"0","ns%":"0","ob%":"0","bkgs":"0","grp_bks_pdg":"0","stf_bkgs":"0","wl_bkgs":"0","etb":"0","class_avl":"0","seg_avl":"0","rev_avl":"0"},{"cabin_code":"Y","family_code":"1","class_code":"O","min_au_prot":"218(0)","nego":"0","ns%":"0","ob%":"0","bkgs":"0","grp_bks_pdg":"0","stf_bkgs":"0","wl_bkgs":"0","etb":"0","class_avl":"0","seg_avl":"0","rev_avl":"0"},{"cabin_code":"Y","family_code":"1","class_code":"Z","min_au_prot":"218(0)","nego":"0","ns%":"0","ob%":"0","bkgs":"0","grp_bks_pdg":"0","stf_bkgs":"0","wl_bkgs":"0","etb":"0","class_avl":"0","seg_avl":"0","rev_avl":"0"},{"cabin_code":"Y","family_code":"1","class_code":"R","min_au_prot":"218(0)","nego":"0","ns%":"0","ob%":"0","bkgs":"0","grp_bks_pdg":"0","stf_bkgs":"0","wl_bkgs":"0","etb":"0","class_avl":"0","seg_avl":"0","rev_avl":"0"},{"cabin_code":"Y","family_code":"1","class_code":"G","min_au_prot":"218(0)","nego":"0","ns%":"0","ob%":"0","bkgs":"0","grp_bks_pdg":"0","stf_bkgs":"0","wl_bkgs":"0","etb":"0","class_avl":"0","seg_avl":"0","rev_avl":"0"},{"cabin_code":"Y","family_code":"1","class_code":"W","min_au_prot":"218(0)","nego":"0","ns%":"0","ob%":"0","bkgs":"0","grp_bks_pdg":"0","stf_bkgs":"0","wl_bkgs":"0","etb":"0","class_avl":"0","seg_avl":"0","rev_avl":"0"},{"cabin_code":"Y","family_code":"1","class_code":"X","min_au_prot":"218(0)","nego":"0","ns%":"0","ob%":"0","bkgs":"0","grp_bks_pdg":"0","stf_bkgs":"0","wl_bkgs":"0","etb":"0","class_avl":"0","seg_avl":"0","rev_avl":"0"},{"cabin_code":"Y","family_code":"1","class_code":"E","min_au_prot":"218(0)","nego":"0","ns%":"0","ob%":"0","bkgs":"0","grp_bks_pdg":"0","stf_bkgs":"0","wl_bkgs":"0","etb":"0","class_avl":"0","seg_avl":"0","rev_avl":"0"},{"cabin_code":"Y","family_code":"1","class_code":"A","min_au_prot":"218(0)","nego":"0","ns%":"0","ob%":"0","bkgs":"0","grp_bks_pdg":"0","stf_bkgs":"0","wl_bkgs":"0","etb":"0","class_avl":"0","seg_avl":"0","rev_avl":"0"}]}]}};
	//var segs = rawJSON.flight_date.segments[0];
	//var cabs = rawJSON.flight_date.legs[0];	
	
	var tempSegs;
	var tempCabs;
	
    var store_flights = Ext.create('Ext.data.Store', {
        model: 'flightinfo',
        proxy: {
            type: 'memory',
			//method: 'POST',
            //url: url,
            data: rawJSON,
            reader: {
                type: 'json',
                root: 'flight_date'
            }, 
        }   
    });
	store_flights.load();
	
	var store_segments = Ext.create('Ext.data.Store', {
        model: 'segmentsinfo',
        proxy: {
            type: 'memory',
			//method: 'POST',
            //url: url,
            data: rawJSON,
            reader: {
                type: 'json',
                root: 'flight_date.segments'
            }, 
        }   
    });
	store_segments.load();
	
	var store_legs = Ext.create('Ext.data.Store', {
        model: 'legsinfo',
        proxy: {
            type: 'memory',
			//method: 'POST',
            //url: url,
            data: rawJSON,
            reader: {
                type: 'json',
                root: 'flight_date.legs'
            }, 
        }  
    });
	store_legs.load();
		
    // create the grid
    var grid_flight = new Ext.grid.GridPanel({
        store: store_flights,
        columns: [
            {'header': "Departure Date", 'width': 100, 'dataIndex': 'departure_date', sortable: true},
            {'header': "Airline Code", 'width': 80, 'dataIndex': 'airline_code', sortable: true},
            {'header': "Flight Number", 'width': 90, 'dataIndex': 'flight_number', sortable: true}  
        ],
        //title: 'Found Flights',
		renderTo:'flights-GridDisplay',
        width:270,
        height:150
		/*listeners: {
			itemclick : function() {
				var data=grid_flight.getSelectionModel().selected.items[0].data;
				grid_legs.setTitle('Legs List');
				store_legs.clearFilter();
				store_legs.load();
			}
		}*/	
    });
    
    var grid_segments = new Ext.grid.GridPanel({
        store: store_segments,
        columns: [
            {header: "Segments", width: 70, dataIndex: 'segment', sortable: true},
        ],
        //title: 'Segments',
		renderTo:'segments-GridDisplay',
        width:70,
        height:150
    });
    
     grid_segments.getSelectionModel().on('selectionchange', function(sm, selectedRecord) {
		
		if(selectedRecord[0] != undefined)
		{
			selectedSegment = selectedRecord[0].data.segment;
			//console.log("Segment selected is: " + selectedSegment);
			
			var $tabs = $('#tabs').tabs();		
			$tabs.tabs('select', 0); // switch to first tab
			
			
			for (var i in rawJSON.flight_date.segments)
			{
				if(rawJSON.flight_date.segments[i].segment == selectedSegment)
				{	
					tempSegs = rawJSON.flight_date.segments[i];
					//console.log(tempSegs);
				}
			}
			
			loadSubclasses(tempSegs);
		}
	});
    
    var grid_legs = new Ext.grid.GridPanel({
        store: store_legs,
        columns: [
            {header: "Board Date", width: 80, dataIndex: 'board_date', sortable: true},
            {header: "Off Date", width: 80, dataIndex: 'off_dDate', sortable: true},
            {header: "Board Point", width: 80, dataIndex: 'board_point', sortable: true},
            {header: "Off Point", width: 80, dataIndex: 'off_point', sortable: true},
            {header: "Distance", width: 70, dataIndex: 'distance', sortable: true},
            {header: "Capacity", width: 65, dataIndex: 'capacity', sortable: true},
			{header: "Board Time", width: 80, dataIndex: 'board_time', sortable: true},
			{header: "Elapsed Time", width: 80, dataIndex: 'elapsed_time', sortable: true},
			{header: "Date Offset", width: 80, dataIndex: 'date_offset', sortable: true},
			{header: "Off Time", width: 80, dataIndex: 'off_time', sortable: true},
			{header: "Time Offset", width: 80, dataIndex: 'time_offset', sortable: true},
			
        ],
        //title: 'Legs',
		renderTo:'legs-GridDisplay',
        width:853,
        height:150
    });
    
    grid_legs.getSelectionModel().on('selectionchange', function(sm, selectedRecord) {
		
		if(selectedRecord[0] != undefined)
		{
			selectedLegBoard = selectedRecord[0].data.board_point;
			selectedLegOff = selectedRecord[0].data.off_point;
			
			//console.log("Leg selected is: " + selectedLegBoard +" to " + selectedLegOff);
			chartLegName = selectedLegBoard +" to " + selectedLegOff;
			
			var displayLeg = selectedRecord[0].data.board_point;
			var $tabs = $('#tabs').tabs();		
			$tabs.tabs('select', 1); // switch to second tab
			
			//BidPrice chart only works for 1 cabin!
			for (var i in rawJSON.flight_date.legs)
			{
				if(rawJSON.flight_date.legs[i].board_point == selectedLegBoard && rawJSON.flight_date.legs[i].off_point == selectedLegOff)
				{	
					tempCabs = rawJSON.flight_date.legs[i];
					BPVData = rawJSON.flight_date.legs[i].cabins[0].BPV;
					chartFlightName = rawJSON.flight_date.airline_code + " " + rawJSON.flight_date.flight_number;
					
					//console.log(BPVData);
				}
			}
			
			loadCabins(tempCabs, BPVData, chartLegName, chartFlightName);
		}
	});
}

function loadSubclasses(segs)
{
	document.getElementById('subclasses-GridDisplay').innerHTML = "";
	var store_subclasses = Ext.create('Ext.data.Store', {
        model: 'subclassesinfo',
        proxy: {
            type: 'memory',
			//method: 'POST',
            //url: url,
            data: segs,
            reader: {
                type: 'json',
                root: 'sub_classes'
            }, 
        }    
    });
	store_subclasses.load();
			
	var cellEditing = Ext.create('Ext.grid.plugin.CellEditing', {clicksToEdit:1});
			
	var grid_subclasses = new Ext.grid.GridPanel({
		store: store_subclasses,
		columns: [
			{header: "Class Code", width: 90, dataIndex: 'class_code', sortable: true},
			{header: "Cabin Code", width: 90, dataIndex: 'cabin_code', sortable: true},
			{header: "Family Code", width: 90, dataIndex: 'family_code', sortable: true},
			{header: "MIN/AU (Prot)", width: 90, dataIndex: 'min_au_prot', sortable: true},
			{header: "Nego", width: 40, dataIndex: 'nego', sortable: true, type: 'int',field: {xtype: 'numberfield',allowBlank: false,minValue: 0,maxValue: 100}},
			{header: "NS%", width: 40, dataIndex: 'ns%', sortable: true, type: 'int',field: {xtype: 'numberfield',allowBlank: false,minValue: 0,maxValue: 100}},
			{header: "OB%", width: 40, dataIndex: 'ob%', sortable: true, type: 'int',field: {xtype: 'numberfield',allowBlank: false,minValue: 0,maxValue: 100}},
			{header: "Bookings", width: 60, dataIndex: 'bkgs', sortable: true, type: 'int',field: {xtype: 'numberfield',allowBlank: false,minValue: 0,maxValue: 100}},
			{header: "Group Bookings", width: 90, dataIndex: 'grp_bks_pdg', sortable: true, type: 'int',field: {xtype: 'numberfield',allowBlank: false,minValue: 0,maxValue: 100}},
			{header: "Staff Bookings", width: 90, dataIndex: 'stf_bkgs', sortable: true, type: 'int',field: {xtype: 'numberfield',allowBlank: false,minValue: 0,maxValue: 100}},
			{header: "WL Bookings", width: 80, dataIndex: 'wl_bkgs', sortable: true, type: 'int',field: {xtype: 'numberfield',allowBlank: false,minValue: 0,maxValue: 100}},
			{header: "ETB", width: 40, dataIndex: 'etb', sortable: true, type: 'int',field: {xtype: 'numberfield',allowBlank: false,minValue: 0,maxValue: 100}},
			{header: "Class AVL", width: 80, dataIndex: 'class_avl', sortable: true, type: 'int',field: {xtype: 'numberfield',allowBlank: false,minValue: 0,maxValue: 100}},
			{header: "Rev AVL", width: 70, dataIndex: 'rev_avl', sortable: true, type: 'int',field: {xtype: 'numberfield',allowBlank: false,minValue: 0,maxValue: 100}},
			{header: "Seg AVL", width: 70, dataIndex: 'seg_avl', sortable: true, type: 'int',field: {xtype: 'numberfield',allowBlank: false,minValue: 0,maxValue: 100}},
		],
		//title: 'Subclasses',
		renderTo:'subclasses-GridDisplay',
		width:1192,
		height:400,
		plugins: [cellEditing]
    });
}

function loadCabins(cabs, BPVData, chartLegName, chartFlightName)
{
    document.getElementById('cabins-GridDisplay').innerHTML = "";
    var store_cabins = Ext.create('Ext.data.Store', {
        model: 'cabinsinfo',
        proxy: {
            type: 'memory',
			//method: 'POST',
            //url: url,
            data: cabs,
            reader: {
                type: 'json',
                root: 'cabins'
            }, 
        }    
    });
	store_cabins.load();
			
	var cellEditing = Ext.create('Ext.grid.plugin.CellEditing', {clicksToEdit:1});
			
	var grid_cabins = new Ext.grid.GridPanel({
		store: store_cabins,
		columns: [
			{header: "Avl", width: 50, dataIndex: 'avl', sortable: true},
			{header: "Code", width: 50, dataIndex: 'code', sortable: true},
			{header: "Offed_Cap", width: 70, dataIndex: 'offed_cap', sortable: true},
			{header: "SS", width: 30, dataIndex: 'ss', sortable: true},
			{header: "GAV", width: 50, dataIndex: 'gav', sortable: true},
			{header: "Phy_Cap", width: 80, dataIndex: 'phy_cap', sortable: true},
			{header: "ETB", width: 40, dataIndex: 'etb', sortable: true, type: 'int',field: {xtype: 'numberfield',allowBlank: false,minValue: 0,maxValue: 100}},
			{header: "ACP", width: 40, dataIndex: 'acp', sortable: true, type: 'int',field: {xtype: 'numberfield',allowBlank: false,minValue: 0,maxValue: 100}},
			{header: "AU", width: 40, dataIndex: 'au', sortable: true, type: 'int',field: {xtype: 'numberfield',allowBlank: false,minValue: 0,maxValue: 100}},
			{header: "Comm_Space", width: 80, dataIndex: 'comm_space', sortable: true, type: 'int',field: {xtype: 'numberfield',allowBlank: false,minValue: 0,maxValue: 100}},
			{header: "Rgd_Adj", width: 80, dataIndex: 'rgd_adj', sortable: true, type: 'int',field: {xtype: 'numberfield',allowBlank: false,minValue: 0,maxValue: 100}},
			{header: "WL", width: 90, dataIndex: 'wl', sortable: true, type: 'int',field: {xtype: 'numberfield',allowBlank: false,minValue: 0,maxValue: 100}},
			{header: "Group", width: 80, dataIndex: 'group', sortable: true, type: 'int',field: {xtype: 'numberfield',allowBlank: false,minValue: 0,maxValue: 100}},
			{header: "Bid Price", width: 60, dataIndex: 'bid_price', sortable: true, type: 'int',field: {xtype: 'numberfield',allowBlank: false,minValue: 0,maxValue: 100}},
			{header: "Nav", width: 80, dataIndex: 'nav', sortable: true, type: 'int',field: {xtype: 'numberfield',allowBlank: false,minValue: 0,maxValue: 100}},
			{header: "UPR", width: 70, dataIndex: 'upr', sortable: true, type: 'int',field: {xtype: 'numberfield',allowBlank: false,minValue: 0,maxValue: 100}},
			{header: "Av_Pool", width: 70, dataIndex: 'av_pool', sortable: true, type: 'int',field: {xtype: 'numberfield',allowBlank: false,minValue: 0,maxValue: 100}},
			{header: "Staff", width: 70, dataIndex: 'staff', sortable: true, type: 'int',field: {xtype: 'numberfield',allowBlank: false,minValue: 0,maxValue: 100}}
		],
		//title: 'Cabins',
		renderTo:'cabins-GridDisplay',
		width:1192,
		height:400,
		plugins: [cellEditing]
    });
    
    window.rebuildBPVChart(BPVData, chartLegName, chartFlightName);
}




/*var IMG_GO = "assets/icons/iconDeparture.png";
function renderSelect(legsArray) {
		//return Ext.String.format(
            return '<img src=' + IMG_GO + ' /><input type="button" value="Display Legs" id="bt"onClick="showLegs()"/>'
        //<a href="http://www.espn.com" target="_blank">);
		//<input type="button" value="toto" id="'+val+'"/>
    }*/
