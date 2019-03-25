```sql
# database
create database COMPANY;
use COMPANY;

set @@foreign_key_checks=OFF;
create table EMPLOYEE(
    ENAME varchar(20),
    ESSN char(18),
    ADDRESS varchar(100),
    SALARY long,
    SUPERSSN char(18),
    DNO varchar(20),
    primary key(ESSN),
    foreign key(DNO) references DEPARTMENT(DNO)
);

create table DEPARTMENT(
    DNAME varchar(20),
    DNO varchar(20),
    MGRSSN varchar(20),
    MGRSTARTDATE datetime,
    primary key(DNO),
    foreign key(MGRSSN) references EMPLOYEE(ESSN)
);

create table PROJECT(
    PNAME varchar(20),
    PNO varchar(20),
    PLOCATION varchar(100),
    DNO varchar(20),
    primary key(PNO),
    foreign key(DNO) references DEPARTMENT(DNO)
);

create table WORKS_ON(
    ESSN varchar(20),
    PNO varchar(20),
    HOURS int,
    primary key(ESSN,PNO)
);
set @@foreign_key_checks=ON;
```
