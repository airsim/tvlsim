# Import section
import math

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
        print date, dtd_list
        #print date, nb
    stddev = stddev / flights
    stddev = math.sqrt (stddev)
    #print mean, stddev

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
def sum_period (demand_dict):
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
            if (x > -30 and x <= -27):
                period_dtd[5] += 1
            if (x > -27 and x <= -23):
                period_dtd[6] += 1
            if (x > -23 and x <= -20):
                period_dtd[7] += 1
            if (x > -20 and x <= -16):
                period_dtd[8] += 1
            if (x > -16 and x <= -13):
                period_dtd[9] += 1
            if (x > -13 and x <= -9):
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
def print_diff (dict1, dict2):
    diff_list=[]
    for x in range(16):
        diff_list.append(0.0)
    for date, unc_dem_list in dict2.items():
        dem_list = dict1[date]
        for x in range(16):
            if (dem_list[x] != 0):
                diff_list[x]+=100*abs(unc_dem_list[x] - dem_list[x])/dem_list[x]
                #diff_list[x]+=abs(unc_dem_list[x] - dem_list[x])
    for x in range(16):
        diff_list[x] = round(diff_list[x]/len(dict2),3)
    print diff_list

#########################################################################
def extract_date (fd):
    month_dict = {'Jan':'01', 'Feb':'02', 'Mar':'03', 'Apr':'04', 'Mai':'05', 'Jun':'06', 'Jul':'07','Aug':'08','Sep':'09','Oct':'10','Nov':'11','Dec':'12'}
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
bkg_dict={}
for i in lines:
    headline, datedtd = i.split(': ')
    # RMOL
    if (headline == "[N]../../../rmol/command/Forecaster.cpp:690") or \
            (headline == "[N]../../../rmol/command/Detruncator.cpp:220"):
        fd, dtd, begin, end, hfd, dem = datedtd.split(';')
        try:
            unc_demand_dict[hfd]
        except (KeyError):
            unc_demand_dict[hfd] = []
        dem_list = unc_demand_dict[hfd]
        dem_float = float (dem)
        dem_list.append (dem_float)

    # TRADEMGEN
    if (headline == "[N]../../../trademgen/bom/DemandStream.cpp:380") or \
            (headline == "[N]../../../trademgen/bom/DemandStream.cpp:274"):
        date, dtd = datedtd.split(';')
        try:
            demand_dict[date]
        except (KeyError):
            demand_dict[date] = []
        dtd_list = demand_dict[date]
        dtd_float = float (dtd)
        dtd_list.append (dtd_float)
        
    # BKGs
    if (headline == "[N]../../../dsim/command/Simulator.cpp:215"):
        c, fd, od, t, dtd = datedtd.split(';')
        date = extract_date (fd)
        try:
            bkg_dict[date]
        except (KeyError):
            bkg_dict[date] = []
        dtd_list = bkg_dict[date]
        dtd_float = float (dtd)
        dtd_list.append (dtd_float)


demand_period_dict = sum_period (demand_dict)
bkg_period_dict = sum_period (bkg_dict)


print_diff (demand_period_dict, unc_demand_dict)
print_diff (demand_period_dict, bkg_period_dict)
    

#print demand_period_dict
#print unc_demand_dict

# Print the output dictionaries
#print_dict (demand_dict)
#print_dict (bkg_dict)
#print_distrib (demand_dict)
#print_distrib (bkg_dict)
#print_dict (demand_period_dict)
#print_dict (bkg_period_dict)
#print_dict (unc_demand_dict)
