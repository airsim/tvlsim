(*
 * dSim
 *
 * Gestion des bases de données
 *
 * 01/12/2009 - 18:58 ~ db.ml (c) William Le Ferrand
 *)

open Mysql

exception Error = Error

let connectdb db = 
  let db_info =  
    {
      dbhost = Some("ncewleferrand");
      dbname = Some(db);
      dbport = None;
      dbpwd  = Some("pods3030");
      dbuser = Some("root")} in
    
  let connection = ref (connect db_info) in
    
  let exec chaine = 
    try
      exec !connection chaine
    with _ -> 
      (
	E.warning dbd ;
	connection := connect db_info;
	exec !connection chaine
      ) in
    
    ignore (exec "SET CHARACTER SET 'utf8'") ;
    ignore (exec "SET collation_connection = 'utf8_general_ci'") ;  
    exec
      


(** Querying the users database *)
let users = connectdb "users"
let rds = connectdb "rds"

 
let fetch = fetch
let size = size

let secure = secure

    
