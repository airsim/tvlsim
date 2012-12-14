(* 
 * Asim nutshell
 *
 *)

open Types
open Printf


def release () & wait () = reply to wait 


    
let _ = 
  printf "Launching TVLSIM\n"; flush stdout; 
  
  (** External functions exposure *)
  
  Join.Ns.register Join.Ns.here "register" (Agents.register: Join.Site.t -> unit);

  Join.Ns.register Join.Ns.here "remote_control" (Simulation.ui: action -> unit);
    
  Join.Site.listen 
    (
      Unix.ADDR_INET (
	Join.Site.get_local_addr (), 21111)
    ) ;
      
  wait ()
