```sql
# database
create database COMPANY character set utf8  COLLATE utf8_general_ci;

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
load data local infile "./lab1/employee.txt" into table EMPLOYEE;
load data local infile "./lab1/department.txt" into table DEPARTMENT;
set @@foreign_key_checks=ON;
load data local infile "./lab1/project.txt" into table PROJECT;
load data local infile "./lab1/works_on.txt" into table WORKS_ON;

select ENAME
from EMPLOYEE,WORKS_ON,PROJECT
where PNAME='SQL Project' and
	  PROJECT.PNO=WORKS_ON.PNO and
	  WORKS_ON.ESSN=EMPLOYEE.ESSN;

select ENAME,ADDRESS
from EMPLOYEE,DEPARTMENT
where DNAME='Research Department' and
	  DEPARTMENT.DNO=EMPLOYEE.DNO and
	  SALARY<3000;

select ENAME
from EMPLOYEE
where EMPLOYEE.ESSN not in(
	select ESSN
	from WORKS_ON
	where PNO='P1'
);

select ENAME,DNAME
from EMPLOYEE,DEPARTMENT
where SUPERSSN=(
		select ESSN
		from EMPLOYEE
		where ENAME='张红'
	  ) and
	  EMPLOYEE.DNO=DEPARTMENT.DNO;


select ESSN
from EMPLOYEE
where ESSN in(
		  select ESSN
		  from WORKS_ON
		  where PNO='P1'
	  ) and
	  ESSN in(
		  select ESSN
		  from WORKS_ON
		  where PNO='P2'
	  );


select ESSN,ENAME
from EMPLOYEE
where ESSN in(
		  select ESSN
		  from WORKS_ON
		  group by ESSN having count(distinct PNO)=(
			  select count(distinct PNO)
			  from PROJECT
		  )
	  );

select DNAME
from EMPLOYEE,DEPARTMENT
where EMPLOYEE.DNO=DEPARTMENT.DNO
group by DNAME having avg(SALARY)<3000;

select ENAME
from EMPLOYEE
where ESSN in (
		  select ESSN
		  from WORKS_ON
		  group by ESSN having count(distinct PNO)>=3 and sum(HOURS)<=8
	  );

select avg(AVG_SALARY)
from (
	select EMPLOYEE.ESSN as ESSN,DNO,SALARY/SUM_HOURS as AVG_SALARY
	from EMPLOYEE,WORKS_ON,(
			select EMPLOYEE.ESSN,sum(HOURS) as SUM_HOURS
			from EMPLOYEE,WORKS_ON
			where EMPLOYEE.ESSN=WORKS_ON.ESSN
			group by EMPLOYEE.ESSN
		) t1
	where EMPLOYEE.ESSN=WORKS_ON.ESSN and
		EMPLOYEE.ESSN=t1.ESSN
) t2
group by DNO;
```
