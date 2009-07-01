(*
 * Prototype of a distributed simulator
 *
 *)

type booking_request = 
    { 
      airline : string; 
      date : int;
      flight_number : int;
      class_code : int 
    } 


type schedule_elt = 
    { descr: string * int ; 
      number_of_classes: int }

type schedule = schedule_elt list

module Client = 
  struct 
    type t = string * int 
    let compare = compare
  end 

module Balancer = Map.Make (Client)


(** Basic actions *)

type action = RUN of int | DISPLAY
