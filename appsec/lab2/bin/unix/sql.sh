#!/usr/bin/env bash

# Set up MySQL database

# Change to folder with database configurations
cd ../../db;

# THIS WILL FAIL IF ROOT USER HAS A PASSWORD NOT SET TO NONE

mysql -u root -e "create database insecapp";

mysql -u root insecapp < appdatabase.sql;
