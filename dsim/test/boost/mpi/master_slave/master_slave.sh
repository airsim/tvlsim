# Execute master on localhost, 4 slave on localhost (or fed1, fed2)
#!/bin/sh

/usr/lib/openmpi/bin/mpirun --host localhost -n 1  master : \
                            --host localhost -n 4 slave
