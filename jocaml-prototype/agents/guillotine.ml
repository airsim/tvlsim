(*
 * Dsim guillotine
 *
 * Imperative style + 1
 *
 *)


open Printf
open Types

open Mysql

(** Two operations : booking & forecasting *)

let airlines = Hashtbl.create 10

  
let date = ref 0 


def book (booking_request) = 
  
  let date = !date in
  let flights_table = 
    Hashtbl.find airlines booking_request.airline
  in
  
  let class_table = 
    Hashtbl.find flights_table booking_request.flight_number 
  in
  
  let guillotine = 
    class_table.(booking_request.class_code - 1)
  in

 
  let day_to_departure =  
    booking_request.date - date 
  in
    
  let flights_list = 
    guillotine.(day_to_departure - 1)  
  in
    
  incr (Hashtbl.find flights_list booking_request.date).real ; 
  
    reply to book


(** Forecaster *)

def forecast (date) = 
  (** Naive forecaster based on averages *)
   
    let forecast_guillotine guillotine = 
      Array.iteri (fun dtd flight_list -> 
		     
		     let average = ref 0.0 in
		       
		       for d = 366 to (date + 365 - dtd) do  (* <- ... *)
			 			 
	
			 average := 
			   !average +. 
			     (float_of_int !((Hashtbl.find flight_list d).real)) ;
			

		       done ; 
		       
		   if (date - dtd + 1 > 0) then
		       average := !average /. (float_of_int (date - dtd + 1)); 
		       
		       (** Updating for  the remaining dates *)
		       
		       for d = (max 366 (date + 365 - dtd + 1)) to date + 365 do 
		
			 (Hashtbl.find flight_list d).expected := !average ; 
			 (* Update average here *)
		       done 
			 
		  ) guillotine
    in
      
    let forecast_flight _ class_table = 
      Array.iter (function guillotine -> forecast_guillotine guillotine) class_table
    in
      
    let forecast_airline _ flights_table =
      Hashtbl.iter forecast_flight flights_table
    in
      
      Hashtbl.iter forecast_airline airlines ;
      reply to forecast
	
	
	
(** This function has to be called once and ONLY once *)
	
	
let init_tables schedule_elt = 
  
  let airline_code, flight_number = schedule_elt.descr in 
    
  let airline = 
    try 
      Hashtbl.find airlines airline_code 
    with Not_found -> Hashtbl.create 10 in

  let class_table = 
    Array.init schedule_elt.number_of_classes (function _ -> 
				    Array.init 365 (fun _ -> Hashtbl.create 10)
				 ) in
    
    Hashtbl.add airline flight_number class_table ; 
    Hashtbl.replace airlines airline_code airline (* dirty*)


(** This function has to be called at midnight every day *)

let open_flights date schedule =
    
    List.iter 
      (fun schedule_elt -> 
	 let airline_code, flight_number = schedule_elt.descr in
	 let flights_table = 
	   Hashtbl.find airlines airline_code
	 in

	 let class_table = Hashtbl.find flights_table flight_number in
	
	   Array.iter (function guillotine -> 
			 Array.iter (function flight_list -> 
				       Hashtbl.add flight_list (date + 365) ({real = ref 0; expected = ref 0.0})) guillotine
		      ) class_table

      ) schedule  

def display () = 
  printf "Internal table:\n\n" ;
  
  let display_column fd record = 
    printf "Flight-date: %d -> Real: %d; Expected: %f\n" fd !(record.real) !(record.expected)
  in
    
  let display_guillotine guillotine = 
    Array.iteri (fun dtd flights ->
    printf "++++ %d:\n" dtd ; 
    Hashtbl.iter display_column flights) guillotine
  in

  let display_classes class_code guillotine = 
    printf "+++ %d:\n" class_code; 
    display_guillotine guillotine
  in
    
  let display_flights flight_number classes = 
    printf "++ %d:\n" flight_number ; 
    Array.iteri display_classes classes
  in
      
  let display_airlines airline_code airline_flights = 
    printf "+ %s:\n" airline_code ;
    Hashtbl.iter display_flights airline_flights 
  in

    Hashtbl.iter display_airlines airlines ; 
    reply to display




def dump (server, port, rid) = 
    
  let db_info =  
    {
      dbhost = Some(server);
      dbname = Some("simbi");
      dbport = Some(port);
      dbpwd  = Some("pods3030");
      dbuser = Some("root")} in
    
  let dbhandler = connect db_info in

  let dump_airlines airline_code airline_flights = 
    let dump_flights flight_number classes = 
      let dump_classes class_code guillotine =
	let dump_guillotine dtd flights = 
	  let dump_flight flight_date record = 
	    let insert_sql = 
	      sprintf "INSERT INTO tvlsim_dump (RID, airline_code, flight_number, class_code, day_to_departure, flight_date, insertion_date, booking_counter, forecast) VALUES (\"%d\", \"%s\", \"%d\", \"%d\", \"%d\", \"%d\", \"%d\",  \"%d\", \"%f\")" 
		rid airline_code flight_number class_code dtd flight_date !date !(record.real) !(record.expected) in 
	    ignore(exec dbhandler insert_sql)
	  in
	  Hashtbl.iter dump_flight flights 
	in
	Array.iteri dump_guillotine guillotine
      in
      Array.iteri dump_classes classes
    in
    Hashtbl.iter dump_flights airline_flights
  in
    Hashtbl.iter dump_airlines airlines; 
    reply to dump


def schedule (l) & add (se) = 
  init_tables se ;
  schedule (se::l) & reply to add

or  schedule (l) & midnight () = 
   incr date ; 
   open_flights !date l ;
   flush stdout; 
   forecast !date; 
  
  
  schedule (l) & reply to midnight ;;
  
spawn schedule ([]) ;;
