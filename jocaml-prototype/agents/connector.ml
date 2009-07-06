(*
 * Dsim computing element
 *
 *)

open Printf
open Types

let host = ref "127.0.0.1"
let port = ref 12000

def wait() & release() = reply to wait

let _ =
  printf "DSIM Computing Element\n"; flush stdout ;
  Arg.parse [
    "-ip", Arg.String (fun s -> host := s), "<xxx.xxx.xxx.xxx>: ip core" ;
    "-port", Arg.Int (fun s -> port := s), "port" ;  
      
  ] 
    (fun t -> print_string t)
    (
      " Element\n Options are :"
    ) ;

  printf "Looking for core server\n"; flush stdout;

  let asim_core = Join.Site.there (Unix.ADDR_INET((Unix.inet_addr_of_string !host),21111)) in
  let ns = Join.Ns.of_site asim_core in
    Join.Site.at_fail asim_core release ;  
   
  let register  = 
    (Join.Ns.lookup ns "register" : (Join.Site.t -> unit)) in
    
    printf "Core server found\n"; flush stdout;
    
    Join.Ns.register Join.Ns.here "update_schedule" (Guillotine.add: (schedule_elt -> unit)); 
    
    Join.Ns.register Join.Ns.here "book" (Guillotine.book: (booking_request -> unit)); 
    
    Join.Ns.register Join.Ns.here "midnight" (Guillotine.midnight: (unit -> unit)); 
  
    Join.Ns.register Join.Ns.here "display" (Guillotine.display: (unit -> unit)); 
  
    Join.Ns.register Join.Ns.here "dump" (Guillotine.dump: ((string * int * int) -> unit)); 
  
    
     Join.Site.listen 
       (
	 Unix.ADDR_INET (
	   Join.Site.get_local_addr (),
	   !port
	 )
       ) ;
     printf "Using register channel\n"; flush stdout;
    
     register (Join.Site.here); 
      
    
     printf "Process agent ready\n"; flush stdout;
    
     wait ()

  


  
