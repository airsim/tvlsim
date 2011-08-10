# Import section
import math

# Min "acceptable" mean for aggregation
M = 1.0

#########################################################################
def format_aggregate_destinations (dest_miu, agg_miu):
    agg_dest = ""
    
    for dest, miu in dest_miu.items():
        if (agg_dest != ""):
            agg_dest += ","
        agg_dest += dest
        agg_dest += ":"
        agg_dest += str (float (miu/agg_miu))
        
    return agg_dest

#########################################################################
def aggregate_in_one_group (dest_dict):
    aggregated_dict = {}
    agg_miu = 0.0
    agg_sigma = 0.0
    agg_dest_miu = {}

    # Output directly the distributions whose mean is greater than N, and aggregate all the others
    for dest, distrib in dest_dict.items():
        
        current_miu = float (distrib[0])
        
        if (current_miu >= M):
            dest += ":1.0"
            aggregated_dict[dest] = distrib
        else:
            if (current_miu > 0.0):
                agg_miu += current_miu
                current_sigma = float (distrib[1])
                agg_sigma += current_sigma * current_sigma
                agg_dest_miu[dest] = current_miu
            
    agg_sigma = math.sqrt (agg_sigma)
    agg_dest = format_aggregate_destinations (agg_dest_miu, agg_miu)
    aggregated_dict[agg_dest] = [agg_miu, agg_sigma]
    
    return aggregated_dict

#########################################################################
def find_min_element_of_list (input_list):
    idx = 0
    min_value = input_list[idx]
    current_index = 0

    for value in input_list:
        if (min_value > value):
            min_value = value
            idx = current_index
        current_index += 1

    return idx

#########################################################################
def aggregate_in_n_groups (dest_dict, n):
    aggregated_dict = {}
    agg_miu = [0.0] * n
    agg_sigma = [0.0] * n
    agg_dest_miu = {}

    # Output directly the distributions whose mean is greater than N, and aggregate all the others
    for dest, distrib in dest_dict.items():
        current_miu = float (distrib[0])
        
        if (current_miu >= M):
            dest += ":1.0"
            aggregated_dict[dest] = distrib
            
        else:
            if (current_miu > 0.0):
                idx = find_min_element_of_list (agg_miu)
                agg_miu[idx] += current_miu
                current_sigma = float (distrib[1])
                agg_sigma[idx] += current_sigma * current_sigma
                try:
                    agg_dest_miu[idx]
                except (KeyError):
                    agg_dest_miu[idx] = {}
                agg_dest_miu_at_idx = agg_dest_miu[idx]
                agg_dest_miu_at_idx[dest] = current_miu

    for i in range (0, n):
        agg_sigma[i] = math.sqrt (agg_sigma[i])
        agg_dest = format_aggregate_destinations (agg_dest_miu[i], agg_miu[i])
        aggregated_dict[agg_dest] = [agg_miu[i], agg_sigma[i]]
            
    return aggregated_dict
    
#########################################################################
def aggregate (dest_dict):
    # Compute the number of aggregation groups
    total_miu = 0.0
    for miu, sigma in dest_dict.values():
        miu = float (miu)
        if (miu < M):
            total_miu += miu
            
    if (total_miu > 0.0):
        number_of_groups = int (total_miu / M)
        if (number_of_groups == 0): 
            number_of_groups += 1

        # If there will be only one group
        if (number_of_groups == 1):
            return aggregate_in_one_group (dest_dict)
    
        # If there will be more than one group
        if (number_of_groups > 1):
            return aggregate_in_n_groups (dest_dict, number_of_groups)


#########################################################################
def print_output (output_dict):
   for origin_cabin, aggregate_dict in output_dict.items():
       for dest, distrib in aggregate_dict.items():
           print '{0};{1};{2};{3};{4}'.format (origin_cabin[0], dest, origin_cabin[1], distrib[0], distrib[1])


#########################################################################
############################### M A I N #################################
#########################################################################

input_file = open('demand_mon.txt')
#input_file = open('test02.txt')
lines = input_file.readlines()

data_dict={}
for i in lines:
    ori, dest, cab, miu, sigma = i[0:-1].split(',')
    try:
        data_dict[ori, cab]
    except (KeyError):
        data_dict[ori, cab] = {}
    dest_dict = data_dict[ori, cab]
    dest_dict[dest] = [miu, sigma]
    
output_dict = {}
for key, value in data_dict.items():
    agg_dest_dict = aggregate (value)
    if (agg_dest_dict != None):
        output_dict[key] = agg_dest_dict
    
# Print the output dictionnary
print_output (output_dict)
    


