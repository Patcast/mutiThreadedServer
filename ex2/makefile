# Create your own targets that compile the application
database: database sensor_db.c main.c
	@gcc -o database sensor_db.c main.c -lsqlite3 -std=c99
	@./database
# the files for ex2 will be ziped and are then ready to
# be submitted to labtools.groept.be
zip:
	@zip lab6_ex2.zip sensor_db.c config.h
