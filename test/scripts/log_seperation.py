# Import section
import math

#########################################################################
def print_final_results (demand_dict, qbkg_dict, unc_dict, si):
    D = 1600.0
    # Initialise the lists to be printed
    od_list = []
    oq_list = []
    ou_list = []
    for x in range (si):
        od_list.append(0.0)
        oq_list.append(0.0)
        ou_list.append(0.0)
    
    for date, ulist in unc_dict.items():
        dlist = demand_dict[date]
        qlist = qbkg_dict[date]
        cumud_list = []
        cumuq_list = []
        cumuu_list = []
        for x in range (si):
            cumud_list.append(0.0)
            cumuq_list.append(0.0)
            cumuu_list.append(0.0)
        for x in range (si-1):
            cumud_list[x+1] = cumud_list[x] + dlist[x]
            cumuq_list[x+1] = cumuq_list[x] + qlist[x]
            cumuu_list[x+1] = cumuu_list[x] + ulist[x]
        total_demand = 0.0
        for x in range (si-1):
            total_demand += dlist[x]
        total_demand = total_demand/100.0
        for x in range (si):
            od_list[x] += cumud_list[x]/total_demand
            oq_list[x] += cumuq_list[x]/total_demand
            ou_list[x] += cumuu_list[x]/total_demand


    for x in range (si):
        od_list[x] = od_list[x] / len(unc_dict)
        oq_list[x] = oq_list[x] / len(unc_dict)
        ou_list[x] = ou_list[x] / len(unc_dict)

    print od_list
    print oq_list
    print ou_list
                   
        

#########################################################################
def print_dict (demand_dict):
    mean = 0.0
    stddev = 0.0
    flights = 0
    for date, dtd_list in demand_dict.items():
        nb = 0
        for dtd in dtd_list:
            if (dtd > -62):
                nb = nb + 1
        #
        stddev = stddev + (flights * mean * mean)
        mean = mean * flights
        flights += 1
        mean += nb
        mean = mean/flights
        stddev = stddev - (flights * mean * mean)
        stddev += nb * nb
        #if (date == "20101122"):
        #    print date, dtd_list
        print date, dtd_list
        #print date, nb
    stddev = stddev / flights
    stddev = math.sqrt (stddev)
    #print mean, stddev

#########################################################################
def print_period_dict (dem_dict, be, si):
    for date, demand_list in dem_dict.items():
        new_list = []
        for x in range(si):
            if (x >= be):
                new_list.append (demand_list[x])
        print date, new_list    

#########################################################################
def print_distrib (demand_dict):
    mean = 0.0
    stddev = 0.0
    flights = 0
    for date, dtd_list in demand_dict.items():
        stddev = stddev + (flights * mean * mean)
        mean = mean * flights
        flights += 1
        mean += len(dtd_list)
        mean = mean/flights
        stddev = stddev - (flights * mean * mean)
        stddev += (len(dtd_list) * len(dtd_list))
        #print date, len(dtd_list)
    stddev = stddev / flights
    stddev = math.sqrt (stddev)
    print mean, stddev

#########################################################################
def sum_period1 (demand_dict):
    demand_period_dict = {}
    for date, dtd_list in demand_dict.items():
        period_dtd = []
        for x in range(16):
            period_dtd.append(0)
        for x in dtd_list:
            if (x > -62 and x <= -55):
                period_dtd[0] += 1
            if (x > -55 and x <= -48):
                period_dtd[1] += 1
            if (x > -48 and x <= -41):
                period_dtd[2] += 1
            if (x > -41 and x <= -34):
                period_dtd[3] += 1
            if (x > -34 and x <= -30):
                period_dtd[4] += 1
            if (x > -30 and x <= -26):
                period_dtd[5] += 1
            if (x > -26 and x <= -22):
                period_dtd[6] += 1
            if (x > -22 and x <= -18):
                period_dtd[7] += 1
            if (x > -18 and x <= -15):
                period_dtd[8] += 1
            if (x > -15 and x <= -12):
                period_dtd[9] += 1
            if (x > -12 and x <= -9):
                period_dtd[10] += 1
            if (x > -9 and x <= -6):
                period_dtd[11] += 1
            if (x > -6 and x <= -4):
                period_dtd[12] += 1
            if (x > -4 and x <= -2):
                period_dtd[13] += 1
            if (x > -2 and x <= -0):
                period_dtd[14] += 1
            if (x > -0 and x <= 1):
                period_dtd[15] += 1
        demand_period_dict[date]=period_dtd
    return demand_period_dict

#########################################################################
def sum_period2 (demand_dict):
    demand_period_dict = {}
    for date, dtd_list in demand_dict.items():
        period_dtd = []
        for x in range(8):
            period_dtd.append(0)
        for x in dtd_list:
            if (x > -62 and x <= -48):
                period_dtd[0] += 1
            if (x > -48 and x <= -34):
                period_dtd[1] += 1
            if (x > -34 and x <= -22):
                period_dtd[2] += 1
            if (x > -22 and x <= -15):
                period_dtd[3] += 1
            if (x > -15 and x <= -9):
                period_dtd[4] += 1
            if (x > -9 and x <= -4):
                period_dtd[5] += 1
            if (x > -4 and x <= -0):
                period_dtd[6] += 1
            if (x > -0 and x <= 1):
                period_dtd[7] += 1
        demand_period_dict[date]=period_dtd
    return demand_period_dict

#########################################################################
def convert_to_8_dcp (demand_dict):
    odict = {}
    for date, period_list in demand_dict.items():
        new_list = []
        for x in range (8):
            new_list.append(0)
        new_list[0] = period_list[0] + period_list[1]
        new_list[1] = period_list[2] + period_list[3]
        new_list[2] = period_list[4] + period_list[5] + period_list[6]
        new_list[3] = period_list[7] + period_list[8]
        new_list[4] = period_list[9] + period_list[10]
        new_list[5] = period_list[11] + period_list[12]
        new_list[6] = period_list[13] + period_list[14]
        new_list[7] = period_list[15]
        odict[date] = new_list
    return odict

#########################################################################
def print_diff1 (dict1, dict2, bo):
    diff_list=[]
    diff_count=[]
    for x in range(16):
        diff_list.append(0.0)
        diff_count.append(0)
    for date, list2 in dict2.items():
        list1 = dict1[date]
        for x in range(16):
            if (list1[x] != 0) and (list1[x] != list2[x]):
                diff_list[x]+=100*abs(list2[x]-list1[x])/list1[x]
                #diff_list[x]+=abs(list2[x] - list1[x])
                diff_count[x] += 1
                #if (x == 2):
                #    print list1[x], list2[x], date
    for x in range(16):
        if (diff_count[x] > 0):
            diff_list[x] = round(diff_list[x]/diff_count[x],3)
    if (bo == 0):
        print diff_list
    if (bo == 1):
        print diff_count

#########################################################################
def print_diff2 (dict1, dict2, bo):
    diff_list=[]
    diff_count=[]
    for x in range(8):
        diff_list.append(0.0)
        diff_count.append(0)
    for date, list2 in dict2.items():
        list1 = dict1[date]
        for x in range(8):
            if (list1[x] != 0) and (list1[x] != list2[x]):
                diff_list[x]+=100*abs(list2[x]-list1[x])/list1[x]
                #diff_list[x]+=abs(list2[x] - list1[x])
                diff_count[x] += 1
                #if (x == 0):
                #    print list1[x], list2[x], date
    for x in range(8):
        if (diff_count[x] > 0):
            diff_list[x] = round(diff_list[x]/diff_count[x],3)
    if (bo == 0):
        print diff_list
    if (bo == 1):
        print diff_count

#########################################################################
def extract_date (fd):
    month_dict = {'Jan':'01', 'Feb':'02', 'Mar':'03', 'Apr':'04', 'May':'05', 'Jun':'06', 'Jul':'07','Aug':'08','Sep':'09','Oct':'10','Nov':'11','Dec':'12'}
    fn, date = fd.split(',')
    y, m, d = date.split('-')
    output = ''
    output += y
    output += month_dict[m]
    output += d
    return output
    

#########################################################################
############################### M A I N #################################
#########################################################################

input_file = open('simulate.log')
lines = input_file.readlines()

demand_dict={}
unc_demand_dict={}
qbkg_dict={}
bkg_dict={}
for i in lines:
    try:
        headline, datedtd = i.split(': ')
    except (ValueError):
        print i

    headline, datedtd = i.split(': ')

    # RMOL
    if (headline == "[N]../../../rmol/command/Forecaster.cpp:730") or \
            (headline == "[N]../../../rmol/command/Detruncator.cpp:226"):
        fd, dtd, begin, end, hfd, dem = datedtd.split(';')
        try:
            unc_demand_dict[hfd]
        except (KeyError):
            unc_demand_dict[hfd] = []
        dem_list = unc_demand_dict[hfd]
        dem_float = float (dem)
        dem_list.append (dem_float)

    if (headline == "[N]../../../rmol/command/Detruncator.cpp:232") or \
            (headline == "[N]../../../rmol/command/Forecaster.cpp:735"):
        fd, dtd, begin, end, hfd, bkg = datedtd.split(';')
        try:
            qbkg_dict[hfd]
        except (KeyError):
            qbkg_dict[hfd] = []
        bkg_list = qbkg_dict[hfd]
        bkg_float = float (bkg)
        bkg_list.append (bkg_float)

    # TRADEMGEN
    if (headline == "[N]../../../trademgen/bom/DemandStream.cpp:385") or \
            (headline == "[N]../../../trademgen/bom/DemandStream.cpp:276"):
        date, dtd = datedtd.split(';')
        try:
            demand_dict[date]
        except (KeyError):
            demand_dict[date] = []
        dtd_list = demand_dict[date]
        dtd_float = float (dtd)
        dtd_list.append (dtd_float)
        
    # BKGs
    if (headline == "[N]../../../tvlsim/command/Simulator.cpp:228"):
        c, fd, od, t, dtd = datedtd.split(';')
        date = extract_date (fd)
        try:
            bkg_dict[date]
        except (KeyError):
            bkg_dict[date] = []
        dtd_list = bkg_dict[date]
        dtd_float = float (dtd)
        dtd_list.append (dtd_float)


demand_period_dict = sum_period1 (demand_dict)
bkg_period_dict = sum_period1 (bkg_dict)


#print_diff2 (demand_period_dict, bkg_period_dict, 0)
print_diff1 (demand_period_dict, qbkg_dict, 0)
print_diff1 (demand_period_dict, unc_demand_dict, 0)
print_diff1 (qbkg_dict, unc_demand_dict, 1)
 
#demand_8 = convert_to_8_dcp(demand_period_dict)
#bkg_8 = convert_to_8_dcp (bkg_period_dict)
#print_diff2 (demand_8, qbkg_dict, 0)
#print_diff2 (demand_8, unc_demand_dict, 0)
#qbkg_8 = convert_to_8_dcp (qbkg_dict)
#unc_8 = convert_to_8_dcp (unc_demand_dict)
#print_diff2 (demand_8, qbkg_8, 0)
#print_diff2 (demand_8, unc_8, 0)
   
print_final_results (demand_period_dict, qbkg_dict, unc_demand_dict, 16)

#print demand_period_dict
#print unc_demand_dict

# Print the output dictionaries
#print_dict (demand_dict)
#print_dict (bkg_dict)
#print_distrib (demand_dict)
#print_distrib (bkg_dict)
be = 13
si = 16
#print_period_dict (demand_period_dict, be, si)
#print_period_dict (qbkg_dict, be, si)
#print_period_dict (unc_demand_dict, be, si)
