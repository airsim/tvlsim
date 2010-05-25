#Execute trademgen on three machines: local, fed1 and fed2
#!/bin/sh

/usr/lib/openmpi/bin/mpirun --host localhost -n 1  master : \
                            --host fed1 -n 4 slave
