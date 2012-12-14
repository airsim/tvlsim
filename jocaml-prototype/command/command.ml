(*
 * TVLSIM Command
 *
 *)


open Printf

let host = ref "127.0.0.1"
let port = ref 21111

def wait() & release() = reply to wait

type action = RUN of int | DISPLAY

let _ =
  printf "TVLSIM Command\n"; flush stdout ;
  Arg.parse [
    "-ip", Arg.String (fun s -> host := s), "<xxx.xxx.xxx.xxx>: ip core" ;
    "-port", Arg.Int (fun s -> port := s), "port" ;
      
  ] 
    (fun t -> print_string t)
    (
      " Airline\n Options are :"
    ) ;

  printf "Looking for core server\n"; flush stdout;

  let asim_core = Join.Site.there (Unix.ADDR_INET((Unix.inet_addr_of_string !host), !port)) in
  let ns = Join.Ns.of_site asim_core in
    Join.Site.at_fail asim_core release ;  
 
  let ui  = 
    (Join.Ns.lookup ns "remote_control" : (action -> unit)) in
    
  let run i = 
    let bt = Unix.gettimeofday () in 
    ui (RUN(i)) ;
      let et = Unix.gettimeofday () in 
	
    printf "Job done in %f seconds\n" (et -. bt); flush stdout
    
  in

 let display _ = 
    ui (DISPLAY) ;
    printf "Done\n"; flush stdout
    
  in

    while true do 
      printf "TVLSIM %% "; flush stdout; 
      let request = input_line stdin in
	  Scanf.sscanf request "RUN %i" run

	  
    done 
