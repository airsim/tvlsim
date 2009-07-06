(* 
 * DSIM 
 *
 *)

open Types
open Printf


let rec schedule_generator airline_code class_number = function
    0 -> []
  | n -> { descr = airline_code, n ; 
	   number_of_classes = class_number } :: (schedule_generator airline_code class_number (n - 1))

let schedule = 
  List.fold_left (fun acc (airline_code, class_number, flight_number) -> 
		    schedule_generator airline_code class_number flight_number @ acc) [] 
    [
      "BA", 2, 10
      
    ]
  
(*
let schedule = 
  [
    { descr = "BA", 1 ; 
      number_of_classes = 10 } ;
    { descr = "AF", 1 ; 
      number_of_classes = 10 } ;
    { descr = "BA", 2 ; 
      number_of_classes = 10 } ;
    { descr = "AF", 2 ; 
      number_of_classes = 10 } ;
    { descr = "BA", 3 ; 
      number_of_classes = 10 } ;
    { descr = "AF", 3 ; 
      number_of_classes = 10 } ;




  ]

*)
def ui (action) = 
  (
    match action with 
	RUN (duration) -> 
	  
	printf "Starting simulation\n"; flush stdout;
	
	Agents.init schedule ; 
	
	printf "Client initialized\n"; flush stdout;

	for current_date = 1 to duration do 
	  printf "Date: %d\n" current_date; flush stdout;

	  Agents.midnight (); 
	  for i = 0 to 10 do 
	    let booking_request = { 
	      airline = "BA" ;
	      
	      date = current_date + 365 - (Random.int (min current_date 365)) ;
	      
	      flight_number = (Random.int 10) + 1; 
	      class_code = (Random.int 2) + 1 ; 
	      
	    } in
	      if (current_date > duration - 10) then
	      Agents.dump ("nceorilnx02", 3306, 1) ;
	      Agents.book booking_request ; 

	  done ;
	done  
      | DISPLAY -> Agents.display ()
  ); 

	reply to ui
	 
