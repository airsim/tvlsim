(*
 * Dsim computing node 
 *
 *)


open Types 

(** Node registration *)

type node = 
    {
      site : Join.Site.t ; 
      book : booking_request -> unit ;
      update_schedule : schedule_elt -> unit ;
      midnight: unit -> unit; 
      dump: (string * int * int) -> unit ; 
      display: unit -> unit;
    }

(** book will lock in case of pool exhaustion *)
(** book is synchronous *)

def nodes(l) & remove (site) = 
    nodes (List.filter (function node -> not (Join.Site.equal node.site site)) l)  & reply to remove

or  nodes(l) & register (site) = 
    
    let book_stub : booking_request -> unit = 
      ((Join.Ns.lookup (Join.Ns.of_site site) "book")) in
    let update_schedule_stub : schedule_elt -> unit = 
      ((Join.Ns.lookup (Join.Ns.of_site site) "update_schedule")) in
    let midnight_stub : unit -> unit = 
      ((Join.Ns.lookup (Join.Ns.of_site site) "midnight")) in
    let display_stub : unit -> unit = 
      ((Join.Ns.lookup (Join.Ns.of_site site) "display")) in
    let dump_stub : (string * int * int) -> unit = 
      ((Join.Ns.lookup (Join.Ns.of_site site) "dump")) in
    
    let node = 
	{ site = site; 
	  book = book_stub; 
	  update_schedule = update_schedule_stub; 
	  midnight = midnight_stub;
	  dump = dump_stub; 
	  display = display_stub } in
	  
	Join.Site.at_fail site (def r () = remove site; 0 in r); 
	nodes(node :: l) & reply to register 

or  nodes(l) & partition (schedule) = 
    let _,_, balancermap =
      List.fold_left (fun connectors  schedule_elt -> 
		   match connectors with 
		       (t::sl, el, mp) -> 
			 t.update_schedule schedule_elt ; 
			 (sl, t::el, Balancer.add schedule_elt.descr t mp)
		     | ([], t::el, mp) -> 
			 t.update_schedule schedule_elt ; 
			 (el, [t], Balancer.add schedule_elt.descr t mp)
		) (l, [], Balancer.empty) schedule in
    nodes (l) & reply (balancermap, l) to partition ;;
	

spawn nodes ([])  ;;

def counter(n) & up () = 
  counter (n + 1) & reply to up
or counter(0) & wait () = 
   counter(0) & reply to wait 
or counter(n) & down () = 
  counter (n - 1) & reply to down;;

spawn counter (0) ;;

def balancer (_) & init (schedule) = 
    balancer(partition (schedule)) & reply to init

or  balancer (balancermap, l) & book (booking_request) = 
    let agent = Balancer.find (booking_request.airline, booking_request.flight_number) balancermap in 
      agent.book booking_request ; 
    balancer (balancermap, l) & reply to book 

or  balancer (balancermap, l) & dump (server, port, rid) = 
     List.iter 
      (function agent -> 
	 up () ; spawn (agent.dump (server, port, rid); down (); 0)) l ; 
    wait () ;
    balancer (balancermap, l) & reply to dump
or  balancer (balancermap, l) & midnight () = 
    List.iter 
      (function agent -> 
	 up () ; spawn (agent.midnight (); down (); 0)) l ; 
    wait () ; 
    balancer(balancermap, l) & reply to midnight 

or  balancer (balancermap, l) & display () = 
    List.iter 
      (function agent -> 
	 up () ; spawn (agent.display (); down (); 0)) l ; 
    wait () ; 
    balancer(balancermap, l) & reply to display

;;

spawn balancer (Balancer.empty, []) ;;

