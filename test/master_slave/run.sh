# Execute log_server on localhost, 4 client on localhost (or fed1, fed2)
#!/bin/sh
rm -fv log

/usr/lib/openmpi/bin/mpirun --host localhost -n 1  ./master :\
							--host localhost -n 1 ./log_server :\
                            --host localhost -n 2 ./slave
