(*
 * TVLSIM Types
 *
 *)


type booking_request = 
    { 
      airline : string; 
      date : int;
      flight_number : int;
      class_code : int 
    } 

type booking_counter = {real: int ref; expected : float ref}

type schedule_elt = 
    { descr: string * int ; 
      number_of_classes: int }

type schedule = schedule_elt list
